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
#include "engine/interfaces/mesh/IMesh.h"
#include "engine/interfaces/mesh/IRawMeshData.h"
#include "engine/interfaces/render/ICanvas.h"
#include "engine/core/macros.h"
#include <vector>

namespace B3D
{
    class Mesh : public IMesh
    {
    public:
        Mesh();
        ~Mesh();

        const BoundingBox& boundingBox() const override { return mBoundingBox; }

        void setData(const RawMeshDataPtr& data, BufferUsage usage, bool async = true);

        void render(ICanvas* canvas) const override;

    private:
        struct Element
        {
            PrimitiveType primitiveType;
            MaterialPtr material;
            VertexSourcePtr vertexSource;
            size_t firstIndex;
            size_t indexCount;
        };

        BoundingBox mBoundingBox;
        VertexBufferPtr mVertexBuffer;
        IndexBufferPtr mIndexBuffer;
        std::vector<Element> mElements;

        B3D_DISABLE_COPY(Mesh);
    };
}
