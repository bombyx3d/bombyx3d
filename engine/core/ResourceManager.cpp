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
#include "ResourceManager.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include "engine/material/Material.h"
#include "engine/image/Image.h"
#include "engine/image/SpriteSheet.h"
#include "engine/mesh/Mesh.h"
#include "engine/mesh/RawMeshData.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/interfaces/core/IThreadManager.h"
#include "engine/material/ShaderLoader.h"
#include <glm/glm.hpp>

namespace Engine
{
    namespace
    {
        template <typename RESOURCEPTR> class ResourceLoader
        {
        public:
            typedef RESOURCEPTR ResourcePtr;
            std::string fileName;

            virtual ~ResourceLoader() = default;

            virtual RESOURCEPTR create() = 0;
            virtual bool load() = 0;
            virtual void setup(const RESOURCEPTR& resource, bool async) = 0;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename LOADER>
        void loadResourceSync(typename LOADER::ResourcePtr& resource, const std::string& fileName)
        {
            LOADER loader;
            loader.fileName = std::move(fileName);

            resource = loader.create();

            if (loader.load())
                loader.setup(resource, false);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename LOADER>
        void loadResourceAsync(typename LOADER::ResourcePtr& resource, const std::string& fileName,
            const std::shared_ptr<ResourceManager::Counters>& counters)
        {
            struct Context
            {
                LOADER loader;
                std::shared_ptr<ResourceManager::Counters> counters;
                typename LOADER::ResourcePtr resource;
            };

            std::shared_ptr<Context> context = std::make_shared<Context>();
            context->loader.fileName = std::move(fileName);
            context->counters = counters;

            resource = context->loader.create();
            context->resource = resource;

            context->counters->onBeginLoadResource();
            Services::threadManager()->performInBackgroundThread([context]() {
                if (!context->loader.load()) {
                    context->counters->onEndLoadResource();
                    return;
                }

                Services::threadManager()->performInRenderThread([context]() {
                    context->loader.setup(context->resource, true);
                    context->counters->onEndLoadResource();
                });
            });
        }

        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename LOADER, typename MAP>
        typename LOADER::ResourcePtr getResource(MAP& map, const std::string& fileName, bool async,
            const std::shared_ptr<ResourceManager::Counters>& counters)
        {
            auto& weakRef = map[fileName];

            typename LOADER::ResourcePtr resource = weakRef.lock();
            if (resource)
                return resource;

            if (!async)
                loadResourceSync<LOADER>(resource, fileName);
            else
                loadResourceAsync<LOADER>(resource, fileName, counters);

            weakRef = resource;
            return resource;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ResourceManager::ResourceManager()
        : mCounters(std::make_shared<Counters>())
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    bool ResourceManager::resourcesAreLoading() const
    {
        return mCounters->pending.load() != 0;
    }

    float ResourceManager::resourceLoadProgress() const
    {
        int complete = mCounters->complete.load();
        int total = mCounters->total.load();
        return glm::clamp(float(complete) / float(total), 0.0f, 1.0f);
    }

    ShaderPtr ResourceManager::compileShader(const std::vector<std::string>* source, const std::string& fileName)
    {
        auto& shader = mBuiltinShaders[source];
        if (!shader)
            shader = ShaderLoader::compile(fileName, *source);
        return shader;
    }

    ////////////////
    // Material

    MaterialPtr ResourceManager::getMaterial(const std::string& fileName, bool async)
    {
        struct MaterialResourceLoader : public ResourceLoader<MaterialPtr>
        {
            std::shared_ptr<Material> mMaterial;

            MaterialPtr create() override
            {
                mMaterial = std::make_shared<Material>();
                return mMaterial;
            }

            bool load() override
            {
                return mMaterial->load(fileName);
            }

            void setup(const MaterialPtr&, bool async) override
            {
                mMaterial->loadPendingResources(async);
            }
        };

        return getResource<MaterialResourceLoader>(mMaterials, fileName, async, mCounters);
    }

    ////////////////
    // Shader

    ShaderPtr ResourceManager::getShader(const std::string& fileName, bool async)
    {
        struct ShaderResourceLoader : public ResourceLoader<ShaderPtr>, public ShaderLoader
        {
            ShaderPtr create() override
            {
                return Services::renderer()->createShader();
            }

            bool load() override
            {
                Z_LOGI("Loading shader \"" << fileName << "\".");
                return ShaderLoader::loadFile(fileName);
            }

            void setup(const ShaderPtr& shader, bool) override
            {
                shader->setVertexSource(ShaderLoader::vertexSource());
                shader->setFragmentSource(ShaderLoader::fragmentSource());
                shader->compile();
            }
        };

        return getResource<ShaderResourceLoader>(mShaders, fileName, async, mCounters);
    }

    ////////////////
    // Texture

    TexturePtr ResourceManager::getTexture(const std::string& fileName, bool async)
    {
        struct TextureResourceLoader : public ResourceLoader<TexturePtr>
        {
            ImagePtr mImage;

            TexturePtr create() override
            {
                return Services::renderer()->createTexture();
            }

            bool load() override
            {
                mImage = Image::fromFile(fileName);
                return mImage && mImage->pixelFormat() != PixelFormat::Invalid;
            }

            void setup(const TexturePtr& texture, bool) override
            {
                texture->upload(*mImage);
            }
        };

        return getResource<TextureResourceLoader>(mTextures, fileName, async, mCounters);
    }

    ////////////////
    // Sprite sheet

    SpriteSheetPtr ResourceManager::getSpriteSheet(const std::string& fileName, bool async)
    {
        struct SpriteSheetResourceLoader : public ResourceLoader<SpriteSheetPtr>
        {
            std::shared_ptr<SpriteSheet> mSpriteSheet;

            SpriteSheetPtr create() override
            {
                mSpriteSheet = std::make_shared<SpriteSheet>();
                return mSpriteSheet;
            }

            bool load() override
            {
                return mSpriteSheet->load(fileName);
            }

            void setup(const SpriteSheetPtr&, bool async) override
            {
                mSpriteSheet->loadPendingResources(async);
            }
        };

        return getResource<SpriteSheetResourceLoader>(mSpriteSheets, fileName, async, mCounters);
    }

    ////////////////
    // Static mesh

    MeshPtr ResourceManager::getStaticMesh(const std::string& fileName, bool async)
    {
        struct StaticMeshResourceLoader : public ResourceLoader<MeshPtr>
        {
            std::shared_ptr<Mesh> mMesh;
            RawMeshDataPtr mMeshData;

            MeshPtr create() override
            {
                mMesh = std::make_shared<Mesh>();
                return mMesh;
            }

            bool load() override
            {
                mMeshData = RawMeshData::fromFile(fileName, false);
                return mMeshData != nullptr;
            }

            void setup(const MeshPtr&, bool async) override
            {
                mMesh->setData(mMeshData, BufferUsage::Static, async);
            }
        };

        return getResource<StaticMeshResourceLoader>(mStaticMeshes, fileName, async, mCounters);
    }
}
