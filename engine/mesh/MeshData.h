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
#include "engine/interfaces/mesh/IMeshData.h"
#include "engine/interfaces/mesh/IMeshLoader.h"
#include "engine/core/macros.h"
#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>

namespace Engine
{
    class MeshData : public IMeshData
    {
    public:
        MeshData();
        ~MeshData();

        const std::vector<Element>& elements() const override { return mElements; }

        void addElement(const Element& element);
        void addElement(Element&& element);
        void setElements(const std::vector<Element>& e);
        void setElements(std::vector<Element>&& e);

        std::vector<uint8_t>& vertexData() { return mVertexData; }
        const std::vector<uint8_t>& vertexData() const override { return mVertexData; }

        std::vector<uint16_t>& indexData() { return mIndexData; }
        const std::vector<uint16_t>& indexData() const override { return mIndexData; }

        const glm::vec3 boundingBoxMin() const override { return mMin; }
        const glm::vec3 boundingBoxMax() const override { return mMax; }
        void setBoundingBox(const glm::vec3& min, const glm::vec3& max) { mMin = min; mMax = max; }

        size_t appendIndices(size_t count, uint16_t** indices);
        template <class VERTEX> size_t appendVertices(size_t count, VERTEX** vertices)
        {
            size_t offset = 0;
            *vertices = reinterpret_cast<VERTEX*>(appendVertices(count, &offset, sizeof(VERTEX)));
            return offset;
        }

        static MeshDataPtr fromFile(const std::string& name, bool loadSkeleton);
        static MeshDataPtr fromFile(const FilePtr& file, bool loadSkeleton);
        static MeshDataPtr fromFile(IFile* file, bool loadSkeleton);

        static void registerLoader(std::unique_ptr<IMeshLoader>&& loader);
        template <typename TYPE, typename... ARGS> static void registerLoader(ARGS&&... args)
            { registerLoader(std::unique_ptr<TYPE>(new TYPE(std::forward<ARGS>(args)...))); }

    private:
        static std::vector<std::unique_ptr<IMeshLoader>> mMeshLoaders;
        static std::mutex mMeshLoadersMutex;

        std::vector<Element> mElements;
        std::vector<uint8_t> mVertexData;
        std::vector<uint16_t> mIndexData;
        glm::vec3 mMin;
        glm::vec3 mMax;

        void* appendVertices(size_t count, size_t* offset, size_t vertexSize);

        void adjustBoundingBox(const glm::vec3& min, const glm::vec3& max, bool first);
        void calculateBoundingBox();

        Z_DISABLE_COPY(MeshData);
    };
}
