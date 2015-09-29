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
#include "engine/interfaces/core/IResourceManager.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <atomic>

namespace B3D
{
    class ResourceManager : public IResourceManager
    {
    public:
        struct Counters
        {
            std::atomic<int> total;
            std::atomic<int> complete;
            std::atomic<int> pending;

            Counters() : total(0), complete(0), pending(0) {}
            void onBeginLoadResource() { ++pending; ++total; }
            void onEndLoadResource() { ++complete; --pending; }
        };

        ResourceManager();
        ~ResourceManager();

        bool resourcesAreLoading() const override;
        float resourceLoadProgress() const override;

        ShaderPtr compileShader(const std::vector<std::string>* source,
            const std::string& fileName = "<builtin>") override;

        MaterialPtr getMaterial(const std::string& fileName, bool async = true) override;
        ShaderPtr getShader(const std::string& fileName, bool async = true) override;
        TexturePtr getTexture(const std::string& fileName, bool async = true) override;
        SpriteSheetPtr getSpriteSheet(const std::string& fileName, bool async = true) override;
        MeshPtr getStaticMesh(const std::string& fileName, bool async = true) override;

    private:
        std::unordered_map<std::string, std::weak_ptr<IMaterial>> mMaterials;
        std::unordered_map<std::string, std::weak_ptr<IShader>> mShaders;
        std::unordered_map<const void*, std::shared_ptr<IShader>> mBuiltinShaders;
        std::unordered_map<std::string, std::weak_ptr<ITexture>> mTextures;
        std::unordered_map<std::string, std::weak_ptr<ISpriteSheet>> mSpriteSheets;
        std::unordered_map<std::string, std::weak_ptr<IMesh>> mStaticMeshes;
        std::shared_ptr<Counters> mCounters;

        B3D_DISABLE_COPY(ResourceManager);
    };
}
