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
#include "io/files/StaticMemoryFile.h"
#include <string>

namespace Z
{
    class StaticMemoryInputStream : public InputStream, public StaticMemoryFile
    {
    public:
        StaticMemoryInputStream(const void* data, size_t size, const std::string& name = "<memory>");
        ~StaticMemoryInputStream() = default;

        const std::string& name() const override;
        FileReader* associatedFile() const override;

        bool atEnd() const override;
        uint64_t bytesAvailable() const override;

        size_t read(void* buffer, size_t size) override;
        bool skip(size_t count) override;

    private:
        size_t m_Offset;
        size_t m_BytesLeft;
    };

    using StaticMemoryInputStreamPtr = std::shared_ptr<StaticMemoryInputStream>;
}
