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
#include "core/interfaces/IUnknown.h"
#include "core/interfaces/IImage.h"
#include "core/utility/Ptr.h"

namespace Engine
{
    /** Base interface for image pixel format converters. */
    class IImageFormatConverter : public IUnknown
    {
        Z_INTERFACE(IImageFormatConverter)

        /**
         * Checks whether the specified input pixel format is accepted by this converter.
         * @param format Input pixel format.
         * @return `true` if the specified pixel format is accepted, otherwise `false`.
         */
        virtual bool acceptsInputPixelFormat(ImagePixelFormat format) const = 0;

        /**
         * Returns output format of this converter.
         * @return Pixel format of output images.
         */
        virtual ImagePixelFormat outputPixelFormat() const = 0;

        /**
         * Converts image from one pixel format into another.
         * @param image Input image.
         * @return Output image or `nullptr` if image can't be converted.
         */
        virtual Ptr<IImage> convertImage(const IImage* image) const = 0;
    };
}
