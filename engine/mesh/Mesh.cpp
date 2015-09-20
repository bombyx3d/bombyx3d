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
#include "engine/core/Services.h"

namespace Engine
{
    Mesh::Mesh()
    {
        mVertexBuffer = Services::renderer()->createVertexBuffer();
        mIndexBuffer = Services::renderer()->createIndexBuffer();
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::setData(const RawMeshDataPtr& data, BufferUsage usage)
    {
        assert(data != nullptr);
        if (!data)
            return;

        mBoundingBox = data->boundingBox();

        const auto& vertices = data->vertexData();
        const size_t vertexElementSize = sizeof(std::remove_reference<decltype(vertices)>::type::value_type);
        mVertexBuffer->setData(vertices.data(), vertices.size() * vertexElementSize, usage);

        const auto& indices = data->indexData();
        const size_t indexElementSize = sizeof(std::remove_reference<decltype(indices)>::type::value_type);
        mIndexBuffer->setData(indices.data(), indices.size() * indexElementSize, usage);

        mElements.reserve(data->elements().size());
        for (const auto& dataElement : data->elements()) {
            Element meshElement;

            meshElement.primitiveType = dataElement->primitiveType();
            meshElement.firstIndex = dataElement->firstIndex();
            meshElement.indexCount = dataElement->indexCount();

            meshElement.material = Services::resourceManager()->getMaterial(dataElement->materialName());

            meshElement.vertexSource = Services::renderer()->createVertexSource();
            meshElement.vertexSource->setAttributes(
                *dataElement->vertexFormat(), mVertexBuffer, dataElement->vertexBufferOffset());
            meshElement.vertexSource->setIndexBuffer(mIndexBuffer);

            mElements.emplace_back(std::move(meshElement));
        }
    }

    void Mesh::render() const
    {
        IRenderer* renderer = Services::renderer().get();

        for (const auto& element : mElements) {
            if (element.material->numTechniques() == 0)
                continue;

            IMaterialTechnique* technique = element.material->technique(0).get();
            size_t numPasses = technique->numPasses();
            if (numPasses == 0)
                continue;

            renderer->bindVertexSource(element.vertexSource);
            for (size_t i = 0; i < numPasses; i++) {
                technique->pass(i)->apply(renderer);
                renderer->drawPrimitive(element.primitiveType, element.firstIndex, element.indexCount);
            }
        }
    }
}
