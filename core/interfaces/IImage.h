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
#include "core/interfaces/IBlob.h"

namespace Engine
{
    /** Pixel format of the image. */
    enum ImagePixelFormat
    {
        /** 8-bit grayscale image. */
        PIXEL_FORMAT_LUMINANCE_8,

        /**
         * 16-bit luminance + alpha image with 8 bits per channel.
         * Channels are ordered in memory as `|L|A|`
         */
        PIXEL_FORMAT_LUMINANCE_ALPHA_16,

        /**
         * 24-bit RGB image with 8 bits per channel.
         * Channels are ordered in memory as `|R|G|B|`
         */
        PIXEL_FORMAT_RGB_24,

        /**
         * 32-bit RGBA image with 8 bits per channel.
         * Channels are ordered in memory as `|R|G|B|A|`
         */
        PIXEL_FORMAT_RGBA_32,

        /** Special value representing an invalid pixel format. */
        INVALID_PIXEL_FORMAT,
        /**
         * Special value equal to the total number of pixel formats defined in this enumeration
         * (excluding @ref INVALID_PIXEL_FORMAT).
         */
        PIXEL_FORMAT_COUNT = INVALID_PIXEL_FORMAT
    };

    /**
     * Interface to one-, two- or three-dimensional bitmap image.
     *
     * Implementations of this interface keep an image data layed out in memory as a continuous array of pixels.
     * Pixels are grouped into 1D lines, and lines are grouped into 2D bitmaps.
     */
    class IImage : public IBlob
    {
        Z_INTERFACE(IImage)

        /**
         * Retrieves pixel format of the image.
         * @return Pixel format of the image.
         */
        virtual ImagePixelFormat pixelFormat() const = 0;

        /**
         * Retrieves width of the image in pixels.
         * @return Width of the image in pixels.
         */
        virtual size_t width() const = 0;

        /**
         * Retrieves height of the image in pixels.
         * @return Height of the image in pixels.
         */
        virtual size_t height() const = 0;

        /**
         * Retrieves depth of the image in pixels.
         * @return Depth of the image in pixels.
         */
        virtual size_t depth() const = 0;

        /**
         * Retrieves size of a single 1D line of the image in bytes.
         * @return Size of a single 1D line of the image in bytes.
         */
        virtual size_t lineSize() const = 0;

        /**
         * Retrieves size of a single 2D slice of the image in bytes.
         * @return Size of a single 2D slice of the image in bytes.
         */
        virtual size_t sliceSize() const = 0;

        virtual const void* rawDataPointer() const override = 0;
        virtual size_t rawDataSize() const override = 0;
    };
}
