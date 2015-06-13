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
#include "GLProgram.h"
#include "GLShader.h"
#include "core/io/streams/FileInputStream.h"
#include "core/interfaces/IFileReader.h"
#include "io/FileSystem.h"
#include "utility/debug.h"
#include "api/error.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace Z
{
    GLProgram::GLProgram()
    {
        m_Handle = gl::CreateProgram();
    }

    GLProgram::~GLProgram()
    {
        gl::DeleteProgram(m_Handle);
    }

    bool GLProgram::use() const
    {
        if (m_Handle != 0) {
            gl::UseProgram(m_Handle);
            return true;
        }
        return false;
    }

    void GLProgram::unbindAll()
    {
        gl::UseProgram(0);
    }

    Ptr<IInputStream> GLProgram::openIncludeFile(std::string filename, const std::string& parentFileName) const
    {
        Ptr<IFileReader> file;

        size_t length = filename.length();
        if (length > 0 && filename[length - 1] == '\n')
            filename.resize(--length);

        if (length > 0 && filename[0] == '/')
            filename = filename.substr(1);
        else {
            size_t index = parentFileName.rfind('/');
            std::string parentDir = (index == std::string::npos ? std::string() : parentFileName.substr(0, index + 1));
            std::string localFileName = parentDir + filename;
            if (FileSystem::defaultFileSystem()->fileExists(localFileName)) {
                Z_LOG(" - include file \"" << localFileName << "\".");
                file = FileSystem::defaultFileSystem()->openFile(localFileName);
            }
        }

        if (!file) {
            Z_LOG(" - include file \"" << filename << "\".");
            file = FileSystem::defaultFileSystem()->openFile(filename);
        }

        return std::make_shared<FileInputStream>(file);
    }

    bool GLProgram::parseProgramSource(IInputStream* input,
        std::vector<std::string>& vertex, std::vector<std::string>& fragment,
        std::vector<std::string>* what) const
    {
        bool success = true;
        int lineNumber = 0;

        Z_CHECK(input != nullptr);
        if (!input)
            return false;

        static const std::string VERTEX = "%vertex";
        static const std::string VERTEX_LF = "%vertex\n";
        static const std::string FRAGMENT = "%fragment";
        static const std::string FRAGMENT_LF = "%fragment\n";
        static const std::string BOTH = "%both";
        static const std::string BOTH_LF = "%both\n";
        static const std::string INCLUDE = "%include ";
        static const std::string LF = "\n";

        while (!input->atEnd()) {
            std::string line = input->readLine(true);
            ++lineNumber;

            size_t index = line.find("//");
            if (index != std::string::npos) {
                bool endsWithLF = false;
                size_t length = line.length();
                if (length > 1 && line[length - 1] == '\n')
                    endsWithLF = true;

                line = line.substr(0, index);

                if (endsWithLF)
                    line += '\n';
            }

            if (*line.c_str() == '%') {
                if (line == VERTEX_LF || line == VERTEX)
                    what = &vertex;
                else if (line == FRAGMENT_LF || line == FRAGMENT)
                    what = &fragment;
                else if (line == BOTH_LF || line == BOTH)
                    what = nullptr;
                else if (line.substr(0, INCLUDE.length()) == INCLUDE) {
                    auto include = openIncludeFile(line.substr(INCLUDE.length()), input->name());
                    success = parseProgramSource(include.get(), vertex, fragment, what) && success;
                } else {
                    Z_LOG(input->name() << "(" << lineNumber << "): invalid directive.");
                    what = nullptr;
                    success = false;
                }
                continue;
            }

            if (what)
                what->emplace_back(std::move(line));
            else {
                vertex.emplace_back(line);
                fragment.emplace_back(std::move(line));
            }
        }

        return success;
    }

    void GLProgram::printSource() const
    {
        GL::Int maxAttachedShaders = 0;
        gl::GetProgramiv(m_Handle, GL::ATTACHED_SHADERS, &maxAttachedShaders);

        std::vector<GL::UInt> shaders(static_cast<size_t>(maxAttachedShaders));
        GL::Sizei numAttachedShaders = 0;
        gl::GetAttachedShaders(m_Handle, maxAttachedShaders, &numAttachedShaders, shaders.data());

        std::stringstream ss;
        for (auto shader : shaders) {
            GL::Int type = 0;
            gl::GetShaderiv(shader, GL::SHADER_TYPE, &type);
            ss << "// " << GL::Enum(type) << '\n';

            GL::Int bufferLength = 0;
            gl::GetShaderiv(shader, GL::SHADER_SOURCE_LENGTH, &bufferLength);

            std::vector<char> shaderSource(static_cast<size_t>(bufferLength));
            GL::Sizei sourceLength = 0;
            gl::GetShaderSource(shader, bufferLength, &sourceLength, shaderSource.data());

            const char* start = shaderSource.data();
            const char* end = start + sourceLength;
            bool printLineNumber = true;
            int lineNumber = 0;
            for (const char* p = start; p < end; ++p) {
                if (printLineNumber) {
                    ss << std::setw(4) << ++lineNumber << std::setw(0) << ": ";
                    printLineNumber = false;
                }

                ss << *p;
                if (*p == '\n')
                    printLineNumber = true;
            }

            if (end > start && end[-1] != '\n')
                ss << '\n';
        }

        std::string source = ss.str();
        size_t length = source.length();
        if (length > 0 && source[length - 1] == '\n')
            source.resize(length - 1);

        Z_LOG(source);
    }

    bool GLProgram::load(const std::string& file)
    {
        Ptr<IFileReader> reader = FileSystem::defaultFileSystem()->openFile(file);
        if (!reader)
            return false;
        return load(reader);
    }

    bool GLProgram::load(const Ptr<IFileReader>& fileReader)
    {
        FileInputStream stream(fileReader);
        return load(&stream);
    }

    bool GLProgram::load(const Ptr<IInputStream>& inputStream)
    {
        return load(inputStream.get());
    }

    bool GLProgram::load(IInputStream* input)
    {
        Z_CHECK(input != nullptr);
        if (!input)
            return false;

        Z_LOG("Loading OpenGL program \"" << input->name() << "\".");

        std::vector<std::string> vertex;
        std::vector<std::string> fragment;
        bool success = parseProgramSource(input, vertex, fragment);

        GLShader vertexShader(GL::VERTEX_SHADER);
        vertexShader.setSource(vertex);
        success = vertexShader.compile() && success;
        gl::AttachShader(m_Handle, vertexShader.handle());

        GLShader fragmentShader(GL::FRAGMENT_SHADER);
        fragmentShader.setSource(fragment);
        success = fragmentShader.compile() && success;
        gl::AttachShader(m_Handle, fragmentShader.handle());

        return success && link();
    }

    bool GLProgram::loadSource(const std::string& vertex, const std::string& fragment)
    {
        bool success = true;

        GLShader vertexShader(GL::VERTEX_SHADER);
        vertexShader.setSource(vertex);
        success = vertexShader.compile() && success;
        gl::AttachShader(m_Handle, vertexShader.handle());

        GLShader fragmentShader(GL::FRAGMENT_SHADER);
        fragmentShader.setSource(fragment);
        success = fragmentShader.compile() && success;
        gl::AttachShader(m_Handle, fragmentShader.handle());

        return success && link();
    }

    bool GLProgram::link()
    {
        gl::LinkProgram(m_Handle);

        GL::Int success = GL::FALSE;
        gl::GetProgramiv(m_Handle, GL::LINK_STATUS, &success);

        if (!success) {
            Z_LOG("Unable to link program.\n---");
            printSource();

            GL::Int logLength = 0;
            gl::GetProgramiv(m_Handle, GL::INFO_LOG_LENGTH, &logLength);

            if (logLength > 0) {
                std::vector<GL::Char> data;
                data.resize(size_t(logLength));
                gl::GetProgramInfoLog(m_Handle, logLength, &logLength, data.data());

                std::string infoLog(data.data(), size_t(logLength));
                size_t length = infoLog.length();
                if (length > 0 && infoLog[length - 1] != '\n')
                    infoLog += '\n';

                if (logLength > 0)
                    Z_LOG("---\n" << infoLog << "---");
            }

            if (logLength <= 0)
                Z_LOG("---\nInfo log is not available.\n---");

            return false;
        }

        return true;
    }
}
