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
#include "opengl/GLProgram.h"
#include <string>
#include <memory>

namespace Z
{
    class Shader
    {
    public:
        static const std::string POSITION_ATTRIBUTE_NAME;
        static const std::string TEXCOORD0_ATTRIBUTE_NAME;

        enum Attribute
        {
            PositionAttribute = 0,
            TexCoord0Attribute,
        };

        Shader();
        virtual ~Shader();

        bool use();

        int getAttribLocation(const char* name) const;
        int getAttribLocation(const std::string& name) const;

        int getUniformLocation(const char* name) const;
        int getUniformLocation(const std::string& name) const;

        virtual void reload() = 0;
        void unload();

    protected:
        GLProgramPtr m_GLProgram;

        void bindAttributes();
    };

    class ShaderFromStaticMemory : public Shader
    {
    public:
        ShaderFromStaticMemory(const void* data, size_t size, const std::string& fileName)
            : m_Data(data), m_Size(size), m_FileName(fileName) {}
        void reload() override;

    private:
        const void* m_Data;
        size_t m_Size;
        std::string m_FileName;
    };

    class ShaderFromFile : public Shader
    {
    public:
        explicit ShaderFromFile(const std::string& fileName) : m_FileName(fileName) {}
        void reload() override;

    private:
        std::string m_FileName;
    };

    using ShaderPtr = std::shared_ptr<Shader>;
}
