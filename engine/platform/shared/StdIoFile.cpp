#include "StdIoFile.h"
#include "engine/core/Log.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <algorithm>

namespace Engine
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
            Z_LOGE("Seek failed in file \"" << mPath << "\": " << error);
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
            Z_LOGE("Unable to determine current position in file \"" << mPath << "\": " << error);
            return 0;
        }
        return uint64_t(pos);
    }

    bool StdIoFile::seek(uint64_t pos)
    {
        if (fseek(mHandle, long(pos), SEEK_SET) < 0) {
            const char* error = strerror(errno);
            Z_LOGE("Seek failed in file \"" << mPath << "\": " << error);
            return false;
        }
        return true;
    }

    size_t StdIoFile::read(void* buffer, size_t bytes)
    {
        size_t bytesRead = fread(buffer, 1, bytes, mHandle);
        if (ferror(mHandle)) {
            const char* error = strerror(errno);
            Z_LOGE("Error reading file \"" << mPath << "\": " << error);
        }
        return bytesRead;
    }
}
