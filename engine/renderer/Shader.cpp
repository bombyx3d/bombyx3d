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
#include "Shader.h"
#include "io/streams/StaticMemoryInputStream.h"
#include "utility/debug.h"

namespace Z
{
    const std::string Shader::POSITION_ATTRIBUTE_NAME = "a_position";
    const std::string Shader::TEXCOORD0_ATTRIBUTE_NAME = "a_texcoord0";

    Shader::Shader()
    {
    }

    Shader::~Shader()
    {
    }

    bool Shader::use()
    {
        if (!m_GLProgram)
            return false;
        return m_GLProgram->use();
    }

    int Shader::getAttribLocation(const char* name) const
    {
        Z_CHECK(m_GLProgram != nullptr);
        return (m_GLProgram ? m_GLProgram->getAttribLocation(name) : -1);
    }

    int Shader::getAttribLocation(const std::string& name) const
    {
        return getAttribLocation(name.c_str());
    }

    int Shader::getUniformLocation(const char* name) const
    {
        Z_CHECK(m_GLProgram != nullptr);
        return (m_GLProgram ? m_GLProgram->getUniformLocation(name) : -1);
    }

    int Shader::getUniformLocation(const std::string& name) const
    {
        return getUniformLocation(name.c_str());
    }

    void Shader::unload()
    {
        m_GLProgram.reset();
    }

    void Shader::bindAttributes()
    {
        Z_ASSERT(m_GLProgram);
        m_GLProgram->bindAttribLocation(PositionAttribute, POSITION_ATTRIBUTE_NAME.c_str());
        m_GLProgram->bindAttribLocation(TexCoord0Attribute, TEXCOORD0_ATTRIBUTE_NAME.c_str());
    }

    void ShaderFromStaticMemory::reload()
    {
        m_GLProgram.reset(new GLProgram);
        bindAttributes();

        StaticMemoryInputStream stream(m_Data, m_Size, m_FileName);
        m_GLProgram->load(&stream);
    }

    void ShaderFromFile::reload()
    {
        m_GLProgram.reset(new GLProgram);
        bindAttributes();
        m_GLProgram->load(m_FileName);
    }
}
