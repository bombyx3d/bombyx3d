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
#include "core/interfaces/render/IViewportConfiguration.h"

namespace Engine
{
    /** Viewport configuration. */
    class ViewportConfiguration : public IViewportConfiguration
    {
    public:
        Z_IMPLEMENTATION(ViewportConfiguration)

        /** Constructor. */
        ViewportConfiguration()
            : m_DesiredWidth(1024)
            , m_DesiredHeight(768)
            , m_DesiredColorBits(UNSPECIFIED)
            , m_DesiredDepthBits(UNSPECIFIED)
            , m_DesiredStencilBits(UNSPECIFIED)
            , m_DesiredFullScreen(false)
        {
        }

        int desiredWidth() const override;
        int desiredHeight() const override;
        int desiredColorBits() const override;
        int desiredDepthBits() const override;
        int desiredStencilBits() const override;
        bool desiredFullScreen() const override;

        /**
         * Sets desired width of the viewport.
         * @param width Desired width.
         */
        virtual void setDesiredWidth(int width);

        /**
         * Sets desired height of the viewport.
         * @param height Desired height.
         */
        virtual void setDesiredHeight(int height);

        /**
         * Sets desired number of bits per pixel in the color buffer.
         * @param bits Desired number of bits per pixel.
         */
        virtual void setDesiredColorBits(int bits);

        /**
         * Sets desired number of bits per pixel in the depth buffer.
         * @param bits Desired number of bits per pixel.
         */
        virtual void setDesiredDepthBits(int bits);

        /**
         * Sets desired number of bits per pixel in the stencil buffer.
         * @param bits Desired number of bits per pixel.
         */
        virtual void setDesiredStencilBits(int bits);

        /**
         * Sets fullscreen mode.
         * @param flag Set to `true` to enable fullscreen mode.
         */
        virtual void setDesiredFullScreen(bool flag);

    private:
        int m_DesiredWidth;
        int m_DesiredHeight;
        int m_DesiredColorBits;
        int m_DesiredDepthBits;
        int m_DesiredStencilBits;
        bool m_DesiredFullScreen;
    };
}
