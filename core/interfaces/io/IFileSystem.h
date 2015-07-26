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
#include "core/interfaces/IUnknown.h"
#include "core/interfaces/io/IFileReader.h"
#include <string>

namespace Engine
{
    /**
     * Base interface for filesystems.
     * @see @ref ICore::registerFileSystem.
     */
    class IFileSystem : public IUnknown
    {
    public:
        Z_INTERFACE(IFileSystem)

        /**
         * Checks whether the specified file exists.
         * @param path Path to the file.
         * @return `true` if file exists, otherwise returns `false`.
         */
        virtual bool fileExists(const std::string& path) = 0;

        /**
         * Opens an existing file.
         * @param path Path to the file.
         * @return Pointer to an instance of @ref Engine::IFileReader or `nullptr` if file does not exist.
         */
        virtual Ptr<IFileReader> openFile(const std::string& path) = 0;
    };
}
