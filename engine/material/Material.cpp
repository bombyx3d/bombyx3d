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

namespace Engine
{
    std::vector<std::unique_ptr<IMaterialLoader>> Material::mMaterialLoaders;
    std::mutex Material::mMaterialLoadersMutex;

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
            return std::make_shared<Material>();
        }

        return materialLoader->loadMaterial(file);
    }

    void Material::registerLoader(std::unique_ptr<IMaterialLoader>&& loader)
    {
        std::lock_guard<decltype(mMaterialLoadersMutex)> lock(mMaterialLoadersMutex);
        mMaterialLoaders.emplace_back(std::move(loader));
    }
}
