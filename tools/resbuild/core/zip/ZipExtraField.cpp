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
#include "ZipExtraField.h"
#include <cstring>
#include <algorithm>

bool ZipExtraField::fromByteArray(const std::vector<char>& data)
{
    if (data.size() != 40)
        return false;

    settingsHash.assign(data.data(), 32);

    buildTime =
        (uint64_t(uint8_t(data[32]))      ) |
        (uint64_t(uint8_t(data[33])) <<  8) |
        (uint64_t(uint8_t(data[34])) << 16) |
        (uint64_t(uint8_t(data[35])) << 24) |
        (uint64_t(uint8_t(data[36])) << 32) |
        (uint64_t(uint8_t(data[37])) << 40) |
        (uint64_t(uint8_t(data[38])) << 48) |
        (uint64_t(uint8_t(data[39])) << 52);

    return true;
}

void ZipExtraField::toByteArray(std::vector<char>& data) const
{
    data.resize(40);

    memset(data.data(), ' ', 32);
    memcpy(data.data(), settingsHash.data(), std::min(size_t(32), settingsHash.length()));

    data[32] = char((buildTime      ) & 0xFF);
    data[33] = char((buildTime >>  8) & 0xFF);
    data[34] = char((buildTime >> 16) & 0xFF);
    data[35] = char((buildTime >> 24) & 0xFF);
    data[36] = char((buildTime >> 32) & 0xFF);
    data[37] = char((buildTime >> 40) & 0xFF);
    data[38] = char((buildTime >> 48) & 0xFF);
    data[39] = char((buildTime >> 52) & 0xFF);
}
