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
#include "AssImpIOStream.h"
#include <cassert>

namespace Engine
{
    AssImpIOStream::AssImpIOStream(IFile* file)
        : mFile(file)
    {
    }

    AssImpIOStream::AssImpIOStream(const FilePtr& file)
        : mFile(file.get())
        , mFilePointer(file)
    {
    }

    AssImpIOStream::~AssImpIOStream()
    {
        if (!mFilePointer)
            delete mFile;
    }

    size_t AssImpIOStream::FileSize() const
    {
        if (!mFile)
            return 0;
        return size_t(mFile->size());
    }

    aiReturn AssImpIOStream::Seek(size_t offset, aiOrigin origin)
    {
        if (!mFile)
            return aiReturn_FAILURE;

        uint64_t newPosition = 0;
        switch (origin)
        {
        case aiOrigin_SET: newPosition = uint64_t(offset); break;
        case aiOrigin_CUR: newPosition = mFile->position() + uint64_t(offset); break;
        case aiOrigin_END: newPosition = mFile->size() - uint64_t(offset); break;
        default: assert(false); return aiReturn_FAILURE;
        }

        return (mFile->seek(newPosition) ? aiReturn_SUCCESS : aiReturn_FAILURE);
    }

    size_t AssImpIOStream::Tell() const
    {
        if (!mFile)
            return 0;
        return size_t(mFile->position());
    }

    size_t AssImpIOStream::Read(void* buffer, size_t size, size_t count)
    {
        if (!mFile)
            return 0;
        return mFile->read(buffer, size * count) / size;
    }

    size_t AssImpIOStream::Write(const void*, size_t, size_t)
    {
        assert(false);
        return 0;
    }

    void AssImpIOStream::Flush()
    {
        assert(false);
    }
}
