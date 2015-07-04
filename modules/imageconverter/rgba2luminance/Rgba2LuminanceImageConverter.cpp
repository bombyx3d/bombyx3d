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
#include "Rgba2LuminanceImageConverter.h"
#include "core/utility/debug.h"
#include "core/image/SimpleImage.h"

namespace Engine
{
    bool Rgba2LuminanceImageConverter::acceptsInputPixelFormat(ImagePixelFormat format) const
    {
        return format == PIXEL_FORMAT_RGB_24 || format == PIXEL_FORMAT_RGBA_32;
    }

    ImagePixelFormat Rgba2LuminanceImageConverter::outputPixelFormat() const
    {
        return PIXEL_FORMAT_LUMINANCE_ALPHA_16;
    }

    Ptr<IImage> Rgba2LuminanceImageConverter::convertImage(const IImage* image) const
    {
        ImagePixelFormat inputFormat = image->pixelFormat();
        Z_ASSERT(inputFormat == PIXEL_FORMAT_RGB_24 || inputFormat == PIXEL_FORMAT_RGBA_32);

        size_t bytesPerRow = image->lineSize();
        size_t width = image->width();
        size_t height = image->height();

        Ptr<SimpleImage> result = new SimpleImage(PIXEL_FORMAT_LUMINANCE_ALPHA_16, width, height, width * 2);
        result->data().resize(width * height * 2);

        const uint8_t* src = reinterpret_cast<const uint8_t*>(image->rawDataPointer());
        uint8_t* dst = result->data().data();

        if (inputFormat == PIXEL_FORMAT_RGB_24) {
            for (size_t y = 0; y < height; y++) {
                const uint8_t* s = src;
                uint8_t* d = dst;
                for (size_t x = 0; x < width; x++) {
                    uint32_t r = *s++;
                    uint32_t g = *s++;
                    uint32_t b = *s++;

                    // Approximation of Digital CCIR601: 0.299 R + 0.587 G + 0.114 B
                    uint32_t luminance = ((r << 1) + (g << 2) + g + b) >> 3;
                    luminance = (luminance <= 0xFF ? luminance : 0xFF);

                    *d++ = uint8_t(luminance);
                    *d++ = 0xFF;
                }
                src += bytesPerRow;
                dst += 2 * width;
            }
        } else if (inputFormat == PIXEL_FORMAT_RGBA_32) {
            for (size_t y = 0; y < height; y++) {
                const uint8_t* s = src;
                uint8_t* d = dst;
                for (size_t x = 0; x < width; x++) {
                    uint32_t r = *s++;
                    uint32_t g = *s++;
                    uint32_t b = *s++;
                    uint32_t a = *s++;

                    // Approximation of Digital CCIR601: 0.299 R + 0.587 G + 0.114 B
                    uint32_t luminance = ((r << 1) + (g << 2) + g + b) >> 3;
                    luminance = (luminance <= 0xFF ? luminance : 0xFF);

                    *d++ = uint8_t(luminance);
                    *d++ = uint8_t(a);
                }
                src += bytesPerRow;
                dst += 2 * width;
            }
        }

        return result;
    }
}
