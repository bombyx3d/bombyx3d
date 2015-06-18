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
#include "StaticMemoryInputStream.h"
#include <string>

namespace Engine
{
    StaticMemoryInputStream::StaticMemoryInputStream(const void* data, size_t length, const std::string& fileName)
        : StaticMemoryFile(data, length, fileName)
        , m_Offset(0)
        , m_BytesLeft(length)
    {
    }

    const std::string& StaticMemoryInputStream::name() const
    {
        return StaticMemoryFile::name();
    }

    const void* StaticMemoryInputStream::rawDataPointer() const
    {
        return StaticMemoryFile::rawDataPointer();
    }

    size_t StaticMemoryInputStream::rawDataSize() const
    {
        return StaticMemoryFile::rawDataSize();
    }

    bool StaticMemoryInputStream::atEnd() const
    {
        return m_BytesLeft == 0;
    }

    uint64_t StaticMemoryInputStream::bytesAvailable() const
    {
        return m_BytesLeft;
    }

    uint64_t StaticMemoryInputStream::size() const
    {
        return StaticMemoryFile::size();
    }

    bool StaticMemoryInputStream::skip(size_t count)
    {
        if (count == 0)
            return true;

        if (count > m_BytesLeft)
        {
            m_BytesLeft = 0;
            return false;
        }

        m_Offset += count;
        m_BytesLeft -= count;

        return true;
    }

    size_t StaticMemoryInputStream::read(void* buffer, size_t size)
    {
        if (m_BytesLeft == 0 || size == 0)
            return 0;

        if (size > m_BytesLeft)
            size = m_BytesLeft;

        if (!StaticMemoryFile::read(m_Offset, buffer, size))
        {
            m_BytesLeft = 0;
            return 0;
        }

        m_Offset += size;
        m_BytesLeft -= size;

        return size;
    }

    bool StaticMemoryInputStream::read(uint64_t offset, void* buffer, size_t bytesToRead)
    {
        return StaticMemoryFile::read(offset, buffer, bytesToRead);
    }
}
