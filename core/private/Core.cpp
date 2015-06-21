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
#include "Core.h"
#include "core/io/streams/FileInputStream.h"
#include "core/utility/path.h"
#include "core/utility/string.h"
#include "core/utility/debug.h"

namespace Engine
{
    Core::Core()
    {
        m_FileSystem = new FileSystemList;
    }

    Core::~Core()
    {
        m_TextureLoaders.clear();
        m_ImageFormatConverters.clear();
        m_FileSystem.release();
    }

    void Core::registerFileSystem(const Ptr<IFileSystem>& fileSystem)
    {
        Z_CHECK(fileSystem != nullptr);
        if (fileSystem)
            m_FileSystem->add(fileSystem);
    }

    void Core::registerTextureLoader(const Ptr<ITextureLoader>& loader)
    {
        Z_CHECK(loader != nullptr);
        if (loader)
            m_TextureLoaders.emplace_back(loader);
    }

    Ptr<ITextureImage> Core::loadTextureImage(IInputStream* stream)
    {
        Z_CHECK(stream != nullptr);
        if (!stream)
            return nullptr;

        std::string format = pathGetFileNameExtension(stream->name());
        return loadTextureImage(stream, format);
    }

    Ptr<ITextureImage> Core::loadTextureImage(IInputStream* stream, const std::string& format)
    {
        Z_CHECK(stream != nullptr);
        if (!stream)
            return nullptr;

        std::string extension = stringToLowerCase(format);
        for (const auto& loader : m_TextureLoaders) {
            if (loader->supportsFormat(extension))
                return loader->loadTextureImage(stream);
        }

        Z_LOG("Unable to load texture \"" << stream->name()
            << "\": there is no texture loader for format \"" << extension << "\".");

        return nullptr;
    }

    Ptr<ITextureImage> Core::loadTextureImage(const std::string& fileName)
    {
        Ptr<IFileReader> fileReader = m_FileSystem->openFile(fileName);
        if (!fileReader)
            return nullptr;

        std::string format = pathGetFileNameExtension(fileName);

        FileInputStream fileInputStream(fileReader);
        return loadTextureImage(&fileInputStream, format);
    }

    void Core::registerImageFormatConverter(const Ptr<IImageFormatConverter>& converter)
    {
        Z_CHECK(converter != nullptr);
        if (converter)
            m_ImageFormatConverters.emplace_back(converter);
    }

    Ptr<IImage> Core::convertImageFormat(IImage* image, ImagePixelFormat targetFormat)
    {
        Z_CHECK(image != nullptr);
        if (!image)
            return nullptr;

        ImagePixelFormat inputFormat = image->pixelFormat();
        if (inputFormat == targetFormat)
            return image;

        for (const auto& converter : m_ImageFormatConverters) {
            if (converter->outputPixelFormat() == targetFormat && converter->acceptsInputPixelFormat(inputFormat))
                return converter->convertImage(image);
        }

        Z_LOG("Unable to convert image to another pixel format: there is no corresponding format converter available.");
        return nullptr;
    }
}
