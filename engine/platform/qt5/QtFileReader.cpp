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
#include "QtFileReader.h"
#include "QtMacros.h"
#include "utility/debug.h"
#include <cerrno>
#include <cstring>

namespace Z
{
    QtFileReader::QtFileReader(const std::string& name, std::unique_ptr<QFile>&& file)
        : m_Name(name)
        , m_File(std::move(file))
        , m_Offset(0)
    {
        qint64 size = m_File->size();
        m_Size = (size >= 0 ? uint64_t(size) : 0);
    }

    QtFileReader::~QtFileReader()
    {
        m_File->close();
    }

    const std::string& QtFileReader::name() const
    {
        return m_Name;
    }

    uint64_t QtFileReader::size() const
    {
        return m_Size;
    }

    bool QtFileReader::read(uint64_t offset, void* buffer, size_t size)
    {
        std::lock_guard<decltype(m_Mutex)> guard(m_Mutex);

        if (offset != m_Offset)
        {
            if (!m_File->seek(qint64(offset)))
            {
                Z_LOG("Seek failed in file \"" << m_Name << "\": " << zqUtf8Printable(m_File->errorString()));
                return false;
            }
            m_Offset = offset;
        }

        qint64 bytesRead = m_File->read(reinterpret_cast<char*>(buffer), qint64(size));
        if (bytesRead < 0)
        {
            Z_LOG("Error reading file \"" << m_Name << "\": " << zqUtf8Printable(m_File->errorString()));
            return false;
        }

        m_Offset += uint64_t(bytesRead);

        if (size_t(bytesRead) != size)
        {
            Z_LOG("Incomplete read in file \"" << m_Name << "\".");
            return false;
        }

        return true;
    }
}
