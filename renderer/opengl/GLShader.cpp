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
#include "GLShader.h"
#include "api/error.h"
#include "utility/debug.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace Z
{
    GLShader::GLShader(GL::Enum type)
        : m_Type(type)
    {
        m_Handle = gl::CreateShader(type);
    }

    GLShader::~GLShader()
    {
        gl::DeleteShader(m_Handle);
    }

    void GLShader::setSource(const std::string& source)
    {
        GL::Int lengths = GL::Int(source.length());
        const GL::Char* lines = reinterpret_cast<const GL::Char*>(source.c_str());
        gl::ShaderSource(m_Handle, 1, &lines, &lengths);
    }

    void GLShader::setSource(const std::vector<std::string>& source)
    {
        std::vector<const GL::Char*> lines;
        std::vector<GL::Int> lengths;

        size_t count = source.size();
        lines.reserve(count);
        lengths.reserve(count);

        for (const auto& it : source) {
            lengths.push_back(GL::Int(it.length()));
            lines.push_back(reinterpret_cast<const GL::Char*>(it.c_str()));
        }

        gl::ShaderSource(m_Handle, GL::Sizei(count), lines.data(), lengths.data());
    }

    bool GLShader::compile()
    {
        gl::CompileShader(m_Handle);

        GL::Int success = GL::FALSE;
        gl::GetShaderiv(m_Handle, GL::COMPILE_STATUS, &success);

        if (!success) {
            Z_LOG("Unable to compile shader (" << m_Type << ").\n---");
            printSource();

            GL::Int logLength = 0;
            gl::GetShaderiv(m_Handle, GL::INFO_LOG_LENGTH, &logLength);

            if (logLength > 0) {
                std::vector<GL::Char> data(static_cast<size_t>(logLength));
                gl::GetShaderInfoLog(m_Handle, logLength, &logLength, data.data());

                std::string infoLog(data.data(), size_t(logLength));
                size_t length = infoLog.length();
                if (length > 0 && infoLog[length - 1] != '\n')
                    infoLog += '\n';

                if (logLength > 0)
                    Z_LOG("---\n" << infoLog << "---");
            }

            if (logLength <= 0)
                Z_LOG("---\nInfo log is not available.\n---");
        }

        return success != GL::FALSE;
    }

    void GLShader::printSource() const
    {
        GL::Int bufferLength = 0;
        gl::GetShaderiv(m_Handle, GL::SHADER_SOURCE_LENGTH, &bufferLength);

        std::vector<char> shaderSource(static_cast<size_t>(bufferLength));
        GL::Sizei sourceLength = 0;
        gl::GetShaderSource(m_Handle, bufferLength, &sourceLength, shaderSource.data());

        const char* p = shaderSource.data();
        const char* end = p + sourceLength;
        std::stringstream ss;
        bool printLineNumber = true;
        int lineNumber = 0;
        for (; p < end; ++p) {
            if (printLineNumber) {
                ss << std::setw(4) << ++lineNumber << std::setw(0) << ": ";
                printLineNumber = false;
            }

            ss << *p;
            if (*p == '\n')
                printLineNumber = true;
        }

        std::string source = ss.str();
        size_t length = source.length();
        if (length > 0 && source[length - 1] == '\n')
            source.resize(length - 1);

        Z_LOG(source);
    }
}
