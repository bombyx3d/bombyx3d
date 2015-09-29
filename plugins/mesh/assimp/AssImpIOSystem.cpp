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
#include "AssImpIOSystem.h"
#include "AssImpIOStream.h"
#include "engine/core/Services.h"
#include <cassert>
#include <cstring>

namespace B3D
{
    AssImpIOSystem::AssImpIOSystem(IFile* primaryFile)
        : mPrimaryFile(primaryFile)
    {
    }

    char AssImpIOSystem::getOsSeparator() const
    {
        return '/';
    }

    bool AssImpIOSystem::Exists(const char* fileName) const
    {
        return Services::fileSystem()->fileExists(fileName);
    }

    Assimp::IOStream* AssImpIOSystem::Open(const char* fileName, const char* mode)
    {
        if (!fileName || !mode || (strcmp(mode, "r") != 0 && strcmp(mode, "rb") != 0)) {
            assert(false);
            return nullptr;
        }

        if (mPrimaryFile->name() == fileName)
            return new AssImpIOStream(mPrimaryFile);

        if (!Services::fileSystem()->fileExists(fileName))
            return nullptr;

        FilePtr file = Services::fileSystem()->openFile(fileName);
        if (!file)
            return nullptr;

        return new AssImpIOStream(std::move(file));
    }

    void AssImpIOSystem::Close(Assimp::IOStream* file)
    {
        delete file;
    }
}
