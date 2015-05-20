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
#include "FileInputStream.h"

namespace Z
{
    FileInputStream::FileInputStream(const FileReaderPtr& reader)
        : m_Reader(reader)
        , m_Offset(0)
        , m_BytesLeft(m_Reader ? m_Reader->size() : 0)
    {
    }

    FileInputStream::FileInputStream(FileReaderPtr&& reader)
        : m_Reader(std::move(reader))
        , m_Offset(0)
        , m_BytesLeft(m_Reader ? m_Reader->size() : 0)
    {
    }

    FileInputStream::FileInputStream(const FileReaderPtr& reader, uint64_t offset, uint64_t size)
        : m_Reader(reader)
        , m_Offset(offset)
        , m_BytesLeft(m_Reader ? size : 0)
    {
    }

    FileInputStream::FileInputStream(FileReaderPtr&& reader, uint64_t offset, uint64_t size)
        : m_Reader(std::move(reader))
        , m_Offset(offset)
        , m_BytesLeft(m_Reader ? size : 0)
    {
    }

    const std::string& FileInputStream::name() const
    {
        if (m_Reader)
            return m_Reader->name();
        static const std::string empty;
        return empty;
    }

    bool FileInputStream::atEnd() const
    {
        return m_BytesLeft == 0;
    }

    uint64_t FileInputStream::bytesAvailable() const
    {
        return m_BytesLeft;
    }

    size_t FileInputStream::read(void* buffer, size_t size)
    {
        if (!m_Reader || m_BytesLeft == 0 || size == 0)
            return 0;

        if (uint64_t(size) > m_BytesLeft)
            size = size_t(m_BytesLeft);

        if (!m_Reader->read(m_Offset, buffer, size))
        {
            m_BytesLeft = 0;
            return 0;
        }

        m_Offset += uint64_t(size);
        m_BytesLeft -= uint64_t(size);

        return size;
    }

    bool FileInputStream::skip(size_t count)
    {
        if (!m_Reader)
            return false;

        if (count == 0)
            return true;

        if (uint64_t(count) > m_BytesLeft)
        {
            m_BytesLeft = 0;
            return false;
        }

        m_Offset += uint64_t(count);
        m_BytesLeft -= uint64_t(count);

        return true;
    }
}
