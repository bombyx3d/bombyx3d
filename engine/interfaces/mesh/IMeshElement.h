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
#include "engine/interfaces/material/IMaterial.h"
#include <memory>

namespace Engine
{
    class IMeshElement
    {
    public:
        virtual ~IMeshElement() = default;

        virtual const std::string& name() const = 0;

        virtual const std::string& materialName() const = 0;
        virtual const MaterialPtr& material() const = 0;

        virtual const VertexSourcePtr& vertexSource() const = 0;

        virtual PrimitiveType primitiveType() const = 0;
        virtual size_t indexBufferOffset() const = 0;
        virtual size_t indexCount() const = 0;

        virtual const glm::vec3& boundingBoxMin() const = 0;
        virtual const glm::vec3& boundingBoxMax() const = 0;
    };

    using MeshElementPtr = std::shared_ptr<IMeshElement>;
}
