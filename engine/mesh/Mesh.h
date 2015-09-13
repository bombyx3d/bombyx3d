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
#include "engine/core/macros.h"
#include "engine/mesh/MeshElement.h"
#include <vector>
#include <cstdint>

namespace Engine
{
    template <class VERTEX> class Mesh : public IMesh
    {
    public:
        Mesh();
        ~Mesh();

        const std::vector<MeshElement>& elements() const { return mElements; }
        void addElement(const MeshElement& element) { mElements.emplace_back(element); }
        void addElement(MeshElement&& element) { mElements.emplace_back(std::move(element)); }
        void setElements(const std::vector<MeshElement>& e) { mElements = e; }
        void setElements(std::vector<MeshElement>&& e) { mElements = std::move(e); }

        const std::vector<VERTEX>& vertexData() const { return mVertexData; }
        const std::vector<uint16_t>& indexData() const { return mIndexData; }

        size_t appendVertices(size_t count, VERTEX** vertices)
        {
            size_t index = mVertices.size();
            mVertices.resize(mVertices.size() + count);
            *vertices = &mVertices[index];
            return index;
        }

        size_t appendIndices(size_t count, uint16_t** indices)
        {
            size_t offset = mIndices.size();
            mIndices.resize(offset + count);
            *indices = &mIndices[offset];
            return offset;
        }

    private:
        std::vector<MeshElement> mElements;
        std::vector<VERTEX> mVertexData;
        std::vector<uint16_t> mIndexData;

        Z_DISABLE_COPY(Mesh);
    };
}
