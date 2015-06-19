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
#include "core/interfaces/ITextureLoader.h"
#include "core/interfaces/ITextureImage.h"
#include "core/interfaces/IFileSystem.h"
#include "core/interfaces/IInputStream.h"
#include "core/utility/debug.h"

namespace Engine
{
     /** Interface to the engine core. */
    class ICore : public IUnknown
    {
        Z_SINGLETON_INTERFACE(ICore)

      #ifdef DOXYGEN
        /**
         * Retrieves reference to the instance of the engine core.
         * @return Reference to the instance of the engine code.
         */
        static ICore& instance();
      #endif

        /**
         * Registers an additional filesystem for engine use.
         * @param filesystem Pointer to the filesystem.
         */
        virtual void registerFileSystem(const Ptr<IFileSystem>& fileSystem) = 0;

        /**
         * Retrieves a reference to the file system.
         * @return Reference to the file system.
         */
        virtual IFileSystem& fileSystem() = 0;

        /**
         * Registers texture loader.
         * @param loader Texture loader.
         */
        virtual void registerTextureLoader(const Ptr<ITextureLoader>& loader) = 0;

        /**
         * Loads texture from the specified stream.
         * @param stream Pointer to the input stream.
         * @return Loaded texture or `nullptr` if texture can't be loaded.
         */
        virtual Ptr<ITextureImage> loadTexture(IInputStream* stream) = 0;

        /**
         * Loads texture from the specified stream.
         * @param stream Pointer to the input stream.
         * @param format Texture format.
         * @return Loaded texture or `nullptr` if texture can't be loaded.
         */
        virtual Ptr<ITextureImage> loadTexture(IInputStream* stream, const std::string& format) = 0;

        /**
         * Loads texture from the specified file.
         * @param fileName Name of the file.
         * @return Loaded texture or `nullptr` if texture can't be loaded.
         */
        virtual Ptr<ITextureImage> loadTexture(const std::string& fileName) = 0;
    };
}
