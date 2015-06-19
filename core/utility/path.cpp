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
#include "path.h"

namespace Engine
{
    size_t pathFindLastDirectorySeparator(const std::string& path)
    {
        size_t index = path.rfind('/');
      #ifdef _WIN32
        size_t index2 = path.rfind('\\');
        if (index2 != std::string::npos && (index == std::string::npos || index2 > index))
            index = index2;
      #endif
        return index;
    }

    size_t pathFindFileNameExtension(const std::string& path)
    {
        size_t dotIndex = path.rfind('.');
        if (dotIndex == std::string::npos)
            return std::string::npos;

        size_t separatorIndex = pathFindLastDirectorySeparator(path);
        if (separatorIndex != std::string::npos && dotIndex <= separatorIndex)
            return std::string::npos;

        return dotIndex;
    }

    std::string pathGetFileNameExtension(const std::string& path)
    {
        size_t dotIndex = pathFindFileNameExtension(path);
        return (dotIndex == std::string::npos ? std::string() : path.substr(dotIndex + 1));
    }
}
