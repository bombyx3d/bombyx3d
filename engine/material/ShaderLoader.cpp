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
#include "ShaderLoader.h"
#include "engine/utility/FileUtils.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include <cassert>

namespace Engine
{
    static const std::string VERTEX = "%vertex";
    static const std::string VERTEX_LF = "%vertex\n";
    static const std::string FRAGMENT = "%fragment";
    static const std::string FRAGMENT_LF = "%fragment\n";
    static const std::string COMMON = "%common";
    static const std::string COMMON_LF = "%common\n";
    static const std::string INCLUDE = "%include ";
    static const std::string LF = "\n";

    ShaderLoader::ShaderLoader()
    {
    }

    ShaderLoader::~ShaderLoader()
    {
    }

    bool ShaderLoader::loadFile(const std::string& fileName, std::vector<std::string>* what)
    {
        FilePtr file = Services::fileSystem()->openFile(fileName);
        return loadFile(file.get(), what);
    }

    bool ShaderLoader::loadFile(const FilePtr& file, std::vector<std::string>* what)
    {
        return loadFile(file.get(), what);
    }

    bool ShaderLoader::loadFile(IFile* file, std::vector<std::string>* what)
    {
        return file && loadMemory(file->name(), FileUtils::loadFileLines(file, true), what);
    }

    bool ShaderLoader::loadMemory(const std::string& fileName, const std::vector<std::string>& lines,
        std::vector<std::string>* what)
    {
        std::vector<std::string> linesCopy = lines;
        return loadMemory(fileName, std::move(linesCopy), what);
    }

    bool ShaderLoader::loadMemory(const std::string& fileName, std::vector<std::string>&& lines,
        std::vector<std::string>* what)
    {
        bool success = true;
        int lineNumber = 0;

        for (auto& line : lines) {
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
                    what = &mVertex;
                else if (line == FRAGMENT_LF || line == FRAGMENT)
                    what = &mFragment;
                else if (line == COMMON_LF || line == COMMON)
                    what = nullptr;
                else if (line.substr(0, INCLUDE.length()) == INCLUDE) {
                    auto include = openIncludeFile(line.substr(INCLUDE.length()), fileName);
                    success = loadFile(include.get(), what) && success;
                } else {
                    Z_LOGE(fileName << "(" << lineNumber << "): invalid directive.");
                    what = nullptr;
                    success = false;
                }
                continue;
            }

            if (what)
                what->emplace_back(std::move(line));
            else {
                mVertex.emplace_back(line);
                mFragment.emplace_back(std::move(line));
            }
        }

        if (!success)
            Z_LOGE("Unable to load shader \"" << fileName << "\".");

        return success;
    }

    ShaderPtr ShaderLoader::compile(const std::string& fileName, const std::vector<std::string>& lines)
    {
        ShaderLoader loader;
        ShaderPtr shader = Services::renderer()->createShader();

        if (loader.loadMemory(fileName, lines)) {
            shader->setVertexSource(loader.vertexSource());
            shader->setFragmentSource(loader.fragmentSource());
            shader->compile();
        }

        return shader;
    }

    ShaderPtr ShaderLoader::compile(const std::string& fileName, std::vector<std::string>&& lines)
    {
        ShaderLoader loader;
        ShaderPtr shader = Services::renderer()->createShader();

        if (loader.loadMemory(fileName, std::move(lines))) {
            shader->setVertexSource(loader.vertexSource());
            shader->setFragmentSource(loader.fragmentSource());
            shader->compile();
        }

        return shader;
    }

    FilePtr ShaderLoader::openIncludeFile(std::string fileName, const std::string& parentFileName) const
    {
        FilePtr file;

        size_t length = fileName.length();
        if (length > 0 && fileName[length - 1] == '\n')
            fileName.resize(--length);

        std::string name = FileUtils::makeFullPath(fileName, parentFileName);
        if (Services::fileSystem()->fileExists(name))
            return Services::fileSystem()->openFile(name);

        return Services::fileSystem()->openFile(fileName);
    }
}
