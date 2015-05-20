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
#include "StaticMemoryFile.h"
#include "utility/debug.h"
#include <cstring>

namespace Z
{
    StaticMemoryFile::StaticMemoryFile(const void* data, size_t length, const std::string& name)
        : m_Name(name)
        , m_Data(reinterpret_cast<const char*>(data))
        , m_Length(length)
    {
        Z_ASSERT(length == 0 || data != nullptr);
    }

    StaticMemoryFile::~StaticMemoryFile()
    {
    }

    const std::string& StaticMemoryFile::name() const
    {
        return m_Name;
    }

    uint64_t StaticMemoryFile::size() const
    {
        return m_Length;
    }

    bool StaticMemoryFile::read(uint64_t offset, void* buffer, size_t size)
    {
        if (offset + size > m_Length) {
            Z_LOG("Incomplete read in file \"" << m_Name << "\".");
            return false;
        }

        memcpy(buffer, m_Data + offset, size);

        return true;
    }
}
