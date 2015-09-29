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

#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/render/lowlevel/IShader.h"
#include "engine/interfaces/io/IFile.h"
#include <vector>
#include <string>

namespace B3D
{
    class ShaderLoader
    {
    public:
        ShaderLoader();
        ~ShaderLoader();

        const std::vector<std::string>& vertexSource() const { return mVertex; }
        const std::vector<std::string>& fragmentSource() const { return mFragment; }

        bool loadFile(const std::string& fileName, std::vector<std::string>* what = nullptr);
        bool loadFile(const FilePtr& file, std::vector<std::string>* what = nullptr);
        bool loadFile(IFile* file, std::vector<std::string>* what = nullptr);

        bool loadMemory(const std::string& fileName, const std::vector<std::string>& lines,
            std::vector<std::string>* what = nullptr);
        bool loadMemory(const std::string& fileName, std::vector<std::string>&& lines,
            std::vector<std::string>* what = nullptr);

        static ShaderPtr compile(const std::string& fileName, const std::vector<std::string>& lines);
        static ShaderPtr compile(const std::string& fileName, std::vector<std::string>&& lines);

    private:
        std::vector<std::string> mVertex;
        std::vector<std::string> mFragment;

        FilePtr openIncludeFile(std::string fileName, const std::string& parentFileName) const;

        B3D_DISABLE_COPY(ShaderLoader);
    };
}
