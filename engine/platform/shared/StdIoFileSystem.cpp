#include "StdIoFileSystem.h"
#include "StdIoFile.h"
#include "engine/core/Log.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

namespace Engine
{
    StdIoFileSystem::StdIoFileSystem(const std::string& basePath)
        : mBasePath(basePath)
    {
    }

    StdIoFileSystem::~StdIoFileSystem()
    {
    }

    bool StdIoFileSystem::fileExists(const std::string& name)
    {
        std::string path = mBasePath + '/' + name;

        FILE* file = fopen(path.c_str(), "rb");
        if (file) {
            fclose(file);
            return true;
        }

        return false;
    }

    FilePtr StdIoFileSystem::openFile(const std::string& name)
    {
        std::string path = mBasePath + '/' + name;

        FILE* file = fopen(path.c_str(), "rb");
        if (!file) {
            const char* error = strerror(errno);
            Z_LOGE("Unable to open file \"" << path << "\": " << error);
            return nullptr;
        }

        return std::make_shared<StdIoFile>(file, name);
    }
}
