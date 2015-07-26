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
#include "io.h"
#include <sstream>

namespace Engine
{
    std::string ioReadLine(IInputStream* stream, bool includeEolMarker)
    {
        std::stringstream ss;

        if (!stream)
            return ss.str();

        while (!stream->atEnd()) {
            char ch = 0;
            stream->read(&ch, 1);
            if (ch == '\n') {
                if (includeEolMarker)
                    ss << ch;
                break;
            }
            ss << ch;
        }

        std::string str = ss.str();
        size_t length = str.length();

        if (!includeEolMarker) {
            if (length > 0 && str[length - 1] == '\r')
                str.resize(length - 1);
        } else {
            if (length > 1 && str[length - 2] == '\r' && str[length - 1] == '\n') {
                str[length - 2] = '\n';
                str.resize(length - 1);
            }
        }

        return str;
    }

    std::vector<char> ioReadAll(IInputStream* stream)
    {
        std::vector<char> result;

        if (!stream)
            return result;

        while (!stream->atEnd()) {
            uint64_t size = stream->bytesAvailable();
            if (size == 0) {
                char ch = 0;
                size_t bytesRead = stream->read(&ch, 1);
                if (bytesRead > 0)
                    result.push_back(ch);
            } else {
                size_t offset = result.size();
                size_t bytesToRead = size_t(size);
                result.resize(offset + bytesToRead);
                size_t bytesRead = stream->read(result.data() + offset, bytesToRead);
                if (bytesRead != bytesToRead)
                    result.resize(offset + bytesRead);
            }
        }

        return result;
    }
}
