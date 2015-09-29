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
#include "Image.h"
#include "engine/interfaces/io/IFileSystem.h"
#include "engine/interfaces/image/IImageLoader.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include <cstring>

namespace B3D
{
    std::vector<std::unique_ptr<IImageLoader>> Image::mImageLoaders;
    std::mutex Image::mImageLoadersMutex;

    Image::Image()
        : mWidth(0)
        , mHeight(0)
        , mPixelFormat(PixelFormat::Invalid)
    {
    }

    Image::Image(PixelFormat fmt, size_t w, size_t h)
        : mWidth(w)
        , mHeight(h)
        , mPixelFormat(fmt)
    {
    }

    Image::~Image()
    {
    }

    void Image::setDimensions(size_t w, size_t h)
    {
        mWidth = w;
        mHeight = h;
    }

    void Image::setDataSize(size_t size)
    {
        mData.resize(size);
    }

    void Image::setData(const void* pointer, size_t size)
    {
        mData.resize(size);
        memcpy(mData.data(), pointer, size);
    }

    void Image::setData(const std::vector<uint8_t>& newData)
    {
        mData = newData;
    }

    ImagePtr Image::fromFile(const std::string& name)
    {
        return fromFile(Services::fileSystem()->openFile(name).get());
    }

    ImagePtr Image::fromFile(const FilePtr& file)
    {
        return fromFile(file.get());
    }

    ImagePtr Image::fromFile(IFile* file)
    {
        if (!file)
            return std::make_shared<Image>();

        B3D_LOGI("Loading image \"" << file->name() << "\"");

        IImageLoader* imageLoader = nullptr;
        {
            std::lock_guard<decltype(mImageLoadersMutex)> lock(mImageLoadersMutex);
            for (const auto& loader : mImageLoaders) {
                if (loader->canLoadImage(file)) {
                    imageLoader = loader.get();
                    break;
                }
            }
        }

        if (!imageLoader) {
            B3D_LOGE("There is no loader able to read image \"" << file->name() << "\".");
            return std::make_shared<Image>();
        }

        return imageLoader->loadImage(file);
    }

    void Image::registerLoader(std::unique_ptr<IImageLoader>&& loader)
    {
        std::lock_guard<decltype(mImageLoadersMutex)> lock(mImageLoadersMutex);
        mImageLoaders.emplace_back(std::move(loader));
    }
}
