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
#include "BufferedInputStream.h"
#include "utility/debug.h"
#include <cstring>
#include <algorithm>

namespace Z
{
    BufferedInputStream::BufferedInputStream(const InputStreamPtr& stream)
        : m_Stream(stream)
        , m_BufferSize(DEFAULT_BUFFER_SIZE)
    {
        if (m_Stream)
            m_Buffer.reset(new uint8_t[m_BufferSize]);
    }

    BufferedInputStream::BufferedInputStream(InputStreamPtr&& stream)
        : m_Stream(std::move(stream))
        , m_BufferSize(DEFAULT_BUFFER_SIZE)
    {
        if (m_Stream)
            m_Buffer.reset(new uint8_t[m_BufferSize]);
    }

    BufferedInputStream::BufferedInputStream(const InputStreamPtr& stream, size_t bufferSize)
        : m_Stream(stream)
        , m_BufferSize(bufferSize)
    {
        Z_CHECK(bufferSize > 0);
        m_BufferSize = std::max<size_t>(m_BufferSize, 1);

        if (m_Stream)
            m_Buffer.reset(new uint8_t[m_BufferSize]);
    }

    BufferedInputStream::BufferedInputStream(InputStreamPtr&& stream, size_t bufferSize)
        : m_Stream(std::move(stream)),
          m_BufferSize(bufferSize)
    {
        Z_CHECK(bufferSize > 0);
        m_BufferSize = std::max<size_t>(m_BufferSize, 1);

        if (m_Stream)
            m_Buffer.reset(new uint8_t[m_BufferSize]);
    }

    const std::string& BufferedInputStream::name() const
    {
        if (m_Stream)
            return m_Stream->name();
        static const std::string empty;
        return empty;
    }

    FileReader* BufferedInputStream::associatedFile() const
    {
        if (m_Stream)
            return m_Stream->associatedFile();
        return nullptr;
    }

    bool BufferedInputStream::atEnd() const
    {
        if (!m_Stream)
            return true;
        return (m_BufferStart == m_BufferEnd && m_Stream->atEnd());
    }

    uint64_t BufferedInputStream::bytesAvailable() const
    {
        return (m_BufferEnd - m_BufferStart) + m_Stream->bytesAvailable();
    }

    size_t BufferedInputStream::read(void* buffer, size_t size)
    {
        if (!m_Stream || size == 0)
            return 0;

        size_t bytesBuffered = m_BufferEnd - m_BufferStart;
        if (bytesBuffered >= size)
        {
            memcpy(buffer, m_Buffer.get() + m_BufferStart, size);
            m_BufferStart += size;
            return size;
        }

        size_t bytesLeft = size;
        uint8_t* p = reinterpret_cast<uint8_t*>(buffer);
        if (bytesBuffered > 0)
        {
            memcpy(p, m_Buffer.get() + m_BufferStart, bytesBuffered);
            m_BufferStart = m_BufferEnd;
            p += bytesBuffered;
            bytesLeft -= bytesBuffered;
        }

        if (bytesLeft > 0)
        {
            if (bytesLeft >= m_BufferSize)
            {
                size_t bytesRead = m_Stream->read(p, bytesLeft);
                p += bytesRead;
                bytesLeft -= bytesRead;
            }
            else
            {
                size_t bytesRead = m_Stream->read(m_Buffer.get(), m_BufferSize);
                m_BufferStart = 0;
                m_BufferEnd = bytesRead;

                size_t bytesAvailable = std::min(bytesRead, bytesLeft);
                memcpy(p, m_Buffer.get(), bytesAvailable);
                p += bytesAvailable;
                m_BufferStart += bytesAvailable;
                bytesLeft -= bytesAvailable;
            }
        }

        return size - bytesLeft;
    }

    bool BufferedInputStream::skip(size_t count)
    {
        if (!m_Stream)
            return false;

        if (count == 0)
            return true;

        size_t bytesBuffered = m_BufferEnd - m_BufferStart;
        if (bytesBuffered >= count)
        {
            m_BufferStart += count;
            return true;
        }

        m_BufferStart = m_BufferEnd;
        return m_Stream->skip(count - bytesBuffered);
    }
}
