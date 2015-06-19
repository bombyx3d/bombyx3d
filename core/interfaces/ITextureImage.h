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

namespace Engine
{
    /** Base interface for texture image data. */
    class ITextureImage : public virtual IUnknown
    {
        Z_INTERFACE(ITextureImage)

        /**
         * Retrieves number of faces in the texture.
         * Most implementations of this method will return `6` for cube map textures and `1` for all other textures.
         * @return Number of faces in the texture.
         */
        virtual size_t faceCount() const = 0;

        /**
         * Retrieves number of mipmap levels for the specified face of the texture.
         * @param faceIndex Index of a face.
         * @return Number of mipmap levels.
         */
        virtual size_t mipmapLevelCount(size_t faceIndex) const = 0;

        /**
         * Retrieves data for the specified mipmap level of the specified face of the texture.
         * @param faceIndex Index of a face.
         * @param mipmapLevel Mipmap level.
         * @return Data for the specified mipmap level.
         */
        virtual IImage* mipmapLevel(size_t faceIndex, size_t mipmapLevel) const = 0;
    };
}
