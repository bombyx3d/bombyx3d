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
#include "engine/interfaces/render/lowlevel/IVertexSource.h"
#include "engine/core/macros.h"
#include "engine/render/gles2/GLES2Buffer.h"
#include "engine/render/gles2/GLES2Shader.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Engine
{
    class GLES2VertexSource : public IVertexSource
    {
    public:
        GLES2VertexSource();
        ~GLES2VertexSource();

        void setAttribute(const Atom& name, VertexAttributeType type,
            const VertexBufferPtr& buffer, size_t offset = 0, size_t stride = 0, bool normalize = false) override;

        void setAttributes(const IVertexFormatAttributeList& attributes,
            const VertexBufferPtr& buffer, size_t offset = 0) override;

        const std::shared_ptr<GLES2Buffer>& indexBuffer() const { return mIndexBuffer; }
        void setIndexBuffer(const IndexBufferPtr& indexBuffer) override;

        void bind(const GLES2Shader& shader);
        void unbind();

    private:
        struct Attribute
        {
            std::shared_ptr<GLES2Buffer> buffer;
            VertexAttributeType type;
            size_t offset;
            size_t stride;
            bool normalize;
        };

        std::unordered_map<Atom, Attribute> mAttributes;
        std::vector<int> mEnabledArrays;
        std::shared_ptr<GLES2Buffer> mIndexBuffer;

        B3D_DISABLE_COPY(GLES2VertexSource);
    };
}
