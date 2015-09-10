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
#include "WinAPI.h"

namespace Engine
{
    std::vector<WCHAR> Win32::latin1ToWideChar(const std::string& string, size_t extraLength)
    {
        std::vector<WCHAR> result(string.length() + extraLength);
        for (unsigned char ch : string) {
            if (ch < 32 || ch == 127)
                result.push_back(WCHAR('?'));
            else
                result.push_back(WCHAR(ch));
        }
        return result;
    }

    std::vector<WCHAR> Win32::multiByteToWideChar(const std::string& string, size_t extraLength)
    {
        const char* stringData = string.data();
        int stringLength = int(string.length());

        int unicodeLength = MultiByteToWideChar(CP_UTF8, 0, stringData, stringLength, nullptr, 0);
        if (unicodeLength < 0)
            return latin1ToWideChar(string, extraLength);

        std::vector<WCHAR> unicode(size_t(unicodeLength) + extraLength);
        unicodeLength = MultiByteToWideChar(CP_UTF8, 0, stringData, stringLength, unicode.data(), unicodeLength);
        if (unicodeLength < 0)
            return latin1ToWideChar(string, extraLength);

        unicode.resize(size_t(unicodeLength) + extraLength);
        return unicode;
    }
}
