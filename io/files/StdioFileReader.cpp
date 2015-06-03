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
#include "StdioFileReader.h"
#include "utility/debug.h"
#include <cerrno>
#include <cstring>

namespace Z
{
    StdioFileReader::StdioFileReader(const std::string& name, FILE* handle)
        : m_Name(name)
        , m_Handle(handle)
        , m_Offset(0)
    {
        fseek(m_Handle, 0, SEEK_END);
        long size = ftell(m_Handle);
        rewind(m_Handle);

        m_Size = (size >= 0 ? uint64_t(size) : 0);
    }

    StdioFileReader::~StdioFileReader()
    {
        fclose(m_Handle);
    }

    const std::string& StdioFileReader::name() const
    {
        return m_Name;
    }

    uint64_t StdioFileReader::size() const
    {
        return m_Size;
    }

    bool StdioFileReader::read(uint64_t offset, void* buffer, size_t size)
    {
        std::lock_guard<decltype(m_Mutex)> guard(m_Mutex);

        if (offset != m_Offset)
        {
            if (fseek(m_Handle, long(offset), SEEK_SET) != 0)
            {
                int err = errno;
                Z_LOG("Seek failed in file \"" << m_Name << "\": " << strerror(err));
                return false;
            }
            m_Offset = offset;
        }

        size_t bytesRead = fread(buffer, 1, size, m_Handle);
        if (ferror(m_Handle))
        {
            int err = errno;
            Z_LOG("Error reading file \"" << m_Name << "\": " << strerror(err));
            return false;
        }

        m_Offset += bytesRead;

        if (bytesRead != size)
        {
            Z_LOG("Incomplete read in file \"" << m_Name << "\".");
            return false;
        }

        return true;
    }
}
