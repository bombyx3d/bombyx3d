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
#include "api/opengl.h"
#include "core/utility/Ptr.h"
#include "core/interfaces/IFileReader.h"
#include "core/interfaces/IInputStream.h"
#include <memory>

namespace Z
{
    using namespace Engine;

    class GLProgram
    {
    public:
        GLProgram();
        ~GLProgram();

        GL::UInt handle() const { return m_Handle; }

        bool use() const;
        static void unbindAll();

        void bindAttribLocation(int index, const char* name) { gl::BindAttribLocation(m_Handle, index, name); }
        int getAttribLocation(const char* name) const { return gl::GetAttribLocation(m_Handle, name); }

        int getUniformLocation(const char* name, bool ignoreFailure = false) const
            { return gl::GetUniformLocation(m_Handle, name, ignoreFailure); }

        virtual Ptr<IInputStream> openIncludeFile(std::string filename, const std::string& parentFileName) const;
        virtual bool parseProgramSource(IInputStream* input, std::vector<std::string>& vertex,
            std::vector<std::string>& fragment, std::vector<std::string>* what = nullptr) const;

        void printSource() const;

        bool load(const std::string& file);
        bool load(const Ptr<IFileReader>& fileReader);
        bool load(const Ptr<IInputStream>& inputStream);
        bool load(IInputStream* input);

        bool loadSource(const std::string& vertex, const std::string& fragment);

        bool link();

    private:
        GL::UInt m_Handle = 0;

        GLProgram(const GLProgram&) = delete;
        GLProgram& operator=(const GLProgram&) = delete;
    };

    using GLProgramPtr = std::shared_ptr<GLProgram>;
}
