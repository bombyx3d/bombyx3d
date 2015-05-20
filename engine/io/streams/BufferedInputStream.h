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
#include "InputStream.h"
#include <memory>

namespace Z
{
    class BufferedInputStream : public InputStream
    {
    public:
        static const size_t DEFAULT_BUFFER_SIZE = 65536;

        BufferedInputStream(const InputStreamPtr& stream);
        BufferedInputStream(InputStreamPtr&& stream);
        BufferedInputStream(const InputStreamPtr& stream, size_t bufferSize);
        BufferedInputStream(InputStreamPtr&& stream, size_t bufferSize);
        ~BufferedInputStream() = default;

        const std::string& name() const override;

        bool atEnd() const override;
        uint64_t bytesAvailable() const override;

        size_t read(void* buffer, size_t size) override;
        bool skip(size_t count) override;

    private:
        InputStreamPtr m_Stream;
        std::unique_ptr<uint8_t[]> m_Buffer;
        size_t m_BufferSize;
        size_t m_BufferStart = 0;
        size_t m_BufferEnd = 0;

        BufferedInputStream(const BufferedInputStream&) = delete;
        BufferedInputStream& operator=(const BufferedInputStream&) = delete;
    };
}
