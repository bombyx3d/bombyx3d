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
#include "BuildRule.h"
#include <sstream>

BuildRule::BuildRule()
{
}

BuildRule::~BuildRule()
{
}

void BuildRule::addInputFile(const std::string& file)
{
    m_InputFiles.emplace_back(file);
}

void BuildRule::addOutputFile(const std::string& file)
{
    m_OutputFiles.emplace_back(file);
}

std::string BuildRule::settingsHash() const
{
    MD4_CTX context;
    MD4_Init(&context);

    hashString(&context, m_Name);
    hashStringVector(&context, m_InputFiles);
    hashStringVector(&context, m_OutputFiles);

    calcSettingsHash(&context);

    unsigned char buf[16];
    MD4_Final(buf, &context);

    char result[32];
    for (size_t i = 0; i < 16; i++) {
        static const char hex[] = "0123456789abcdef";
        result[i * 2 + 0] = hex[buf[i] >> 4];
        result[i * 2 + 1] = hex[buf[i] & 15];
    }

    return std::string(result, sizeof(result));
}

void BuildRule::hashSizeT(MD4_CTX* context, size_t value)
{
    hashUInt64T(context, uint64_t(value));
}

void BuildRule::hashUInt64T(MD4_CTX* context, uint64_t value)
{
    MD4_Update(context, &value, sizeof(value));
}

void BuildRule::hashString(MD4_CTX* context, const std::string& string)
{
    hashSizeT(context, string.length());
    MD4_Update(context, string.data(), static_cast<unsigned long>(string.length()));
}

void BuildRule::hashStringVector(MD4_CTX* context, const std::vector<std::string>& vec)
{
    hashSizeT(context, vec.size());
    for (const auto& string : vec)
        hashString(context, string);
}
