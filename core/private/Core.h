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
#include "core/utility/Ptr.h"
#include "core/interfaces/ICore.h"
#include "core/io/FileSystemList.h"
#include <vector>

namespace Engine
{
    /** Engine core. */
    class Core : public ICore
    {
    public:
        Z_IMPLEMENTATION(Core)

        /**
         * Constructor.
         * @param fileSystemList Pointer to the list of native filesystems.
         */
        explicit Core(const Ptr<FileSystemList>& fileSystemList);

        /** Destructor. */
        ~Core();

        void registerFileSystem(const Ptr<IFileSystem>& fileSystem) override;
        IFileSystem& fileSystem() override { return *m_FileSystem; }

        void registerTextureLoader(const Ptr<ITextureLoader>& loader) override;
        Ptr<ITextureImage> loadTexture(IInputStream* stream) override;
        Ptr<ITextureImage> loadTexture(IInputStream* stream, const std::string& format) override;
        Ptr<ITextureImage> loadTexture(const std::string& fileName) override;

        void registerImageFormatConverter(const Ptr<IImageFormatConverter>& converter) override;
        Ptr<IImage> convertImageFormat(IImage* image, ImagePixelFormat targetFormat) override;

    private:
        Ptr<FileSystemList> m_FileSystem;                                   /**< Instance of the file system. */
        std::vector<Ptr<ITextureLoader>> m_TextureLoaders;                  /**< List of texture loaders. */
        std::vector<Ptr<IImageFormatConverter>> m_ImageFormatConverters;    /**< List of image format converters. */
    };
}
