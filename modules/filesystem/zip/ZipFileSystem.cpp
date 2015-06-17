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
#include "ZipFileSystem.h"
#include "private/ZipFileReader.h"
#include "core/utility/debug.h"
#include <unzip.h>

namespace Engine
{
    namespace
    {
        struct Stream
        {
            uint64_t offset;
            uint64_t size;
            IFileReader* reader;
            int error;
        };

        static voidpf ZCALLBACK ioOpen(voidpf opaque, const void* filename, int mode)
        {
            IFileReader* reader = reinterpret_cast<IFileReader*>(opaque);
            if (!reader)
                return nullptr;

            if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) != ZLIB_FILEFUNC_MODE_READ)
                return nullptr;
            if (mode & ZLIB_FILEFUNC_MODE_CREATE)
                return nullptr;

            if (!filename || reader->name() != reinterpret_cast<const char*>(filename))
                return nullptr;

            Stream* stream = new Stream;
            stream->offset = 0;
            stream->size = reader->size();
            stream->reader = reader;
            stream->error = 0;

            return stream;
        }

        static voidpf ZCALLBACK ioOpenDisk(voidpf, voidpf, int, int)
        {
            return nullptr;
        }

        static int ZCALLBACK ioClose(voidpf, voidpf s)
        {
            Stream* stream = reinterpret_cast<Stream*>(s);
            if (!stream)
                return -1;

            delete stream;

            return 0;
        }

        static uLong ZCALLBACK ioRead(voidpf, voidpf s, void* buf, uLong size)
        {
            Stream* stream = reinterpret_cast<Stream*>(s);
            if (!stream)
                return 0;

            if (stream->offset >= stream->size)
                return 0;
            if (stream->offset + size > stream->size)
                size = uLong(stream->size - stream->offset);

            if (!stream->reader->read(stream->offset, buf, size)) {
                stream->error = 1;
                return 0;
            }

            stream->offset += size;

            return size;
        }

        static uLong ZCALLBACK ioWrite(voidpf, voidpf s, const void*, uLong)
        {
            Stream* stream = reinterpret_cast<Stream*>(s);
            if (stream)
                stream->error = 1;
            return 0;
        }

        static ZPOS64_T ZCALLBACK ioTell(voidpf, voidpf s)
        {
            Stream* stream = reinterpret_cast<Stream*>(s);
            if (!stream)
                return static_cast<ZPOS64_T>(-1);
            return static_cast<ZPOS64_T>(stream->offset);
        }

        static long ZCALLBACK ioSeek(voidpf, voidpf s, ZPOS64_T offset, int origin)
        {
            Stream* stream = reinterpret_cast<Stream*>(s);
            if (!stream)
                return -1;

            int64_t newOffset = 0;
            switch (origin)
            {
            case ZLIB_FILEFUNC_SEEK_CUR:
                newOffset = int64_t(stream->offset) + int64_t(offset);
                break;
            case ZLIB_FILEFUNC_SEEK_END:
                newOffset = int64_t(stream->size) + int64_t(offset);
                break;
            case ZLIB_FILEFUNC_SEEK_SET:
                newOffset = int64_t(offset);
                break;
            default:
                return -1;
            }

            if (newOffset < 0)
                return -1;

            stream->offset = uint64_t(newOffset);

            return 0;
        }

        static int ZCALLBACK ioTestError(voidpf, voidpf s)
        {
            Stream* stream = reinterpret_cast<Stream*>(s);
            if (!stream)
                return -1;
            return stream->error;
        }

        static unzFile openZip(const Ptr<IFileReader>& reader, const unz64_file_pos* filepos = nullptr)
        {
            if (!reader)
                return nullptr;

            zlib_filefunc64_def_s io;
            io.zopen64_file = ioOpen;
            io.zopendisk64_file = ioOpenDisk;
            io.zread_file = ioRead;
            io.zwrite_file = ioWrite;
            io.ztell64_file = ioTell;
            io.zseek64_file = ioSeek;
            io.zclose_file = ioClose;
            io.zerror_file = ioTestError;
            io.opaque = reader.get();

            return unzOpen3_64(reader->name().c_str(), &io, filepos);
        }
    }

    ZipFileSystem::ZipFileSystem(const Ptr<IFileReader>& zipFile)
        : m_ZipReader(zipFile)
    {
        m_ZipFile = openZip(m_ZipReader);
        if (!m_ZipFile)
            Z_LOG("Unable to open ZIP archive \"" << (m_ZipReader ? m_ZipReader->name() : std::string("(null)")) << "\".");
    }

    ZipFileSystem::ZipFileSystem(Ptr<IFileReader>&& zipFile)
        : m_ZipReader(std::move(zipFile))
    {
        m_ZipFile = openZip(m_ZipReader);
        if (!m_ZipFile)
            Z_LOG("Unable to open ZIP archive \"" << (m_ZipReader ? m_ZipReader->name() : std::string("(null)")) << "\".");
    }

    ZipFileSystem::~ZipFileSystem()
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        if (m_ZipFile)
            unzClose(m_ZipFile);
    }

    bool ZipFileSystem::fileExists(const std::string& path)
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);

        if (!m_ZipFile)
            return false;

        int r = unzLocateFile(m_ZipFile, path.c_str(), nullptr);
        return (r == UNZ_OK);
    }

    Ptr<IFileReader> ZipFileSystem::openFile(const std::string& path)
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);

        if (!m_ZipFile) {
            Z_LOG("ZIP archive is not open.");
            return Ptr<IFileReader>();
        }

        int r = unzLocateFile(m_ZipFile, path.c_str(), nullptr);
        if (r != UNZ_OK) {
            Z_LOG("Unable to find file \"" << path << "\" in ZIP archive \"" << m_ZipReader->name() << "\".");
            return Ptr<IFileReader>();
        }

        unz64_file_pos pos;
        r = unzGetFilePos64(m_ZipFile, &pos);
        if (r != UNZ_OK) {
            Z_LOG("Unable to seek to file \"" << path << "\" in ZIP archive \"" << m_ZipReader->name() << "\".");
            return Ptr<IFileReader>();
        }

        unzFile file = openZip(m_ZipReader, &pos);
        if (file == nullptr) {
            Z_LOG("Unable to open file \"" << path << "\" in ZIP archive \"" << m_ZipReader->name() << "\".");
            return Ptr<IFileReader>();
        }

        return std::make_shared<ZipFileReader>(path, m_ZipReader, file);
    }
}
