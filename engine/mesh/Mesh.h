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
#include "engine/interfaces/mesh/IMeshData.h"
#include "engine/interfaces/mesh/IMeshElement.h"
#include "engine/core/macros.h"
#include <vector>
#include <memory>
#include <cstdint>

namespace Engine
{
    class Mesh : public IMesh, public std::enable_shared_from_this<Mesh>
    {
    public:
        Mesh();
        ~Mesh();

        const BoundingBox& boundingBox() const override { return mBoundingBox; }

        const VertexBufferPtr& vertexBuffer() const override { return mVertexBuffer; }
        const IndexBufferPtr& indexBuffer() const override { return mIndexBuffer; }

        const std::vector<MeshElementPtr>& elements() const override { return mElements; }

        void setData(const MeshDataPtr& data, BufferUsage usage);

    private:
        VertexBufferPtr mVertexBuffer;
        IndexBufferPtr mIndexBuffer;
        BoundingBox mBoundingBox;
        std::vector<MeshElementPtr> mElements;

        Z_DISABLE_COPY(Mesh);
    };
}
