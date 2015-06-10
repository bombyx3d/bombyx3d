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
#include "opengl/api/error.h"
#include "io/streams/StaticMemoryInputStream.h"
#include "utility/debug.h"
#include <cstring>

namespace Z
{
    const std::string Shader::POSITION_ATTRIBUTE_NAME = "a_position";
    const std::string Shader::COLOR_ATTRIBUTE_NAME = "a_color";
    const std::string Shader::TEXCOORD0_ATTRIBUTE_NAME = "a_texcoord0";

    const std::string Shader::PROJECTION_MATRIX_UNIFORM_NAME = "u_projection";
    const std::string Shader::MODELVIEW_MATRIX_UNIFORM_NAME = "u_modelview";
    const std::string Shader::COLOR_UNIFORM_NAME = "u_color";
    const std::string Shader::TEXTURE0_UNIFORM_NAME = "u_texture0";

    Shader::Shader()
    {
        memset(m_UniformLocations, -1, sizeof(m_UniformLocations));
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

    bool Shader::hasUniform(Uniform uniform) const
    {
        Z_CHECK(uniform >= 0 && uniform < NUM_STANDARD_UNIFORMS);
        if (uniform < 0 || uniform >= NUM_STANDARD_UNIFORMS)
            return false;
        return m_UniformLocations[uniform] >= 0;
    }

    int Shader::getUniformLocation(Uniform uniform) const
    {
        Z_CHECK(uniform >= 0 && uniform < NUM_STANDARD_UNIFORMS);
        if (uniform < 0 || uniform >= NUM_STANDARD_UNIFORMS)
            return -1;
        Z_CHECK_GL_LOCATION(glGetUniformLocation, standardUniformName(uniform).c_str(), m_UniformLocations[uniform]);
        return m_UniformLocations[uniform];
    }

    void Shader::unload()
    {
        m_GLProgram.reset();
    }

    const std::string& Shader::standardUniformName(Uniform uniform)
    {
        switch (uniform)
        {
        case ProjectionMatrixUniform: return PROJECTION_MATRIX_UNIFORM_NAME;
        case ModelViewMatrixUniform: return MODELVIEW_MATRIX_UNIFORM_NAME;
        case ColorUniform: return COLOR_UNIFORM_NAME;
        case Texture0Uniform: return TEXTURE0_UNIFORM_NAME;
        case NUM_STANDARD_UNIFORMS: break;
        }

        Z_DEBUG_FAIL("Invalid standard uniform ID.");
        static const std::string empty;
        return empty;
    }

    void Shader::bindAttributes()
    {
        Z_ASSERT(m_GLProgram);
        m_GLProgram->bindAttribLocation(PositionAttribute, POSITION_ATTRIBUTE_NAME.c_str());
        m_GLProgram->bindAttribLocation(ColorAttribute, COLOR_ATTRIBUTE_NAME.c_str());
        m_GLProgram->bindAttribLocation(TexCoord0Attribute, TEXCOORD0_ATTRIBUTE_NAME.c_str());
    }

    void Shader::enumerateStandardUniforms()
    {
        Z_CHECK(m_GLProgram);
        if (m_GLProgram) {
            for (int i = 0; i < NUM_STANDARD_UNIFORMS; i++) {
                const char* name = standardUniformName(Uniform(i)).c_str();
                Z_ASSERT(*name != 0);
                m_UniformLocations[i] = m_GLProgram->getUniformLocation(name, true);
            }
        }
    }

    void ShaderFromStaticMemory::reload()
    {
        m_GLProgram.reset(new GLProgram);
        bindAttributes();

        StaticMemoryInputStream stream(m_Data, m_Size, m_FileName);
        m_GLProgram->load(&stream);

        enumerateStandardUniforms();
    }

    void ShaderFromFile::reload()
    {
        m_GLProgram.reset(new GLProgram);
        bindAttributes();
        m_GLProgram->load(m_FileName);
        enumerateStandardUniforms();
    }
}
