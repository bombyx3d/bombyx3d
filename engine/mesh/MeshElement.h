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
#include "engine/core/Services.h"
#include "engine/mesh/VertexFormat.h"
#include "engine/interfaces/mesh/IMesh.h"
#include "engine/interfaces/mesh/IMeshElement.h"
#include <memory>
#include <string>

namespace Engine
{
    class MeshElement : public IMeshElement
    {
    public:
        explicit MeshElement(const MeshPtr& mesh);
        ~MeshElement() = default;

        const std::string& name() const override { return mName; }
        void setName(const std::string& elementName) { mName = elementName; }
        void setName(std::string&& elementName) { mName = std::move(elementName); }

        const std::string& materialName() const override { return mMaterialName; }
        void setMaterialName(const std::string& matName) { mMaterialName = matName; mMaterial.reset(); }
        void setMaterialName(std::string&& matName) { mMaterialName = std::move(matName); mMaterial.reset(); }

        const IVertexFormatAttributeList* vertexFormat() const { return mVertexFormat; }
        void setVertexFormat(const IVertexFormatAttributeList* attributes) { mVertexFormat = attributes; }

        PrimitiveType primitiveType() const override { return mPrimitiveType; }
        void setPrimitiveType(PrimitiveType type) { mPrimitiveType = type; }

        size_t vertexBufferOffset() const { return mVertexBufferOffset; }
        size_t vertexCount() const { return mVertexCount; }
        void setVertexRange(size_t offset, size_t count) { mVertexBufferOffset = offset; mVertexCount = count; }

        size_t indexBufferOffset() const override { return mIndexBufferOffset; }
        size_t indexCount() const override { return mIndexCount; }
        void setIndexRange(size_t offset, size_t count) { mIndexBufferOffset = offset; mIndexCount = count; }

        const BoundingBox& boundingBox() const override { return mBoundingBox; }
        void setBoundingBox(const BoundingBox& box) { mBoundingBox = box; }

        const MaterialPtr& material() const override;
        const VertexSourcePtr& vertexSource() const override;

    private:
        std::weak_ptr<IMesh> mMesh;
        std::string mName;
        std::string mMaterialName;
        mutable MaterialPtr mMaterial;
        mutable VertexSourcePtr mVertexSource;
        size_t mVertexBufferOffset = 0;
        size_t mVertexCount = 0;
        size_t mIndexBufferOffset = 0;
        size_t mIndexCount = 0;
        const IVertexFormatAttributeList* mVertexFormat = nullptr;
        PrimitiveType mPrimitiveType = PrimitiveType::Points;
        BoundingBox mBoundingBox;
    };
}
