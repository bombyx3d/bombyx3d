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
#include "MeshData.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"

namespace Engine
{
    std::vector<std::unique_ptr<IMeshLoader>> MeshData::mMeshLoaders;
    std::mutex MeshData::mMeshLoadersMutex;

    MeshData::MeshData()
        : mMin(0.0f)
        , mMax(0.0f)
    {
    }

    MeshData::~MeshData()
    {
    }

    void MeshData::addElement(const Element& element)
    {
        adjustBoundingBox(element.boundingBoxMin, element.boundingBoxMax, mElements.empty());
        mElements.emplace_back(element);
    }

    void MeshData::addElement(Element&& element)
    {
        adjustBoundingBox(element.boundingBoxMin, element.boundingBoxMax, mElements.empty());
        mElements.emplace_back(std::move(element));
    }

    void MeshData::setElements(const std::vector<Element>& e)
    {
        mElements = e;
        calculateBoundingBox();
    }

    void MeshData::setElements(std::vector<Element>&& e)
    {
        mElements = std::move(e);
        calculateBoundingBox();
    }

    size_t MeshData::appendIndices(size_t count, uint16_t** indices)
    {
        size_t offset = mIndexData.size();
        mIndexData.resize(offset + count);
        *indices = &mIndexData[offset];
        return offset;
    }

    MeshDataPtr MeshData::fromFile(const std::string& name, bool loadSkeleton)
    {
        return fromFile(Services::fileSystem()->openFile(name).get(), loadSkeleton);
    }

    MeshDataPtr MeshData::fromFile(const FilePtr& file, bool loadSkeleton)
    {
        return fromFile(file.get(), loadSkeleton);
    }

    MeshDataPtr MeshData::fromFile(IFile* file, bool loadSkeleton)
    {
        if (!file)
            return std::make_shared<MeshData>();

        Z_LOGI("Loading mesh \"" << file->name() << "\"");

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
            Z_LOGE("There is no loader able to read mesh \"" << file->name() << "\".");
            return std::make_shared<MeshData>();
        }

        return meshLoader->loadMesh(file, loadSkeleton);
    }

    void MeshData::registerLoader(std::unique_ptr<IMeshLoader>&& loader)
    {
        std::lock_guard<decltype(mMeshLoadersMutex)> lock(mMeshLoadersMutex);
        mMeshLoaders.emplace_back(std::move(loader));
    }

    void* MeshData::appendVertices(size_t count, size_t* offset, size_t vertexSize)
    {
        *offset = mVertexData.size();
        mVertexData.resize(*offset + count * vertexSize);
        return &mVertexData[*offset];
    }

    void MeshData::adjustBoundingBox(const glm::vec3& min, const glm::vec3& max, bool first)
    {
        if (first) {
            mMin = min;
            mMax = max;
        } else {
            mMin = glm::min(mMin, min);
            mMax = glm::max(mMax, max);
        }
    }

    void MeshData::calculateBoundingBox()
    {
        if (mElements.empty()) {
            mMin = glm::vec3(0.0f);
            mMax = glm::vec3(0.0f);
        } else {
            adjustBoundingBox(mElements[0].boundingBoxMin, mElements[0].boundingBoxMax, true);
            for (size_t i = 1; i < mElements.size(); i++)
                adjustBoundingBox(mElements[i].boundingBoxMin, mElements[i].boundingBoxMax, false);
        }
    }
}
