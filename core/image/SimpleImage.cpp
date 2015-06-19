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
#include "SimpleImage.h"

namespace Engine
{
    SimpleImage::SimpleImage(ImagePixelFormat format, size_t width, size_t height, size_t lineSize)
        : m_Width(width)
        , m_Height(height)
        , m_Depth(1)
        , m_LineSize(lineSize)
        , m_SliceSize(0)
        , m_Format(format)
    {
    }

    SimpleImage::SimpleImage(ImagePixelFormat format, size_t width, size_t height, size_t lineSize,
            std::vector<uint8_t>&& data)
        : m_Width(width)
        , m_Height(height)
        , m_Depth(1)
        , m_LineSize(lineSize)
        , m_SliceSize(0)
        , m_Data(std::move(data))
        , m_Format(format)
    {
    }

    SimpleImage::SimpleImage(ImagePixelFormat format, size_t width, size_t height, size_t depth,
            size_t lineSize, size_t sliceSize)
        : m_Width(width)
        , m_Height(height)
        , m_Depth(depth)
        , m_LineSize(lineSize)
        , m_SliceSize(sliceSize)
        , m_Format(format)
    {
    }

    SimpleImage::SimpleImage(ImagePixelFormat format, size_t width, size_t height, size_t depth,
            size_t lineSize, size_t sliceSize, std::vector<uint8_t>&& data)
        : m_Width(width)
        , m_Height(height)
        , m_Depth(depth)
        , m_LineSize(lineSize)
        , m_SliceSize(sliceSize)
        , m_Data(std::move(data))
        , m_Format(format)
    {
    }

    size_t SimpleImage::faceCount() const
    {
        return 1;
    }

    size_t SimpleImage::mipmapLevelCount(size_t faceIndex) const
    {
        return (faceIndex == 0 ? 1 : 0);
    }

    IImage* SimpleImage::mipmapLevel(size_t faceIndex, size_t mipmapLevel) const
    {
        return (faceIndex == 0 && mipmapLevel == 0 ? const_cast<SimpleImage*>(this) : nullptr);
    }

    ImagePixelFormat SimpleImage::pixelFormat() const
    {
        return m_Format;
    }

    size_t SimpleImage::width() const
    {
        return m_Width;
    }

    size_t SimpleImage::height() const
    {
        return m_Height;
    }

    size_t SimpleImage::depth() const
    {
        return m_Depth;
    }

    size_t SimpleImage::lineSize() const
    {
        return m_LineSize;
    }

    size_t SimpleImage::sliceSize() const
    {
        return m_SliceSize;
    }

    const void* SimpleImage::rawDataPointer() const
    {
        return m_Data.data();
    }

    size_t SimpleImage::rawDataSize() const
    {
        return m_Data.size();
    }
}
