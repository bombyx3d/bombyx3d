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
#include "engine/utility/StringUtils.h"
#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <vector>
#include <cassert>

#ifdef _MSC_VER
 #pragma warning(disable:4316)      // object allocated on the heap may not be aligned 16
#endif

namespace Engine
{
    namespace
    {
        static const std::string gEmptyString;

        namespace Grammar
        {
            using namespace pegtl;
            using namespace pegtl::ascii;
            #include "ParseGrammar.h"
        }

        namespace Tree
        {
            #include "ParseTree.h"
        }

        namespace GrammarActions
        {
            #include "ParseGrammarContext.h"
            #include "ParseGrammarActions.h"
        }
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

        GrammarActions::Context context;
        context.materialFileName = file->name();

        try {
            assert(pegtl::analyze<Grammar::File>() == 0);
            pegtl::parse<Grammar::File, GrammarActions::Action>(p, size, "", context);
        } catch (const pegtl::parse_error& error) {
            Z_LOGE("Unable to parse material \"" << file->name() << "\": " << error.what());
            return std::make_shared<Material>();
        }

        assert(context.currentTechnique == nullptr);
        assert(context.currentPass == nullptr);
        assert(context.optionLists.size() == 1);
        assert(&context.currentOptionList() == context.currentFile.get());
        assert(context.boolValues.empty());
        assert(context.stringValues.empty());
        assert(context.floatValues.empty());
        assert(context.blendFuncValues.empty());
        assert(context.uniformValue == nullptr);
        assert(context.techniqueName == nullptr);
        assert(context.passName == nullptr);

        auto rootTree = context.currentFile;
        auto material = std::make_shared<Material>();

        for (const auto& techniqueTree : rootTree->techniques) {
            const std::string& techniqueName = (techniqueTree->name ? *techniqueTree->name : gEmptyString);
            auto technique = std::make_shared<MaterialTechnique>(techniqueName);

            if (techniqueTree->passes.empty()) {
                Z_LOGW("In material file \"" << file->name()
                    << "\": technique \"" << techniqueName << "\" does not have passes.");
            }

            for (const auto& passTree : techniqueTree->passes) {
                const std::string& passName = (passTree->name ? *passTree->name : gEmptyString);
                auto pass = std::make_shared<MaterialPass>(passName);

                for (const auto& option : rootTree->options)
                    option->applyToPass(*pass);
                for (const auto& option : techniqueTree->options)
                    option->applyToPass(*pass);
                for (const auto& option : passTree->options)
                    option->applyToPass(*pass);

                for (const auto& uniform : rootTree->uniforms)
                    uniform.second->applyToPass(*pass, uniform.first);
                for (const auto& uniform : techniqueTree->uniforms)
                    uniform.second->applyToPass(*pass, uniform.first);
                for (const auto& uniform : passTree->uniforms)
                    uniform.second->applyToPass(*pass, uniform.first);

                technique->addPass(pass);
            }

            material->addTechnique(std::move(technique));
        }

        return material;
    }

    bool TextMaterialLoader::canLoadMaterial(IFile* file)
    {
        return file && StringUtils::endsWith(file->name(), ".material");
    }
}
