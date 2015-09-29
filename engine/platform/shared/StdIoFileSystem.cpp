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
            B3D_LOGE("Unable to open file \"" << path << "\": " << error);
            return nullptr;
        }

        return std::make_shared<StdIoFile>(file, name);
    }
}
