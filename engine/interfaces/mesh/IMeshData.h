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
#include "engine/mesh/VertexFormat.h"
#include "engine/interfaces/render/IRenderer.h"
#include <memory>
#include <vector>
#include <string>
#include <cstdint>

namespace Engine
{
    class IMeshData
    {
    public:
        struct Element
        {
            std::string name;
            std::string materialName;
            size_t vertexBufferOffset;
            size_t vertexCount;
            size_t indexBufferOffset;
            size_t indexCount;
            PrimitiveType primitiveType;
            const IVertexFormatAttributeList* vertexFormat;
        };

        virtual ~IMeshData() = default;

        virtual const std::vector<Element>& elements() const = 0;
        virtual std::vector<Element>&& moveElements() = 0;

        virtual const std::vector<uint8_t>& vertexData() const = 0;
        virtual const std::vector<uint16_t>& indexData() const = 0;
    };

    using MeshDataPtr = std::shared_ptr<IMeshData>;
}
