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

#pragma once
#include "../BuildManager.h"
#include <zip.h>
#include <string>

class ZipReader;
struct ZipExtraField;

class ZipWriter
{
public:
    enum CompressionMethod
    {
        UNCOMPRESSED,
        DEFLATE,
        BZIP2,
    };

    explicit ZipWriter(BuildManager::Notifications* notifications);
    ~ZipWriter();

    const std::string& fileName() const { return m_FileName; }

    bool open(const std::string& fileName);
    void close();

    void createFile(const std::string& entryName, uint64_t dosTime = 0, const tm_zip* zipTime = nullptr,
        const ZipExtraField* extraField = nullptr, CompressionMethod compressionMethod = DEFLATE,
        int compressionLevel = 5);
    void writeFile(const void* data, size_t size);
    void closeFile();

    bool addCurrentFileFromZip(ZipReader* reader);

private:
    BuildManager::Notifications* m_Notifications;
    std::string m_FileName;
    std::string m_EntryName;
    void* m_ZipFile = nullptr;

    ZipWriter(const ZipWriter&) = delete;
    ZipWriter& operator=(const ZipWriter&) = delete;

    friend class ZipReader;
};
