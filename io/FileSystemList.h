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
#include "FileSystem.h"
#include <mutex>
#include <vector>
#include <memory>

namespace Z
{
    using namespace Engine;

    class FileSystemList : public FileSystem
    {
    public:
        FileSystemList();
        virtual ~FileSystemList();

        bool fileExists(const std::string& path) override;
        Ptr<IFileReader> openFile(const std::string& path) override;

        void add(FileSystem* fileSystem);
        void add(const FileSystemPtr& fileSystem);
        void add(FileSystemPtr&& fileSystem);

    private:
        using Array = std::vector<FileSystemPtr>;

        std::mutex m_Mutex;
        std::shared_ptr<Array> m_CachedFileSystems;
        Array m_FileSystems;

        std::shared_ptr<Array> cachedFileSystems();
        void invalidateCache();
    };

    using FileSystemListPtr = std::shared_ptr<FileSystemList>;
}
