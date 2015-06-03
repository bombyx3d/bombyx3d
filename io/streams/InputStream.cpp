/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include "InputStream.h"
#include "utility/debug.h"
#include <sstream>
#include <algorithm>

namespace Z
{
    bool InputStream::readAtMost(void* buffer, size_t size, size_t* bytesRead)
    {
        Z_ASSERT(bytesRead != nullptr);

        unsigned char* p = reinterpret_cast<unsigned char*>(buffer);
        *bytesRead = 0;

        while (size > 0) {
            if (atEnd())
                break;

            uint64_t haveBytes = bytesAvailable();
            size_t bytesToRead = size_t(std::min(uint64_t(size), haveBytes));
            if (!read(p, bytesToRead))
                return false;

            p += bytesToRead;
            *bytesRead += bytesToRead;
            size -= bytesToRead;
        }

        return true;
    }

    std::string InputStream::readLine(bool includeEolMarker)
    {
        std::stringstream ss;

        while (!atEnd()) {
            char ch = 0;
            read(&ch, 1);
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

    std::vector<char> InputStream::readAll()
    {
        std::vector<char> result;

        while (!atEnd()) {
            uint64_t size = bytesAvailable();
            if (size == 0) {
                char ch = 0;
                size_t bytesRead = read(&ch, 1);
                if (bytesRead > 0)
                    result.push_back(ch);
            } else {
                size_t offset = result.size();
                size_t bytesToRead = size_t(size);
                result.resize(offset + bytesToRead);
                size_t bytesRead = read(result.data() + offset, bytesToRead);
                if (bytesRead != bytesToRead)
                    result.resize(offset + bytesRead);
            }
        }

        return result;
    }
}
