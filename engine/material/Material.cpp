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
#include "Material.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"
#include <mutex>
#include <cassert>

namespace Engine
{
    const std::string Material::DEFAULT_FILE_EXTENSION = ".material";
    std::vector<std::unique_ptr<IMaterialLoader>> Material::mMaterialLoaders;
    std::mutex Material::mMaterialLoadersMutex;

    static const MaterialTechniquePtr gNullTechnique;

    Material::Material()
    {
    }

    Material::~Material()
    {
    }

    size_t Material::numTechniques() const
    {
        return mTechniques.size();
    }

    const MaterialTechniquePtr& Material::technique(const std::string& name) const
    {
        for (const auto& technique : mTechniques) {
            if (technique->name() == name)
                return technique;
        }
        return gNullTechnique;
    }

    const MaterialTechniquePtr& Material::technique(size_t index) const
    {
        assert(index < mTechniques.size());
        return (index < mTechniques.size() ? mTechniques[index] : gNullTechnique);
    }

    void Material::addTechnique(const MaterialTechniquePtr& technique)
    {
        mTechniques.emplace_back(technique);
    }

    void Material::addTechnique(MaterialTechniquePtr&& technique)
    {
        mTechniques.emplace_back(std::move(technique));
    }

    bool Material::load(const std::string& fileName)
    {
        return load(Services::fileSystem()->openFile(fileName).get());
    }

    bool Material::load(const FilePtr& file)
    {
        return load(file.get());
    }

    bool Material::load(IFile* file)
    {
        if (!file)
            return false;

        Z_LOGI("Loading material \"" << file->name() << "\"");

        IMaterialLoader* materialLoader = nullptr;
        {
            std::lock_guard<decltype(mMaterialLoadersMutex)> lock(mMaterialLoadersMutex);
            for (const auto& loader : mMaterialLoaders) {
                if (loader->canLoadMaterial(file)) {
                    materialLoader = loader.get();
                    break;
                }
            }
        }

        if (!materialLoader) {
            Z_LOGE("There is no loader able to read material \"" << file->name() << "\".");
            return false;
        }

        return materialLoader->loadMaterial(file, this);
    }

    void Material::loadPendingResources(bool async)
    {
        for (const auto& technique : mTechniques)
            technique->loadPendingResources(async);
    }

    MaterialPtr Material::fromFile(const std::string& name)
    {
        return fromFile(Services::fileSystem()->openFile(name).get());
    }

    MaterialPtr Material::fromFile(const FilePtr& file)
    {
        return fromFile(file.get());
    }

    MaterialPtr Material::fromFile(IFile* file)
    {
        if (!file)
            return std::make_shared<Material>();

        auto material = std::make_shared<Material>();
        material->load(file);

        return material;
    }

    void Material::registerLoader(std::unique_ptr<IMaterialLoader>&& loader)
    {
        std::lock_guard<decltype(mMaterialLoadersMutex)> lock(mMaterialLoadersMutex);
        mMaterialLoaders.emplace_back(std::move(loader));
    }
}
