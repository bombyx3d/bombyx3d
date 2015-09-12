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
#include <vector>
#include <cassert>

#ifndef NDEBUG
#include <pegtl/analyze.hh>
#endif

namespace Engine
{
    namespace Grammar
    {
        using namespace pegtl;
        using namespace pegtl::ascii;

        // Comments are introduced by a '//' and proceed to the end-of-line/file.
        struct Comment
            : if_must<string<'/','/'>, until<eolf>> {};

        // The parser ignores all spaces and comments; space is a pegtl rule
        // that matches the usual ascii characters ' ', '\t', '\n' etc. In other
        // words, everything that is space or a comment is ignored.
        struct Whitespace
            : sor<space, Comment> {};

        // Primary production of the grammar
        struct File
            : must<star<Whitespace>, eof> {};
    }

    namespace Errors
    {
        using namespace pegtl;
        using namespace pegtl::ascii;

        template <class RULE> struct Message : public normal<RULE>
        {
            static const std::string errorMessage;
            template <class INPUT, class... STATES> static void raise(const INPUT& in, STATES&&...) {
                throw parse_error(errorMessage, in);
            }
        };

        template<> const std::string Message<eof>::errorMessage = "syntax error.";
        template<> const std::string Message<star<Grammar::Whitespace>>::errorMessage = "syntax error.";
        template<> const std::string Message<until<eolf>>::errorMessage = "syntax error.";
    }

    namespace Actions
    {
        template <class RULE> struct Action : pegtl::nothing<RULE> {};
    }

  #ifndef NDEBUG
    static const size_t GrammarIssuesFound = pegtl::analyze<Grammar::File>();
  #endif

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
            assert(GrammarIssuesFound == 0);
            pegtl::parse<Grammar::File, Actions::Action, Errors::Message>(p, size, "");
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
