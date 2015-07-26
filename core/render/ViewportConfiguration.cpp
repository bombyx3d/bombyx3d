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
#include "ViewportConfiguration.h"

namespace Engine
{
    int ViewportConfiguration::desiredWidth() const
    {
        return m_DesiredWidth;
    }

    int ViewportConfiguration::desiredHeight() const
    {
        return m_DesiredHeight;
    }

    int ViewportConfiguration::desiredColorBits() const
    {
        return m_DesiredColorBits;
    }

    int ViewportConfiguration::desiredDepthBits() const
    {
        return m_DesiredDepthBits;
    }

    int ViewportConfiguration::desiredStencilBits() const
    {
        return m_DesiredStencilBits;
    }

    bool ViewportConfiguration::desiredFullScreen() const
    {
        return m_DesiredFullScreen;
    }

    void ViewportConfiguration::setDesiredWidth(int width)
    {
        m_DesiredWidth = width;
    }

    void ViewportConfiguration::setDesiredHeight(int height)
    {
        m_DesiredHeight = height;
    }

    void ViewportConfiguration::setDesiredColorBits(int bits)
    {
        m_DesiredColorBits = bits;
    }

    void ViewportConfiguration::setDesiredDepthBits(int bits)
    {
        m_DesiredDepthBits = bits;
    }

    void ViewportConfiguration::setDesiredStencilBits(int bits)
    {
        m_DesiredStencilBits = bits;
    }

    void ViewportConfiguration::setDesiredFullScreen(bool flag)
    {
        m_DesiredFullScreen = flag;
    }
}
