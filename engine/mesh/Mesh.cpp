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
#include "Mesh.h"
#include "engine/mesh/MeshElement.h"
#include "engine/core/Services.h"
#include <cassert>

namespace Engine
{
    Mesh::Mesh()
        : mBoundingBoxMin(0.0f)
        , mBoundingBoxMax(0.0f)
    {
        mVertexBuffer = Services::renderer()->createVertexBuffer();
        mIndexBuffer = Services::renderer()->createIndexBuffer();
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::setData(const MeshDataPtr& data, BufferUsage usage)
    {
        assert(data != nullptr);
        if (!data)
            return;

        mBoundingBoxMin = data->boundingBoxMin();
        mBoundingBoxMax = data->boundingBoxMax();

        const auto& vertices = data->vertexData();
        mVertexBuffer->setData(vertices.data(), vertices.size(), usage);

        const auto& indices = data->indexData();
        mIndexBuffer->setData(indices.data(), indices.size() * sizeof(uint16_t), usage);

        auto self = shared_from_this();
        mElements.reserve(data->elements().size());
        for (const auto& dataElement : data->elements()) {
            auto meshElement = std::make_shared<MeshElement>(self);
            meshElement->setName(dataElement.name);
            meshElement->setMaterialName(dataElement.materialName);
            meshElement->setVertexFormat(dataElement.vertexFormat);
            meshElement->setVertexRange(dataElement.vertexBufferOffset, dataElement.vertexCount);
            meshElement->setIndexRange(dataElement.indexBufferOffset, dataElement.indexCount);
            meshElement->setPrimitiveType(dataElement.primitiveType);
            meshElement->setVertexFormat(dataElement.vertexFormat);
            meshElement->setBoundingBox(dataElement.boundingBoxMin, dataElement.boundingBoxMax);

            // Ensure that lazy-initialized resources are created now
            meshElement->material();
            meshElement->vertexSource();

            mElements.emplace_back(std::move(meshElement));
        }
    }
}
