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
#include "core/interfaces/IFileReader.h"
#include <string>
#include <mutex>
#include <memory>
#include <QFile>

namespace Engine
{
    /** A Qt-based implementation of @ref IFileReader. */
    class QtFileReader : public IFileReader
    {
    public:
        Z_IMPLEMENTATION(QtFileReader)

        /**
         * Constructor.
         * @param fileName File name.
         * @param file Pointer to the corresponding `QFile`.
         */
        QtFileReader(const std::string& fileName, std::unique_ptr<QFile>&& file);

        /** Destructor. */
        ~QtFileReader();

        const std::string& name() const override;
        uint64_t size() const override;
        bool read(uint64_t offset, void* buffer, size_t bytesToRead) override;

    private:
        std::mutex m_Mutex;
        std::string m_Name;
        std::unique_ptr<QFile> m_File;
        uint64_t m_Size;
        uint64_t m_Offset;
    };
}
