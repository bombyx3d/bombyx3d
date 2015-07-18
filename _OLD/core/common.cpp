/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include "interfaces/IUnknown.h"
#include "interfaces/IBlob.h"
#include "interfaces/ICore.h"
#include "interfaces/IEvent.h"
#include "interfaces/IFileReader.h"
#include "interfaces/IFileSystem.h"
#include "interfaces/IMemoryMappedFile.h"
#include "interfaces/ISystem.h"
#include "interfaces/IStream.h"
#include "interfaces/IInputStream.h"
#include "interfaces/IImage.h"
#include "interfaces/IImageFormatConverter.h"
#include "interfaces/ITextureImage.h"
#include "interfaces/ITextureLoader.h"
#include "interfaces/ITextureSource.h"
#include "interfaces/IRenderer.h"
#include "interfaces/IRendererResource.h"
#include "interfaces/IViewport.h"
#include "interfaces/IViewportDelegate.h"
#include "image/SimpleImage.h"
#include "resource/TextureSourceFile.h"
#include "io/FileSystemList.h"
#include "io/files/MemoryFile.h"
#include "io/files/StaticMemoryFile.h"
#include "io/files/StdioFileReader.h"
#include "io/streams/StaticMemoryInputStream.h"
#include "private/Core.h"

namespace Engine
{
    void* IUnknown::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IUnknown>())
            return this;
        return nullptr;
    }

    void* IBlob::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IBlob>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* ICore::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<ICore>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IEvent::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IEvent>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IFileReader::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IFileReader>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IFileSystem::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IFileSystem>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IMemoryMappedFile::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IMemoryMappedFile>())
            return this;

        void* interface = IBlob::queryInterface(typeID);
        if (interface)
            return interface;

        return IFileReader::queryInterface(typeID);
    }

    void* ISystem::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<ISystem>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IStream::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IStream>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IInputStream::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IInputStream>())
            return this;
        return IStream::queryInterface(typeID);
    }

    void* IImage::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IImage>())
            return this;
        return IBlob::queryInterface(typeID);
    }

    void* IImageFormatConverter::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IImageFormatConverter>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* ITextureImage::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<ITextureImage>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* ITextureLoader::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<ITextureLoader>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* ITextureSource::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<ITextureSource>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IRenderer::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IRenderer>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IRendererResource::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IRendererResource>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IViewport::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IViewport>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* IViewportDelegate::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<IViewportDelegate>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* ViewportSettings::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<ViewportSettings>())
            return this;
        return IUnknown::queryInterface(typeID);
    }

    void* Core::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<Core>())
            return this;
        return ICore::queryInterface(typeID);
    }

    void* SimpleImage::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<SimpleImage>())
            return this;

        void* interface = IImage::queryInterface(typeID);
        if (interface)
            return interface;

        return ITextureImage::queryInterface(typeID);
    }

    void* TextureSourceFile::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<TextureSourceFile>())
            return this;
        return ITextureSource::queryInterface(typeID);
    }

    void* FileSystemList::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<FileSystemList>())
            return this;
        return IFileSystem::queryInterface(typeID);
    }

    void* MemoryFile::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<MemoryFile>())
            return this;
        return IMemoryMappedFile::queryInterface(typeID);
    }

    void* StaticMemoryFile::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<StaticMemoryFile>())
            return this;
        return IMemoryMappedFile::queryInterface(typeID);
    }

    void* StdioFileReader::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<StdioFileReader>())
            return this;
        return IFileReader::queryInterface(typeID);
    }

    void* StaticMemoryInputStream::queryInterface(TypeID typeID)
    {
        if (typeID == typeOf<StaticMemoryInputStream>())
            return this;

        void* interface = IInputStream::queryInterface(typeID);
        if (interface)
            return interface;

        return StaticMemoryFile::queryInterface(typeID);
    }
}
