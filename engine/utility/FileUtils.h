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
#include "engine/interfaces/io/IFile.h"
#include <vector>
#include <string>

namespace B3D
{
    namespace FileUtils
    {
        std::string makeFullPath(const std::string& fileName, const std::string& parentFileName);
        std::string extractBaseName(const std::string& fileName);

        std::vector<char> loadFile(const std::string& fileName);
        std::vector<char> loadFile(const FilePtr& file);
        std::vector<char> loadFile(IFile* file);

        std::vector<std::string> loadFileLines(const std::string& fileName, bool includeEolMarker = true);
        std::vector<std::string> loadFileLines(const FilePtr& file, bool includeEolMarker = true);
        std::vector<std::string> loadFileLines(IFile* file, bool includeEolMarker = true);
    }
}
