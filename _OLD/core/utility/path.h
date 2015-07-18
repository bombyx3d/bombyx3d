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
#include <string>

namespace Engine
{
    /**
     * Retrieves offset of the last directory separator (usually '/' or '\\').
     * @param path Path.
     * @return Offset of the last directory separator from the beginning of the string
     * or `std::string::npos` if provided string does not have directory separators.
     */
    size_t pathFindLastDirectorySeparator(const std::string& path);

    /**
     * Retrieves offset of the file name extension (the part after the last '.').
     * @param path Path.
     * @return Offset of the last '.' in the file name from the beginning of the string
     * or `std::string::npos` if provided file name does not have an extension.
     */
    size_t pathFindFileNameExtension(const std::string& path);

    /**
     * Extracts extension (the part after the last '.') from the provided file name.
     * @param path Path.
     * @return Extension or empty string if file name does not have an extension.
     */
    std::string pathGetFileNameExtension(const std::string& path);
}
