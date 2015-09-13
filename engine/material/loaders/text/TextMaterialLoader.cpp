/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "TextMaterialLoader.h"
#include "engine/utility/FileUtils.h"
#include "engine/material/Material.h"
#include "engine/material/MaterialTechnique.h"
#include "engine/material/MaterialPass.h"
#include "engine/core/Log.h"
#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/trace.hh>
#include <vector>
#include <cassert>

namespace Engine
{
    namespace
    {
        static const std::string gEmptyString;

        namespace Grammar
        {
            using namespace pegtl;
            using namespace pegtl::ascii;

            // Comments are introduced by a '//' and proceed to the end-of-line/file.
            struct Comment : if_must<string<'/','/'>, until<eolf>> {};

            // The parser ignores all spaces and comments; space is a pegtl rule
            // that matches the usual ascii characters ' ', '\t', '\n' etc. In other
            // words, everything that is space or a comment is ignored.
            struct WhitespaceElement : sor<space, Comment> {};
            struct Whitespace : plus<WhitespaceElement> {};
            struct OptionalWhitespace : star<WhitespaceElement> {};

            // FloatValue <= '-'? [0-9]+ ('.' [0-9]+)? ([eE] [+-]? [0-9]+)?
            struct FloatValue : seq<
                opt<one<'-'>>,                      // '-'?
                plus<digit>,                        // [0-9]+
                opt<                                // (
                    one<'.'>,                       // '.'
                    plus<digit>                     // [0-9]+
                >,                                  // )?
                opt<                                // (
                    one<'e', 'E'>,                  // [eE]
                    opt<one<'+', '-'>>,             // [+-]?
                    plus<digit>                     // [0-9]+
                >                                   // )?
            > {};

            ///////////////////////////////
            // Options

            struct LINE_WIDTH : seq<string<'l','i','n','e','-','w','i','d','t','h'>, OptionalWhitespace> {};
            struct LineWidthOption : seq<LINE_WIDTH, one<':'>, OptionalWhitespace, FloatValue, OptionalWhitespace> {};

            struct Option : seq<sor<
                LineWidthOption
            >, OptionalWhitespace> {};

            ///////////////////////////////
            // Passes

            struct PASS : string<'p','a','s','s'> {};

            struct PassName : identifier {};
            struct NoPassName : success {};
            struct OptionalPassName : seq<sor<seq<Whitespace, PassName>, NoPassName>, OptionalWhitespace> {};

            struct PassBegin : seq<PASS, OptionalPassName, one<'{'>, OptionalWhitespace> {};
            struct PassEnd : seq<one<'}'>, OptionalWhitespace> {};

            struct Pass : seq<PassBegin, star<Option>, PassEnd> {};

            ///////////////////////////////
            // Techniques

            struct TECHNIQUE : string<'t','e','c','h','n','i','q','u','e'> {};

            struct TechniqueName : identifier {};
            struct NoTechniqueName : success {};
            struct OptionalTechniqueName : seq<sor<seq<Whitespace, TechniqueName>, NoTechniqueName>, OptionalWhitespace> {};

            struct TechniqueBegin : seq<TECHNIQUE, OptionalTechniqueName, one<'{'>, OptionalWhitespace> {};
            struct TechniqueEnd : seq<one<'}'>, OptionalWhitespace> {};

            struct Technique : seq<TechniqueBegin, star<sor<Option, Pass>>, TechniqueEnd> {};

            ///////////////////////////////

            // Primary production of the grammar
            struct File : must<OptionalWhitespace, star<sor<Option, Technique>>, eof> {};
        }

        ///////////////////////////////

        namespace AST
        {
            struct Option {
                enum Name {
                    LineWidth,
                };

                virtual ~Option() = default;
                virtual Name name() const = 0;
                virtual void applyToPass(MaterialPass& pass) const = 0;
            };

            template <Option::Name NAME> struct FloatOption : Option {
                const float value;
                FloatOption(float v) : value(v) {}
                Name name() const override { return NAME; }
                void applyToPass(MaterialPass& pass) const override;
            };

            struct OptionList {
                std::vector<std::shared_ptr<Option>> options;
            };

            struct Pass : public OptionList {
                std::unique_ptr<std::string> name;
            };

            struct Technique : public OptionList {
                std::unique_ptr<std::string> name;
                std::vector<std::shared_ptr<Pass>> passes;
            };

            struct File : public OptionList {
                std::vector<std::shared_ptr<Technique>> techniques;
            };

            // Options

            using LineWidthOption = FloatOption<Option::LineWidth>;
            template <> void LineWidthOption::applyToPass(MaterialPass& pass) const {
                pass.setLineWidth(value);
            }
        }

        ///////////////////////////////

        namespace Actions
        {
            struct Context {
                std::vector<float> floatValues;
                std::unique_ptr<std::string> techniqueName;
                std::unique_ptr<std::string> passName;
                std::vector<std::shared_ptr<AST::OptionList>> optionLists;
                std::shared_ptr<AST::Pass> currentPass;
                std::shared_ptr<AST::Technique> currentTechnique;
                std::shared_ptr<AST::File> currentFile;

                Context() {
                    currentFile = std::make_shared<AST::File>();
                    optionLists.emplace_back(currentFile);
                }

                AST::OptionList& currentOptionList() {
                    assert(!optionLists.empty());
                    return *optionLists.back();
                }

                template <class TYPE, class... ARGS> void emitOption(ARGS&&... args) {
                    currentOptionList().options.emplace_back(std::make_shared<TYPE>(std::forward<ARGS>(args)...));
                }

                void pushOptionList(std::shared_ptr<AST::OptionList>&& list) {
                    optionLists.emplace_back(list);
                }

                void popOptionList(AST::OptionList& current) {
                    assert(!optionLists.empty());
                    assert(optionLists.back().get() == &current);
                    optionLists.pop_back();
                }
            };

            template <class TYPE> TYPE pop(std::vector<TYPE>& array) {
                assert(!array.empty());
                TYPE value(std::move(array.back()));
                array.pop_back();
                return value;
            }

            ///////////////////////////////

            template <class RULE> struct Action : pegtl::nothing<RULE> {};

            // Options

            template <> struct Action<Grammar::FloatValue> {
                static void apply(const pegtl::input& input, Context& context) {
                    context.floatValues.emplace_back(std::stof(input.string()));
                }
            };

            template <> struct Action<Grammar::LineWidthOption> {
                static void apply(const pegtl::input&, Context& context) {
                    context.emitOption<AST::LineWidthOption>(pop(context.floatValues));
                }
            };

            // Passes

            template <> struct Action<Grammar::PassBegin> {
                static void apply(const pegtl::input&, Context& context) {
                    assert(context.currentPass == nullptr);
                    assert(context.currentTechnique != nullptr);
                    context.currentPass = std::make_shared<AST::Pass>();
                    context.currentPass->name = std::move(context.passName);
                    context.currentTechnique->passes.emplace_back(context.currentPass);
                    context.pushOptionList(context.currentPass);
                    context.passName.reset();
                }
            };

            template <> struct Action<Grammar::PassEnd> {
                static void apply(const pegtl::input&, Context& context) {
                    assert(context.currentPass != nullptr);
                    context.popOptionList(*context.currentPass);
                    context.currentPass.reset();
                }
            };

            template <> struct Action<Grammar::PassName> {
                static void apply(const pegtl::input& input, Context& context) {
                    context.passName.reset(new std::string(input.string()));
                }
            };

            template <> struct Action<Grammar::NoPassName> {
                static void apply(const pegtl::input&, Context& context) {
                    context.passName.reset();
                }
            };

            // Techniques

            template <> struct Action<Grammar::TechniqueBegin> {
                static void apply(const pegtl::input&, Context& context) {
                    assert(context.currentTechnique == nullptr);
                    context.currentTechnique = std::make_shared<AST::Technique>();
                    context.currentTechnique->name = std::move(context.techniqueName);
                    context.currentFile->techniques.emplace_back(context.currentTechnique);
                    context.pushOptionList(context.currentTechnique);
                    context.techniqueName.reset();
                }
            };

            template <> struct Action<Grammar::TechniqueEnd> {
                static void apply(const pegtl::input&, Context& context) {
                    assert(context.currentTechnique != nullptr);
                    context.popOptionList(*context.currentTechnique);
                    context.currentTechnique.reset();
                }
            };

            template <> struct Action<Grammar::TechniqueName> {
                static void apply(const pegtl::input& input, Context& context) {
                    context.techniqueName.reset(new std::string(input.string()));
                }
            };

            template <> struct Action<Grammar::NoTechniqueName> {
                static void apply(const pegtl::input&, Context& context) {
                    context.techniqueName.reset();
                }
            };
        }
    }

    ///////////////////////////////

    MaterialPtr TextMaterialLoader::loadMaterial(IFile* file)
    {
        if (!file)
            return std::make_shared<Material>();

        std::vector<char> data = FileUtils::loadFile(file);

        const char* p = data.data();
        size_t size = data.size();
        if (size >= 3 && uint8_t(p[0]) == 0xEF && uint8_t(p[1]) == 0xBB && uint8_t(p[2]) == 0xBF) {
            Z_LOGW("Material file \"" << file->name() << "\" contains an UTF-8 byte order mark.");
            p += 3;
            size -= 3;
        }

        Actions::Context context;
        try {
            assert(pegtl::analyze<Grammar::File>() == 0);
            pegtl::parse<Grammar::File, Actions::Action>(p, size, "", context);
        } catch (const pegtl::parse_error& error) {
            Z_LOGE("Unable to parse material \"" << file->name() << "\": " << error.what());
        }

        assert(context.currentTechnique == nullptr);
        assert(context.currentPass == nullptr);
        assert(context.optionLists.size() == 1);
        assert(&context.currentOptionList() == context.currentFile.get());
        assert(context.floatValues.empty());
        assert(context.techniqueName == nullptr);
        assert(context.passName == nullptr);

        auto rootAST = context.currentFile;
        auto material = std::make_shared<Material>();

        for (const auto& techniqueAST : rootAST->techniques) {
            const std::string& techniqueName = (techniqueAST->name ? *techniqueAST->name : gEmptyString);
            auto technique = std::make_shared<MaterialTechnique>(techniqueName);

            for (const auto& passAST : techniqueAST->passes) {
                const std::string& passName = (passAST->name ? *passAST->name : gEmptyString);
                auto pass = std::make_shared<MaterialPass>(passName);

                for (const auto& option : rootAST->options)
                    option->applyToPass(*pass);
                for (const auto& option : techniqueAST->options)
                    option->applyToPass(*pass);
                for (const auto& option : passAST->options)
                    option->applyToPass(*pass);

                technique->addPass(pass);
            }

            material->addTechnique(std::move(technique));
        }

        return material;
    }

    bool TextMaterialLoader::canLoadMaterial(IFile* file)
    {
        // FIXME
        (void)file;
        return true;
    }
}
