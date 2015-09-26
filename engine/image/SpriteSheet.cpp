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
#include "SpriteSheet.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"
#include "engine/image/Sprite.h"
#include <cassert>

namespace Engine
{
    std::vector<std::unique_ptr<ISpriteSheetLoader>> SpriteSheet::mSpriteSheetLoaders;
    std::mutex SpriteSheet::mSpriteSheetLoadersMutex;

    SpriteSheet::SpriteSheet()
    {
    }

    SpriteSheet::~SpriteSheet()
    {
    }

    SpritePtr SpriteSheet::getSprite(const std::string& name)
    {
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        auto it = mSprites.find(name);
        if (it == mSprites.end()) {
            auto element = std::make_shared<Element>();
            element->originalQuad = Quad::allZero();
            element->trimmedQuad = Quad::allZero();
            element->textureCoordinates = Quad::allZero();
            it = mSprites.emplace(name, std::move(element)).first;
        }
        return std::make_shared<Sprite>(shared_from_this(), it->second);
    }

    void SpriteSheet::setSprite(const std::string& name, Element&& element)
    {
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        auto it = mSprites.find(name);
        if (it != mSprites.end())
            *it->second = std::move(element);
        else
            mSprites.emplace(name, std::make_shared<Element>(std::move(element)));
    }

    bool SpriteSheet::load(const std::string& fileName)
    {
        return load(Services::fileSystem()->openFile(fileName).get());
    }

    bool SpriteSheet::load(const FilePtr& file)
    {
        return load(file.get());
    }

    bool SpriteSheet::load(IFile* file)
    {
        if (!file)
            return false;

        Z_LOGI("Loading sprite sheet \"" << file->name() << "\"");

        ISpriteSheetLoader* spriteSheetLoader = nullptr;
        {
            std::lock_guard<decltype(mSpriteSheetLoadersMutex)> lock(mSpriteSheetLoadersMutex);
            for (const auto& loader : mSpriteSheetLoaders) {
                if (loader->canLoadSpriteSheet(file)) {
                    spriteSheetLoader = loader.get();
                    break;
                }
            }
        }

        if (!spriteSheetLoader) {
            Z_LOGE("There is no loader able to read sprite sheet \"" << file->name() << "\".");
            return false;
        }

        return spriteSheetLoader->loadSpriteSheet(file, this);
    }

    void SpriteSheet::loadPendingResources()
    {
    }

    SpriteSheetPtr SpriteSheet::fromFile(const std::string& name)
    {
        return fromFile(Services::fileSystem()->openFile(name).get());
    }

    SpriteSheetPtr SpriteSheet::fromFile(const FilePtr& file)
    {
        return fromFile(file.get());
    }

    SpriteSheetPtr SpriteSheet::fromFile(IFile* file)
    {
        if (!file)
            return std::make_shared<SpriteSheet>();

        auto sheet = std::make_shared<SpriteSheet>();
        sheet->load(file);

        return sheet;
    }

    void SpriteSheet::registerLoader(std::unique_ptr<ISpriteSheetLoader>&& loader)
    {
        std::lock_guard<decltype(mSpriteSheetLoadersMutex)> lock(mSpriteSheetLoadersMutex);
        mSpriteSheetLoaders.emplace_back(std::move(loader));
    }
}
