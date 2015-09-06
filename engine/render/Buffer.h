/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
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
#include "engine/interfaces/render/IIndexBuffer.h"
#include "engine/interfaces/render/IVertexBuffer.h"
#include "engine/core/macros.h"

namespace Engine
{
    class Buffer : public IVertexBuffer, public IIndexBuffer
    {
    public:
        explicit Buffer(size_t target);
        ~Buffer();

        size_t handle() const { return mHandle; }

        size_t currentSize() const override;

        void initEmpty(size_t size, BufferUsage usage) override;
        void setData(const void* data, size_t size, BufferUsage usage) override;

    private:
        size_t mHandle;
        size_t mSize;
        size_t mTarget;
        BufferUsage mUsage;

        Z_DISABLE_COPY(Buffer);
    };
}
