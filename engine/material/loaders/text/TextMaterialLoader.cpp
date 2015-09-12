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
#include "engine/core/Log.h"
#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/trace.hh>
#include <vector>
#include <cassert>

namespace Engine
{
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

        // FloatValue <= '-'? [0-9]+ ('.' [0-9]+)? ([eE] [+-]? [0-9]+)? Whitespace*
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
            >,                                  // )?
            OptionalWhitespace                  // Whitespace*
        > {};

        ///////////////////////////////
        // Options

        struct LINE_WIDTH : seq<string<'l','i','n','e','-','w','i','d','t','h'>, OptionalWhitespace> {};
        struct LineWidthOption : seq<LINE_WIDTH, one<':'>, OptionalWhitespace, FloatValue> {};

        struct Option : seq<sor<
            LineWidthOption
        >, OptionalWhitespace> {};


        ///////////////////////////////
        // Techniques

        struct TECHNIQUE : seq<string<'t','e','c','h','n','i','q','u','e'>> {};
        struct TechniqueBegin : seq<TECHNIQUE, opt<Whitespace, identifier>, OptionalWhitespace, one<'{'>, OptionalWhitespace> {};
        struct TechniqueEnd : seq<one<'}'>, OptionalWhitespace> {};

        struct Technique : seq<TechniqueBegin, star<Option>, TechniqueEnd> {};

        ///////////////////////////////

        // Primary production of the grammar
        struct File : must<OptionalWhitespace, star<sor<Option, Technique>>, eof> {};
    }

    namespace Actions
    {
        template <class RULE> struct Action : pegtl::nothing<RULE> {};
    }

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

        try {
            assert(pegtl::analyze<Grammar::File>() == 0);
            pegtl::parse<Grammar::File, Actions::Action>(p, size, "");
        } catch (const pegtl::parse_error& error) {
            Z_LOGE("Unable to parse material \"" << file->name() << "\": " << error.what());
            abort();
        }

        return std::make_shared<Material>();
    }

    bool TextMaterialLoader::canLoadMaterial(IFile* file)
    {
        // FIXME
        return true;
    }
}
