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
#include "core/utility/macros.h"
#include "core/interfaces/IFileSystem.h"
#include "core/interfaces/IFileReader.h"
#include <mutex>
#include <string>

/**
 * @addtogroup Modules
 * @{
 * @addtogroup FileSystem
 * @{
 * @addtogroup Zip
 * @{
 */

namespace Engine
{
     /** A read-only ZIP-file based implementation of @ref Engine::IFileSystem. */
    class ZipFileSystem : public IFileSystem
    {
    public:
        Z_IMPLEMENTATION(ZipFileSystem)

        /**
         * Constructor.
         * @param zipFile Pointer to the ZIP file.
         */
        explicit ZipFileSystem(const Ptr<IFileReader>& zipFile);

        /**
         * Constructor.
         * @param zipFile Pointer to the ZIP file.
         */
        explicit ZipFileSystem(Ptr<IFileReader>&& zipFile);

        /** Destructor. */
        ~ZipFileSystem();

        bool fileExists(const std::string& path) final override;
        Ptr<IFileReader> openFile(const std::string& path) final override;

    private:
        std::mutex m_Mutex;
        Ptr<IFileReader> m_ZipReader;
        void* m_ZipFile;
    };
}

/**
 * @}
 * @}
 * @}
 */