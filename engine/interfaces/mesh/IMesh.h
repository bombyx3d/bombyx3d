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
#include "engine/interfaces/render/IIndexBuffer.h"
#include "engine/interfaces/render/IVertexBuffer.h"
#include "engine/interfaces/mesh/IMeshElement.h"
#include <memory>
#include <vector>

namespace Engine
{
    class IMesh
    {
    public:
        virtual ~IMesh() = default;

        virtual const VertexBufferPtr& vertexBuffer() const = 0;
        virtual const IndexBufferPtr& indexBuffer() const = 0;

        virtual const glm::vec3& boundingBoxMin() const = 0;
        virtual const glm::vec3& boundingBoxMax() const = 0;

        virtual const std::vector<MeshElementPtr>& elements() const = 0;
    };

    using MeshPtr = std::shared_ptr<IMesh>;
}
