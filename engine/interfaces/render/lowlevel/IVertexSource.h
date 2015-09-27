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
#include "engine/interfaces/render/lowlevel/IVertexBuffer.h"
#include "engine/interfaces/render/lowlevel/IIndexBuffer.h"
#include "engine/core/Atom.h"
#include <memory>

namespace Engine
{
    enum class VertexAttributeType
    {
        Float,
        Float2,
        Float3,
        Float4,
    };

    class IVertexFormatAttributeList;

    class IVertexSource
    {
    public:
        virtual ~IVertexSource() = default;

        virtual void setAttribute(const Atom& name, VertexAttributeType type,
            const VertexBufferPtr& buffer, size_t offset = 0, size_t stride = 0, bool normalize = false) = 0;
        virtual void setAttributes(const IVertexFormatAttributeList& attributes,
            const VertexBufferPtr& buffer, size_t offset = 0) = 0;

        template <typename T> void setAttributes(const VertexBufferPtr& buffer, size_t offset)
            { setAttributes(buffer, T::attributes(), offset); }

        virtual void setIndexBuffer(const IndexBufferPtr& indexBuffer) = 0;
    };

    using VertexSourcePtr = std::shared_ptr<IVertexSource>;
}
