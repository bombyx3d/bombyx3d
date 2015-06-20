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
#include "core/interfaces/IImage.h"
#include "core/interfaces/ITextureImage.h"
#include <vector>
#include <cstdint>

namespace Engine
{
    /**
     * Simple implementation of @ref IImage and @ref ITextureImage.
     *
     * @see
     * - @ref _textures.
     */
    class SimpleImage : public IImage, public ITextureImage
    {
    public:
        Z_IMPLEMENTATION(SimpleImage)

        /**
         * Constructs a 2D image.
         * @note Constructor does not allocate actual storage for the pixel data.
         * @param format Pixel format of the image.
         * @param imageWidth Width of the image.
         * @param imageHeight Height of the image.
         * @param imageLineSize Number of bytes per single 1D line of the image.
         */
        SimpleImage(ImagePixelFormat format, size_t imageWidth, size_t imageHeight, size_t imageLineSize);

        /**
         * Constructs a 2D image.
         * @param format Pixel format of the image.
         * @param imageWidth Width of the image.
         * @param imageHeight Height of the image.
         * @param imageLineSize Number of bytes per single 1D line of the image.
         * @param imageData Image data.
         */
        SimpleImage(ImagePixelFormat format, size_t imageWidth, size_t imageHeight, size_t imageLineSize,
            std::vector<uint8_t>&& imageData);

        /**
         * Constructs a 3D image.
         * @note Constructor does not allocate actual storage for the pixel data.
         * @param format Pixel format of the image.
         * @param imageWidth Width of the image.
         * @param imageHeight Height of the image.
         * @param imageDepth Depth of the image.
         * @param imageLineSize Number of bytes per single 1D line of the image.
         * @param imageSliceSize Number of bytes per single 2D slice of the image.
         */
        SimpleImage(ImagePixelFormat format, size_t imageWidth, size_t imageHeight, size_t imageDepth,
            size_t imageLineSize, size_t imageSliceSize);

        /**
         * Constructs a 3D image.
         * @param format Pixel format of the image.
         * @param imageWidth Width of the image.
         * @param imageHeight Height of the image.
         * @param imageDepth Depth of the image.
         * @param imageLineSize Number of bytes per single 1D line of the image.
         * @param imageSliceSize Number of bytes per single 2D slice of the image.
         * @param imageData Image data.
         */
        SimpleImage(ImagePixelFormat format, size_t imageWidth, size_t imageHeight, size_t imageDepth,
            size_t imageLineSize, size_t imageSliceSize, std::vector<uint8_t>&& imageData);

        /**
         * Retrieves reference to data.
         * @return Reference to data.
         */
        std::vector<uint8_t>& data()
        {
            return m_Data;
        }

        /**
         * Retrieves reference to data.
         * @return Reference to data.
         */
        const std::vector<uint8_t>& data() const
        {
            return m_Data;
        }

        /**
         * Retrieves reference to data.
         * @return Reference to data.
         */
        const std::vector<uint8_t>& constData() const
        {
            return m_Data;
        }

        size_t faceCount() const override;
        size_t mipmapLevelCount(size_t faceIndex) const override;
        IImage* mipmapLevel(size_t faceIndex, size_t mipmapLevel) const override;

        ImagePixelFormat pixelFormat() const override;
        size_t width() const override;
        size_t height() const override;
        size_t depth() const override;
        size_t lineSize() const override;
        size_t sliceSize() const override;

        const void* rawDataPointer() const override;
        size_t rawDataSize() const override;

    private:
        size_t m_Width;
        size_t m_Height;
        size_t m_Depth;
        size_t m_LineSize;
        size_t m_SliceSize;
        std::vector<uint8_t> m_Data;
        ImagePixelFormat m_Format;
    };
}
