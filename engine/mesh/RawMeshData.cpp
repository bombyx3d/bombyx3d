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
#include "RawMeshData.h"
#include "engine/mesh/RawMeshElementData.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"

namespace Engine
{
    std::vector<std::unique_ptr<IMeshLoader>> RawMeshData::mMeshLoaders;
    std::mutex RawMeshData::mMeshLoadersMutex;

    RawMeshData::RawMeshData()
    {
    }

    RawMeshData::~RawMeshData()
    {
    }

    size_t RawMeshData::appendVertices(size_t count, void** vertices, size_t vertexSize)
    {
        size_t offset = mVertexData.size();
        mVertexData.resize(offset + count * vertexSize);
        *vertices = &mVertexData[offset];
        return offset;
    }

    size_t RawMeshData::appendIndices(size_t count, uint16_t** indices)
    {
        size_t offset = mIndexData.size();
        mIndexData.resize(offset + count);
        *indices = &mIndexData[offset];
        return offset;
    }

    RawMeshDataPtr RawMeshData::fromFile(const std::string& name, bool loadSkeleton)
    {
        return fromFile(Services::fileSystem()->openFile(name).get(), loadSkeleton);
    }

    RawMeshDataPtr RawMeshData::fromFile(const FilePtr& file, bool loadSkeleton)
    {
        return fromFile(file.get(), loadSkeleton);
    }

    RawMeshDataPtr RawMeshData::fromFile(IFile* file, bool loadSkeleton)
    {
        if (!file)
            return std::make_shared<RawMeshData>();

        B3D_LOGI("Loading mesh \"" << file->name() << "\"");

        IMeshLoader* meshLoader = nullptr;
        {
            std::lock_guard<decltype(mMeshLoadersMutex)> lock(mMeshLoadersMutex);
            for (const auto& loader : mMeshLoaders) {
                if (loader->canLoadMesh(file)) {
                    meshLoader = loader.get();
                    break;
                }
            }
        }

        if (!meshLoader) {
            B3D_LOGE("There is no loader able to read mesh \"" << file->name() << "\".");
            return std::make_shared<RawMeshData>();
        }

        return meshLoader->loadMesh(file, loadSkeleton);
    }

    void RawMeshData::registerLoader(std::unique_ptr<IMeshLoader>&& loader)
    {
        std::lock_guard<decltype(mMeshLoadersMutex)> lock(mMeshLoadersMutex);
        mMeshLoaders.emplace_back(std::move(loader));
    }
}
