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
#include "core/interfaces/IUnknown.h"
#include <string>
#include <cstdint>

namespace Engine
{
    /** Base interface for file readers. */
    class IFileReader : public virtual IUnknown
    {
        Z_INTERFACE(IFileReader)

        /**
         * Retrieves name of file.
         * @return Name of file.
         */
        virtual const std::string& name() const = 0;

        /**
         * Retrieves size of file.
         * @return Size of file in bytes.
         */
        virtual uint64_t size() const = 0;

        /**
         * Reads data from file.
         * @param offset Offset to read data from.
         * @param buffer Pointer to the buffer where data should be read.
         * @param bytesToRead Number of bytes to read.
         * @return `true` on success, otherwise returns `false`.
         */
        virtual bool read(uint64_t offset, void* buffer, size_t bytesToRead) = 0;
    };
}
