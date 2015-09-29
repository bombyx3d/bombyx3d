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
#include "engine/math/BoundingBox.h"
#include "engine/interfaces/material/IMaterial.h"
#include "engine/interfaces/render/lowlevel/IRenderer.h"
#include <memory>
#include <string>

namespace B3D
{
    class IRawMeshElementData
    {
    public:
        virtual ~IRawMeshElementData() = default;

        virtual const std::string& name() const = 0;
        virtual const std::string& materialName() const = 0;
        virtual PrimitiveType primitiveType() const = 0;

        virtual const BoundingBox& boundingBox() const = 0;

        virtual const IVertexFormatAttributeList* vertexFormat() const = 0;
        virtual size_t vertexBufferOffset() const = 0;
        virtual size_t vertexBufferSize() = 0;
        virtual size_t firstIndex() const = 0;
        virtual size_t indexCount() = 0;
    };

    using RawMeshElementDataPtr = std::unique_ptr<IRawMeshElementData>;
}
