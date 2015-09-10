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
#include "engine/image/Image.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/interfaces/core/IThreadManager.h"
#include "engine/material/ShaderLoader.h"

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
            virtual void setup(const RESOURCEPTR& resource) = 0;
        };

        template <typename LOADER>
        void loadResourceSync(typename LOADER::ResourcePtr& resource, const std::string& fileName)
        {
            LOADER loader;
            loader.fileName = std::move(fileName);

            resource = loader.create();

            if (loader.load())
                loader.setup(resource);
        }

        template <typename LOADER>
        void loadResourceAsync(typename LOADER::ResourcePtr& resource, const std::string& fileName,
            const std::shared_ptr<std::atomic<int>>& pendingResourceCounter)
        {
            struct Context
            {
                LOADER loader;
                std::shared_ptr<std::atomic<int>> pendingResourceCounter;
                typename LOADER::ResourcePtr resource;
            };

            std::shared_ptr<Context> context = std::make_shared<Context>();
            context->loader.fileName = std::move(fileName);
            context->pendingResourceCounter = pendingResourceCounter;

            resource = context->loader.create();
            context->resource = resource;

            ++*pendingResourceCounter;
            Services::threadManager()->performInBackgroundThread([context]() {
                if (!context->loader.load()) {
                    --*context->pendingResourceCounter;
                    return;
                }

                Services::threadManager()->performInRenderThread([context]() {
                    context->loader.setup(context->resource);
                    --*context->pendingResourceCounter;
                });
            });
        }

        template <typename LOADER, typename MAP>
        typename LOADER::ResourcePtr getResource(MAP& map, const std::string& fileName, bool async,
            const std::shared_ptr<std::atomic<int>>& pendingResourceCounter)
        {
            auto& weakRef = map[fileName];

            typename LOADER::ResourcePtr resource = weakRef.lock();
            if (resource)
                return resource;

            if (!async)
                loadResourceSync<LOADER>(resource, fileName);
            else
                loadResourceAsync<LOADER>(resource, fileName, pendingResourceCounter);

            weakRef = resource;
            return resource;
        }
    }


    ResourceManager ResourceManager::mInstance;

    ResourceManager::ResourceManager()
        : mNumPendingResources(std::make_shared<std::atomic<int>>())
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    int ResourceManager::numPendingResources() const
    {
        return mNumPendingResources->load();
    }

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

            void setup(const ShaderPtr& shader) override
            {
                shader->setVertexSource(ShaderLoader::vertexSource());
                shader->setFragmentSource(ShaderLoader::fragmentSource());
                shader->compile();
            }
        };

        return getResource<ShaderResourceLoader>(mShaders, fileName, async, mNumPendingResources);
    }

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

            void setup(const TexturePtr& texture) override
            {
                texture->upload(*mImage);
            }
        };

        return getResource<TextureResourceLoader>(mTextures, fileName, async, mNumPendingResources);
    }
}
