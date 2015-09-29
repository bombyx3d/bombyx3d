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
#include "engine/interfaces/io/IFile.h"
#include "engine/interfaces/image/IImage.h"
#include <vector>
#include <mutex>
#include <cstdint>
#include <memory>

namespace B3D
{
    class IImageLoader;

    class Image : public IImage
    {
    public:
        Image();
        Image(PixelFormat fmt, size_t w, size_t h);
        ~Image();

        PixelFormat pixelFormat() const override { return mPixelFormat; }
        void setPixelFormat(PixelFormat format) { mPixelFormat = format; }

        size_t width() const override { return mWidth; }
        size_t height() const override { return mHeight; }
        void setDimensions(size_t w, size_t h = 1);

        size_t dataSize() const override { return mData.size(); }
        void setDataSize(size_t size);

        uint8_t* data() { return mData.data(); }
        const uint8_t* data() const override { return mData.data(); }

        void setData(const void* pointer, size_t size);
        void setData(const std::vector<uint8_t>& newData);
        void setData(std::vector<uint8_t>&& newData) { mData = std::move(newData); }

        static ImagePtr fromFile(const std::string& name);
        static ImagePtr fromFile(const FilePtr& file);
        static ImagePtr fromFile(IFile* file);

        static void registerLoader(std::unique_ptr<IImageLoader>&& loader);
        template <typename TYPE, typename... ARGS> static void registerLoader(ARGS&&... args)
            { registerLoader(std::unique_ptr<TYPE>(new TYPE(std::forward<ARGS>(args)...))); }

    private:
        static std::vector<std::unique_ptr<IImageLoader>> mImageLoaders;
        static std::mutex mImageLoadersMutex;

        std::vector<uint8_t> mData;
        size_t mWidth;
        size_t mHeight;
        PixelFormat mPixelFormat;
    };
}
