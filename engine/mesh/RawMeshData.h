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
#include "engine/core/macros.h"
#include "engine/interfaces/mesh/IRawMeshData.h"
#include "engine/interfaces/mesh/IMeshLoader.h"
#include <mutex>
#include <cstdint>
#include <vector>
#include <memory>

namespace Engine
{
    template <class VERTEX> class RawMeshElementData;

    class RawMeshData : public IRawMeshData
    {
    public:
        RawMeshData();
        ~RawMeshData();

        const std::vector<RawMeshElementDataPtr>& elements() const override { return mElements; }
        template <class TYPE> RawMeshElementData<TYPE>* addElement(PrimitiveType primitiveType)
        {
            auto element = new RawMeshElementData<TYPE>(this, primitiveType);
            mElements.emplace_back(element);
            return element;
        }

        const BoundingBox& boundingBox() const override { return mBoundingBox; }
        void setBoundingBox(const BoundingBox& box) { mBoundingBox = box; }

        const std::vector<uint8_t>& vertexData() const override { return mVertexData; }
        const std::vector<uint16_t>& indexData() const override { return mIndexData; }

        size_t appendVertices(size_t count, void** vertices, size_t vertexSize);
        size_t appendIndices(size_t count, uint16_t** indices);

        static RawMeshDataPtr fromFile(const std::string& name, bool loadSkeleton);
        static RawMeshDataPtr fromFile(const FilePtr& file, bool loadSkeleton);
        static RawMeshDataPtr fromFile(IFile* file, bool loadSkeleton);

        static void registerLoader(std::unique_ptr<IMeshLoader>&& loader);
        template <typename TYPE, typename... ARGS> static void registerLoader(ARGS&&... args)
            { registerLoader(std::unique_ptr<TYPE>(new TYPE(std::forward<ARGS>(args)...))); }

    private:
        static std::vector<std::unique_ptr<IMeshLoader>> mMeshLoaders;
        static std::mutex mMeshLoadersMutex;

        BoundingBox mBoundingBox;
        std::vector<RawMeshElementDataPtr> mElements;
        std::vector<uint8_t> mVertexData;
        std::vector<uint16_t> mIndexData;

        B3D_DISABLE_COPY(RawMeshData);
    };
}
