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
#include "api/opengl.h"
#include "io/FileSystem.h"
#include "core/interfaces/IInputStream.h"
#include "core/interfaces/IFileReader.h"
#include <memory>

namespace Z
{
    using namespace Engine;

    class GLTexture
    {
    public:
        GLTexture();
        ~GLTexture();

        GL::UInt handle() const { return m_Handle; }

        int width() const { return m_Width; }
        int height() const { return m_Height; }
        int depth() const { return m_Depth; }

        bool bind(GL::Enum type = GL::Enum(0));
        bool bind(GL3::Enum type) { return bind(GL::Enum(type)); }
        void unbind();
        static void unbindAll();

        bool load(const std::string& file);
        bool load(const Ptr<IFileReader>& fileReader);
        bool load(const Ptr<IInputStream>& inputStream);
        bool load(IInputStream* inputStream);

        void setMinFilter(GL::Enum filter) { m_MinFilter = filter; m_Dirty = true; }
        void setMagFilter(GL::Enum filter) { m_MagFilter = filter; m_Dirty = true; }

        void setWrap(GL::Enum wrap) { m_WrapS = m_WrapT = wrap; m_Dirty = true; }
        void setWrapS(GL::Enum wrap) { m_WrapS = wrap; m_Dirty = true; }
        void setWrapT(GL::Enum wrap) { m_WrapT = wrap; m_Dirty = true; }

        static bool isUncompressedFormatSupported(GL::Enum format, GL::Enum type);
        static bool isCompressedFormatSupported(GL::Enum format);

    private:
        GL::UInt m_Handle = 0;
        GL::Enum m_Type = GL::Enum(0);
        GL::Enum m_MinFilter = GL::LINEAR;
        GL::Enum m_MagFilter = GL::LINEAR;
        GL::Enum m_WrapS = GL::CLAMP_TO_EDGE;
        GL::Enum m_WrapT = GL::CLAMP_TO_EDGE;
        int m_Width = 0;
        int m_Height = 0;
        int m_Depth = 0;
        bool m_Dirty = true;

        static bool validateUncompressedDataSize(const std::string& name, uint32_t format, uint32_t type,
            uint32_t width, uint32_t height, uint32_t depth, size_t dataSize);

        bool loadUncompressedImage(const std::string& name, uint32_t target, size_t level, uint32_t internalFormat,
            uint32_t width, uint32_t height, uint32_t depth, uint32_t format, uint32_t type, const void* data);

        bool loadCompressedImage(const std::string& name, uint32_t target, size_t level, uint32_t internalFormat,
            uint32_t width, uint32_t height, uint32_t depth, size_t dataSize, const void* data);
    };

    using GLTexturePtr = std::shared_ptr<GLTexture>;
}
