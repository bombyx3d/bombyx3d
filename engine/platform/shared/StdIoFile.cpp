/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
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
#include "StdIoFile.h"
#include "engine/core/Log.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <algorithm>

namespace B3D
{
    StdIoFile::StdIoFile(FILE* handle, const std::string& path)
        : mHandle(handle)
        , mPath(path)
    {
    }

    StdIoFile::~StdIoFile()
    {
        fclose(mHandle);
    }

    const std::string& StdIoFile::name() const
    {
        return mPath;
    }

    uint64_t StdIoFile::size()
    {
        uint64_t savedPosition = position();

        uint64_t size;
        if (fseek(mHandle, 0, SEEK_END) == 0)
            size = position();
        else {
            const char* error = strerror(errno);
            B3D_LOGE("Seek failed in file \"" << mPath << "\": " << error);
            size = 0;
        }

        seek(savedPosition);

        return size;
    }

    uint64_t StdIoFile::position()
    {
        long pos = ftell(mHandle);
        if (pos < 0) {
            const char* error = strerror(errno);
            B3D_LOGE("Unable to determine current position in file \"" << mPath << "\": " << error);
            return 0;
        }
        return uint64_t(pos);
    }

    bool StdIoFile::seek(uint64_t pos)
    {
        if (fseek(mHandle, long(pos), SEEK_SET) < 0) {
            const char* error = strerror(errno);
            B3D_LOGE("Seek failed in file \"" << mPath << "\": " << error);
            return false;
        }
        return true;
    }

    size_t StdIoFile::read(void* buffer, size_t bytes)
    {
        size_t bytesRead = fread(buffer, 1, bytes, mHandle);
        if (ferror(mHandle)) {
            const char* error = strerror(errno);
            B3D_LOGE("Error reading file \"" << mPath << "\": " << error);
        }
        return bytesRead;
    }
}
