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
#include "core/interfaces/IStream.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace Engine
{
    /** Base interface for input streams. */
    class IInputStream : public IStream
    {
        Z_INTERFACE(IInputStream)

        /**
         * Checks whether end of stream has been reached.
         * @return `true` if end of stream has been reached, otherwise returns `false`.
         */
        virtual bool atEnd() const = 0;

        /**
         * Returns number of bytes available for reading from stream without blocking.
         * Returned value may be less than total number of bytes in the stream.
         * @return number of bytes available for reading from stream without blocking.
         */
        virtual uint64_t bytesAvailable() const = 0;

        /**
         * Reads the specified number of bytes from stream.
         * @note This method may read less data than requested even if there is more data available in the stream.
         * Use the @ref atEnd() method to ensure that end of stream has been reached.
         * @note This method may block for unspecified amount of time if requested `size` is greater than
         * a value returned by @ref bytesAvailable().
         * @param buffer Pointer to the buffer where data should be read.
         * @param bytesToRead Number of bytes to read.
         * @return Number of bytes actually read.
         */
        virtual size_t read(void* buffer, size_t bytesToRead) = 0;

        /**
         * Reads single line of text from stream.
         * This method interpretes `LF` and `CRLF` as line separators.
         * @param includeEolMarker Set to `true` to include '\n' into the read string.
         * @return Read string.
         */
        virtual std::string readLine(bool includeEolMarker);

        /**
         * Reads all remaining bytes from stream.
         * @return Vector of bytes.
         */
        virtual std::vector<char> readAll();

        /**
         * Skips the specified amount of bytes.
         * @note This method may block for unspecified amount of time if requested `count` is greater than
         * a value returned by @ref bytesAvailable().
         * @param bytesToSkip Number of bytes to skip.
         * @return `true` if the specified amount of bytes have been skipped, otherwise returns `false`.
         */
        virtual bool skip(size_t bytesToSkip) = 0;
    };
}
