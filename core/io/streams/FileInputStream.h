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
#include "core/interfaces/io/IInputStream.h"
#include "core/interfaces/io/IFileReader.h"

namespace Engine
{
    /** An @ref IInputStream implemented as a wrapper for an instance of @ref Engine::IFileReader. */
    class FileInputStream : public IInputStream
    {
    public:
        Z_CUSTOM_IMPLEMENTATION(FileInputStream)

        /**
         * Constructor.
         * @param reader File reader.
         */
        explicit FileInputStream(IFileReader* reader);

        /**
         * Constructor.
         * @param reader File reader.
         */
        explicit FileInputStream(const Ptr<IFileReader>& reader);

        /**
         * Constructor.
         * @param reader File reader.
         */
        explicit FileInputStream(Ptr<IFileReader>&& reader);

        /**
         * Constructor.
         * @param reader File reader.
         * @param offset Starting offset in file.
         * @param limit Limit in bytes.
         */
        FileInputStream(IFileReader* reader, uint64_t offset, uint64_t limit);

        /**
         * Constructor.
         * @param reader File reader.
         * @param offset Starting offset in file.
         * @param limit Limit in bytes.
         */
        FileInputStream(const Ptr<IFileReader>& reader, uint64_t offset, uint64_t limit);

        /**
         * Constructor.
         * @param reader File reader.
         * @param offset Starting offset in file.
         * @param limit Limit in bytes.
         */
        FileInputStream(Ptr<IFileReader>&& reader, uint64_t offset, uint64_t limit);

        /** Destructor. */
        ~FileInputStream() = default;

        const std::string& name() const override;
        bool atEnd() const override;
        uint64_t bytesAvailable() const override;
        bool skip(size_t count) override;
        size_t read(void* buffer, size_t bytesToRead) override;

    private:
        Ptr<IFileReader> m_Reader;      /**< File reader. */
        uint64_t m_Offset;              /**< Current offset from the beginning of stream. */
        uint64_t m_BytesLeft;           /**< Number of bytes left in stream. */

        void* _queryCustomInterface(TypeID typeID);
    };
}
