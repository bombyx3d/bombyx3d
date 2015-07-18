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
#include "core/interfaces/ITextureImage.h"
#include "core/interfaces/IInputStream.h"
#include "core/utility/Ptr.h"

namespace Engine
{
    /**
     * Base interface for texture image loaders.
     *
     * @see
     * - @ref _textures;
     * - @ref ICore::registerTextureLoader.
     */
    class ITextureLoader : public IUnknown
    {
    public:
        Z_INTERFACE(ITextureLoader)

        /**
         * Checks whether this loader can load textures of the specified format.
         * @param extension File extension of the format (using lower-case symbols).
         * @return `true` if format is supported by this loader or `false` if it is not.
         */
        virtual bool supportsFormat(const std::string& extension) const = 0;

        /**
         * Attempts to load texture image from the provided input stream.
         * @param stream Pointer to the input stream.
         * @return Pointer to the texture or `nullptr` if texture can't be loaded.
         */
        virtual Ptr<ITextureImage> loadTextureImage(IInputStream* stream) const = 0;
    };
}
