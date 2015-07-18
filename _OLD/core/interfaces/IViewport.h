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
#include "core/utility/debug.h"

namespace Engine
{
    /** Viewport settings. */
    class ViewportSettings : public IUnknown
    {
    public:
        Z_IMPLEMENTATION(ViewportSettings)

        enum {
            /** Represents an unspecified value. */
            AUTO = -1
        };

        /** Constructor. */
        ViewportSettings()
            : m_DesiredWidth(1024)
            , m_DesiredHeight(768)
            , m_DesiredColorBits(AUTO)
            , m_DesiredDepthBits(AUTO)
            , m_DesiredStencilBits(AUTO)
            , m_FullScreen(false)
        {
        }

        /**
         * Retrieves desired width of the viewport.
         * @return Desired width.
         */
        int desiredWidth() const
        {
            return m_DesiredWidth;
        }

        /**
         * Sets desired width of the viewport.
         * @param width Desired width.
         */
        void setDesiredWidth(int width)
        {
            m_DesiredWidth = width;
        }

        /**
         * Retrieves desired height of the viewport.
         * @return Desired height.
         */
        int desiredHeight() const
        {
            return m_DesiredHeight;
        }

        /**
         * Sets desired height of the viewport.
         * @param height Desired height.
         */
        void setDesiredHeight(int height)
        {
            m_DesiredHeight = height;
        }

        /**
         * Retrieves desired number of bits per pixel in the color buffer.
         * @return Desired number of bits per pixel.
         */
        int desiredColorBits() const
        {
            return m_DesiredColorBits;
        }

        /**
         * Sets desired number of bits per pixel in the color buffer.
         * @param bits Desired number of bits per pixel.
         */
        void setDesiredColorBits(int bits)
        {
            m_DesiredColorBits = bits;
        }

        /**
         * Retrieves desired number of bits per pixel in the epeth buffer.
         * @return Desired number of bits per pixel.
         */
        int desiredDepthBits() const
        {
            return m_DesiredDepthBits;
        }

        /**
         * Sets desired number of bits per pixel in the depth buffer.
         * @param bits Desired number of bits per pixel.
         */
        void setDesiredDepthBits(int bits)
        {
            m_DesiredDepthBits = bits;
        }

        /**
         * Retrieves desired number of bits per pixel in the stencil buffer.
         * @return Desired number of bits per pixel.
         */
        int desiredStencilBits() const
        {
            return m_DesiredStencilBits;
        }

        /**
         * Sets desired number of bits per pixel in the stencil buffer.
         * @param bits Desired number of bits per pixel.
         */
        void setDesiredStencilBits(int bits)
        {
            m_DesiredStencilBits = bits;
        }

        /**
         * Checks whether fullscreen mode is desired.
         * @return `true` if fullscreen mode is desired, otherwise returns `false`.
         */
        bool fullScreen() const
        {
            return m_FullScreen;
        }

        /**
         * Sets fullscreen mode.
         * @param flag Set to `true` to enable fullscreen mode.
         */
        void setFullScreen(bool flag)
        {
            m_FullScreen = flag;
        }

    private:
        int m_DesiredWidth;
        int m_DesiredHeight;
        int m_DesiredColorBits;
        int m_DesiredDepthBits;
        int m_DesiredStencilBits;
        bool m_FullScreen;
    };

    /** Interface to the viewport. */
    class IViewport : public IUnknown
    {
    public:
        Z_INTERFACE(IViewport)

        /**
         * Retrieves width of the viewport.
         * @return Width of the viewport.
         */
        virtual int viewportWidth() const = 0;

        /**
         * Retrieves height of the viewport.
         * @return Height of the viewport.
         */
        virtual int viewportHeight() const = 0;

        #if 0
        /**
         * Retrieves a reference to the renderer for this viewport.
         * @return Reference to the renderer.
         */
        virtual IRenderer& renderer() = 0;
        #endif
    };
}
