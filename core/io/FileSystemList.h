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
#include "core/utility/Ptr.h"
#include "core/interfaces/IFileSystem.h"
#include <mutex>
#include <vector>
#include <memory>

namespace Engine
{
    /**
     * A list of filesystems.
     * @see @ref Engine::ICore::registerFileSystem.
     */
    class FileSystemList : public IFileSystem
    {
    public:
        Z_IMPLEMENTATION(FileSystemList)

        /** Constructor. */
        FileSystemList();

        /** Destructor. */
        ~FileSystemList();

        /**
         * Adds filesystem to the list.
         * @param fileSystem Pointer to the filesystem to add. This list will take ownership of the filesystem
         * and will delete it when appropriate.
         */
        void add(IFileSystem* fileSystem);

        /**
         * Adds filesystem to the list.
         * @param fileSystem Pointer to the filesystem to add.
         */
        void add(const Ptr<IFileSystem>& fileSystem);

        /**
         * Adds filesystem to the list.
         * @param fileSystem Pointer to the filesystem to add.
         */
        void add(Ptr<IFileSystem>&& fileSystem);

        bool fileExists(const std::string& path) override;
        Ptr<IFileReader> openFile(const std::string& path) override;

    private:
        using Array = std::vector<Ptr<IFileSystem>>;
        using ArrayPtr = std::shared_ptr<Array>;

        std::mutex m_Mutex;
        ArrayPtr m_CachedFileSystems;
        Array m_FileSystems;

        ArrayPtr cachedFileSystems();
        void invalidateCache();
    };
}
