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
#include "core/interfaces/IMemoryMappedFile.h"
#include "core/utility/macros.h"
#include <string>
#include <vector>

namespace Engine
{
    /** A "file" based on static data. */
    class StaticMemoryFile : public IMemoryMappedFile
    {
    public:
        Z_IMPLEMENTATION(StaticMemoryFile)

        /**
         * Constructor.
         * @param data Pointer to the data.
         * @param length Length of the data.
          *@param fileName File name.
          */
        StaticMemoryFile(const void* data, size_t length, const std::string& fileName = "<memory>");

        /** Destructor. */
        ~StaticMemoryFile();

        const std::string& name() const override;
        size_t rawDataSize() const override;
        const void* rawDataPointer() const override;
        uint64_t size() const override;
        bool read(uint64_t offset, void* buffer, size_t size) override;

    private:
        std::string m_Name;         /**< File name. */
        const char* m_Data;         /**< Pointer to the data. */
        size_t m_Length;            /**< Size of the data. */
    };
}