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

namespace Engine
{
    /**
     * Desired configuration of a viewport.
     *
     * When creating a viewport, renderer should make its best to create viewport with the requested parameters.
     * However, renderer is free to ignore some of parameters if target device does not support the requested values.
     */
    class IViewportConfiguration : public IUnknown
    {
    public:
        Z_INTERFACE(IViewportConfiguration)

        enum {
            /** Represents an unspecified value. */
            UNSPECIFIED = -1
        };

        /**
         * Retrieves desired width of the viewport.
         * @return Desired width.
         */
        virtual int desiredWidth() const = 0;

        /**
         * Retrieves desired height of the viewport.
         * @return Desired height.
         */
        virtual int desiredHeight() const = 0;

        /**
         * Retrieves desired number of bits per pixel in the color buffer.
         * @return Desired number of bits per pixel.
         */
        virtual int desiredColorBits() const = 0;

        /**
         * Retrieves desired number of bits per pixel in the epeth buffer.
         * @return Desired number of bits per pixel.
         */
        virtual int desiredDepthBits() const = 0;

        /**
         * Retrieves desired number of bits per pixel in the stencil buffer.
         * @return Desired number of bits per pixel.
         */
        virtual int desiredStencilBits() const = 0;

        /**
         * Checks whether fullscreen mode is desired.
         * @return `true` if fullscreen mode is desired, otherwise returns `false`.
         */
        virtual bool desiredFullScreen() const = 0;
    };
}
