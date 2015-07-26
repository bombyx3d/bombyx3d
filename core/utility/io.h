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
#include "core/interfaces/io/IInputStream.h"
#include <string>
#include <vector>

namespace Engine
{
    /**
     * Reads single line of text from an input stream.
     * This function interprets `LF` and `CR-LF` as line separators.
     * @param stream Pointer to the input stream.
     * @param includeEolMarker Set to `true` to include '\n' into the returned string
     * (if it was present in the input stream).
     * @return The string that has been read.
     */
    std::string ioReadLine(IInputStream* stream, bool includeEolMarker = false);

    /**
     * Reads all remaining bytes from an input stream.
     * @return Vector of bytes.
     */
    std::vector<char> ioReadAll(IInputStream* stream);
}
