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
#include "ZipFileReader.h"
#include "utility/debug.h"
#include <unzip.h>
#include <vector>

namespace Z
{
    ZipFileReader::ZipFileReader(const std::string& name, const FileReaderPtr& reader, void* handle)
        : m_Name(name)
        , m_Handle(handle)
        , m_ZipReader(reader)
        , m_Size(0)
        , m_Offset(0)
        , m_IsOpen(false)
    {
        Z_ASSERT(m_Handle != nullptr);
        Z_ASSERT(m_ZipReader != nullptr);
        reopen();
    }

    ZipFileReader::~ZipFileReader()
    {
        unzClose(m_Handle);
    }

    const std::string& ZipFileReader::name() const
    {
        return m_Name;
    }

    uint64_t ZipFileReader::size() const
    {
        return m_Size;
    }

    bool ZipFileReader::read(uint64_t offset, void* buffer, size_t size)
    {
        std::lock_guard<decltype(m_Mutex)> guard(m_Mutex);

        if (!m_IsOpen) {
            Z_LOG("Error reading file \"" << m_Name << "\".");
            return false;
        }

        if (offset != m_Offset)
        {
            if (offset < m_Offset)
            {
                // Backwards seek
                if (!reopen())
                    return false;
            }

            if (m_Offset < offset)
            {
                const size_t MAX_BUFFER_SIZE = 65536;

                size_t bufferSize;
                if (m_Offset - offset >= MAX_BUFFER_SIZE)
                    bufferSize = MAX_BUFFER_SIZE;
                else
                    bufferSize = size_t(m_Offset - offset);

                std::vector<char> buffer(bufferSize);
                while (m_Offset < offset) {
                    size_t bytesToRead;
                    if (m_Offset - offset >= buffer.size())
                        bytesToRead = buffer.size();
                    else
                        bytesToRead = size_t(m_Offset - offset);

                    int r = unzReadCurrentFile(m_Handle, buffer.data(), bytesToRead);
                    if (r <= 0) {
                        Z_LOG("Error reading file \"" << m_Name << "\".");
                        return false;
                    }
                    m_Offset += r;
                }
            }

            Z_ASSERT(m_Offset == offset);
        }

        int r = unzReadCurrentFile(m_Handle, buffer, size);
        if (r <= 0) {
            Z_LOG("Error reading file \"" << m_Name << "\".");
            return false;
        }

        m_Offset += r;
        if (size_t(r) != size)
        {
            Z_LOG("Incomplete read in file \"" << m_Name << "\".");
            return false;
        }

        return true;
    }

    bool ZipFileReader::reopen()
    {
        if (m_IsOpen) {
            int r = unzCloseCurrentFile(m_Handle);
            Z_ASSERT(r == UNZ_OK);
            m_Size = 0;
            m_IsOpen = false;
        }

        int method = 0;
        int level = 0;
        int r = unzOpenCurrentFile3(m_Handle, &method, &level, 0, nullptr);
        if (r != UNZ_OK) {
            Z_LOG("Unable to open file \"" << m_Name << "\" in ZIP archive \"" << m_ZipReader->name() << "\".");
            return false;
        }

        unz_file_info64 fileInfo;
        r = unzGetCurrentFileInfo64(m_Handle, &fileInfo, NULL, 0, NULL, 0, NULL, 0);
        if (r != UNZ_OK) {
            unzCloseCurrentFile(m_Handle);
            Z_LOG("Unable to open file \"" << m_Name << "\" in ZIP archive \"" << m_ZipReader->name() << "\".");
            return false;
        }

        m_IsOpen = true;
        m_Offset = 0;
        m_Size = fileInfo.uncompressed_size;

        return true;
    }
}
