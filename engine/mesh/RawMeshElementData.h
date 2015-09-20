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
#include "engine/interfaces/mesh/IRawMeshElementData.h"
#include "engine/mesh/RawMeshData.h"

namespace Engine
{
    template <class VERTEX> class RawMeshElementData : public IRawMeshElementData
    {
    public:
        RawMeshElementData(RawMeshData* mesh, PrimitiveType type)
            : mMesh(mesh), mPrimitiveType(type) {}

        PrimitiveType primitiveType() const override { return mPrimitiveType; }

        const std::string& name() const override { return mName; }
        void setName(const std::string& string) { mName = string; }
        void setName(std::string&& string) { mName = std::move(string); }

        const std::string& materialName() const override { return mMaterialName; }
        void setMaterialName(const std::string& string) { mMaterialName = string; }
        void setMaterialName(std::string&& string) { mMaterialName = std::move(string); }

        const BoundingBox& boundingBox() const override { return mBoundingBox; }
        void setBoundingBox(const BoundingBox& box) { mBoundingBox = box; }

        VERTEX* allocVertexBuffer(size_t count)
        {
            void* vertices = nullptr;
            mVertexBufferOffset = mMesh->appendVertices(count, &vertices, sizeof(VERTEX));
            mVertexCount = count;
            return reinterpret_cast<VERTEX*>(vertices);
        }

        uint16_t* allocIndexBuffer(size_t count)
        {
            uint16_t* indices = nullptr;
            mIndexBufferOffset = mMesh->appendIndices(count, &indices);
            mIndexCount = count;
            return indices;
        }

        const IVertexFormatAttributeList* vertexFormat() const override { return &VERTEX::attributes(); }
        size_t vertexBufferOffset() const override { return mVertexBufferOffset; }
        size_t vertexBufferSize() override { return mVertexCount * sizeof(VERTEX); }
        size_t firstIndex() const override { return mIndexBufferOffset; }
        size_t indexCount() override { return mIndexCount; }

    private:
        RawMeshData* mMesh;
        size_t mVertexBufferOffset = 0;
        size_t mVertexCount = 0;
        size_t mIndexBufferOffset = 0;
        size_t mIndexCount = 0;
        std::string mName;
        std::string mMaterialName;
        PrimitiveType mPrimitiveType;
        BoundingBox mBoundingBox;
    };
}
