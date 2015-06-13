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
#include "GLTexture.h"
#include "core/io/streams/FileInputStream.h"
#include "renderer/formats/ztex.h"
#include "utility/debug.h"
#include <iomanip>

namespace Z
{
    GLTexture::GLTexture()
    {
        gl::GenTextures(1, &m_Handle);
    }

    GLTexture::~GLTexture()
    {
        gl::DeleteTextures(1, &m_Handle);
    }

    bool GLTexture::bind(GL::Enum type)
    {
        if (m_Handle == 0)
            return false;

        if (type == 0) {
            if (m_Type == 0)
                return false;
            type = m_Type;
        } else if (m_Type != type) {
            m_Type = type;
            m_Dirty = true;
        }

        gl::BindTexture(type, m_Handle);
        if (m_Dirty) {
            gl::TexParameteri(type, GL::TEXTURE_MIN_FILTER, m_MinFilter);
            gl::TexParameteri(type, GL::TEXTURE_MAG_FILTER, m_MagFilter);
            gl::TexParameteri(type, GL::TEXTURE_WRAP_S, m_WrapS);
            gl::TexParameteri(type, GL::TEXTURE_WRAP_T, m_WrapT);
            m_Dirty = false;
        }

        return true;
    }

    void GLTexture::unbind()
    {
        if (m_Type != 0)
            gl::BindTexture(m_Type, 0);
    }

    void GLTexture::unbindAll()
    {
        gl::BindTexture(GL::TEXTURE_2D, 0);
        gl::BindTexture(GL::TEXTURE_CUBE_MAP, 0);
        if (gl::Supports2DArrayTextures())
            gl3::BindTexture(GL3::TEXTURE_2D_ARRAY, 0);
        if (gl::Supports3DTextures())
            gl3::BindTexture(GL3::TEXTURE_3D, 0);
    }

    bool GLTexture::load(const std::string& file)
    {
        Ptr<IFileReader> reader = FileSystem::defaultFileSystem()->openFile(file);
        if (!reader)
            return false;
        return load(reader);
    }

    bool GLTexture::load(const Ptr<IFileReader>& fileReader)
    {
        FileInputStream stream(fileReader);
        return load(&stream);
    }

    bool GLTexture::load(const Ptr<IInputStream>& inputStream)
    {
        return load(inputStream.get());
    }

    bool GLTexture::load(IInputStream* inputStream)
    {
        Z_CHECK(inputStream != nullptr);
        if (!inputStream)
            return false;

        Z_LOG("Loading texture \"" << inputStream->name() << "\".");
        std::vector<char> data = inputStream->readAll();

        ZTexHeader* header = reinterpret_cast<ZTexHeader*>(data.data());
        header->faces = reinterpret_cast<ZTexFace*>(data.data() + header->facesOffset);
        if (header->magic != ZTEX_MAGIC) {
            Z_LOG(inputStream->name() << ": not a valid texture file.");
            return false;
        }

        bool isCubeMap = (header->flags & ZTEXFLAG_CUBEMAP);
        size_t numFaces = (isCubeMap ? 6 : 1);
        for (size_t faceIndex = 0; faceIndex < numFaces; faceIndex++) {
            ZTexFace* face = &header->faces[faceIndex];
            face->mipmaps = reinterpret_cast<ZTexMipmap*>(data.data() + face->mipmapsOffset);

            size_t numMipmaps = face->numMipmaps;
            for (size_t mipmapIndex = 0; mipmapIndex < numMipmaps; mipmapIndex++) {
                ZTexMipmap* mipmap = &face->mipmaps[mipmapIndex];
                mipmap->formats = reinterpret_cast<ZTexMipmapFormat*>(data.data() + mipmap->formatsOffset);

                size_t numFormats = mipmap->numFormats;
                for (size_t formatIndex = 0; formatIndex < numFormats; formatIndex++) {
                    ZTexMipmapFormat* format = &mipmap->formats[formatIndex];
                    format->data = reinterpret_cast<uint8_t*>(data.data() + format->dataOffset);

                    const uint8_t* data = format->data;
                    size_t dataSize = format->dataSize;
                    if (format->internalFormat == ZTEX_COMPRESSED_JPEG_RGB) {
                        // FIXME
                        Z_LOG(inputStream->name() << ": JPEG format is not supported yet.");
                        return false;
                    }

                    if (format->format == 0 && format->type == 0) {
                        if (isCompressedFormatSupported(GL::Enum(format->internalFormat))) {
                            if (!loadCompressedImage(inputStream->name(), face->target, mipmapIndex,
                                    format->internalFormat, mipmap->width, mipmap->height, mipmap->depth,
                                    dataSize, data))
                                return false;
                            break;
                        }
                    } else {
                        if (isUncompressedFormatSupported(GL::Enum(format->format), GL::Enum(format->type))) {
                            if (!validateUncompressedDataSize(inputStream->name(), format->format, format->type,
                                    mipmap->width, mipmap->height, mipmap->depth, dataSize))
                                return false;
                            if (!loadUncompressedImage(inputStream->name(), face->target, mipmapIndex,
                                    format->internalFormat, mipmap->width, mipmap->height, mipmap->depth,
                                    format->format, format->type, data))
                                return false;
                            break;
                        }
                    }
                }
            }
        }

        return true;
    }

    bool GLTexture::isUncompressedFormatSupported(GL::Enum format, GL::Enum type)
    {
        switch (format)
        {
        case GL::ALPHA:
        case GL::RGB:
        case GL::RGBA:
        case GL::LUMINANCE:
        case GL::LUMINANCE_ALPHA:
            return true;
        }

        return false;
    }

    bool GLTexture::isCompressedFormatSupported(GL::Enum format)
    {
        switch (format)
        {
        case GL::COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
        case GL::COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
        case GL::COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
        case GL::COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
            return gl::IsExtensionSupported(GL_IMG_texture_compression_pvrtc);

        case GL::ETC1_RGB8_OES:
            return gl::IsExtensionSupported(GL_OES_compressed_ETC1_RGB8_texture);

        /*
        case GL::COMPRESSED_R11_EAC:
        case GL::COMPRESSED_SIGNED_R11_EAC:
        case GL::COMPRESSED_RG11_EAC:
        case GL::COMPRESSED_SIGNED_RG11_EAC:
        case GL::COMPRESSED_RGB8_ETC2:
        case GL::COMPRESSED_SRGB8_ETC2:
        case GL::COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
        case GL::COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
        case GL::COMPRESSED_RGBA8_ETC2_EAC:
        case GL::COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
            // FIXME
            return false;
        */

        case GL::COMPRESSED_RGBA_BPTC_UNORM_ARB:
        case GL::COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
        case GL::COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
        case GL::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
            return gl::IsExtensionSupported(GL_ARB_texture_compression_bptc);

        case GL::COMPRESSED_RGB_FXT1_3DFX:
        case GL::COMPRESSED_RGBA_FXT1_3DFX:
            return gl::IsExtensionSupported(GL_3DFX_texture_compression_FXT1);

        case GL::COMPRESSED_LUMINANCE_LATC1_EXT:
        case GL::COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT:
        case GL::COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT:
        case GL::COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT:
            return
                gl::IsExtensionSupported(GL_EXT_texture_compression_latc) ||
                gl::IsExtensionSupported(GL_NV_texture_compression_latc);

        case GL::COMPRESSED_RED_RGTC1_EXT:
        case GL::COMPRESSED_SIGNED_RED_RGTC1_EXT:
        case GL::COMPRESSED_RED_GREEN_RGTC2_EXT:
        case GL::COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT:
            return
                gl::IsExtensionSupported(GL_EXT_texture_compression_rgtc) ||
                gl::IsExtensionSupported(GL_ARB_texture_compression_rgtc);

        case GL::COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL::COMPRESSED_RGBA_S3TC_DXT1_EXT:
            return
                gl::IsExtensionSupported(GL_EXT_texture_compression_s3tc) ||
                gl::IsExtensionSupported(GL_EXT_texture_compression_dxt1) ||
                gl::IsExtensionSupported(GL_ANGLE_texture_compression_dxt1);

        case GL::COMPRESSED_RGBA_S3TC_DXT3_EXT:
            return
                gl::IsExtensionSupported(GL_EXT_texture_compression_s3tc) ||
                gl::IsExtensionSupported(GL_ANGLE_texture_compression_dxt3);

        case GL::COMPRESSED_RGBA_S3TC_DXT5_EXT:
            return
                gl::IsExtensionSupported(GL_EXT_texture_compression_s3tc) ||
                gl::IsExtensionSupported(GL_ANGLE_texture_compression_dxt5);

        case GL::COMPRESSED_RGBA_ASTC_4x4_KHR:
        case GL::COMPRESSED_RGBA_ASTC_5x4_KHR:
        case GL::COMPRESSED_RGBA_ASTC_5x5_KHR:
        case GL::COMPRESSED_RGBA_ASTC_6x5_KHR:
        case GL::COMPRESSED_RGBA_ASTC_6x6_KHR:
        case GL::COMPRESSED_RGBA_ASTC_8x5_KHR:
        case GL::COMPRESSED_RGBA_ASTC_8x6_KHR:
        case GL::COMPRESSED_RGBA_ASTC_8x8_KHR:
        case GL::COMPRESSED_RGBA_ASTC_10x5_KHR:
        case GL::COMPRESSED_RGBA_ASTC_10x6_KHR:
        case GL::COMPRESSED_RGBA_ASTC_10x8_KHR:
        case GL::COMPRESSED_RGBA_ASTC_10x10_KHR:
        case GL::COMPRESSED_RGBA_ASTC_12x10_KHR:
        case GL::COMPRESSED_RGBA_ASTC_12x12_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR:
        case GL::COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR:
            return
                gl::IsExtensionSupported(GL_KHR_texture_compression_astc_ldr) ||
                gl::IsExtensionSupported(GL_KHR_texture_compression_astc_hdr);
        }

        return false;
    }

    bool GLTexture::validateUncompressedDataSize(const std::string& name, uint32_t format, uint32_t type,
        uint32_t width, uint32_t height, uint32_t depth, size_t dataSize)
    {
        size_t numChannels = 0;
        switch (format)
        {
        case GL::ALPHA: numChannels = 1; break;
        case GL::LUMINANCE: numChannels = 1; break;
        case GL::LUMINANCE_ALPHA: numChannels = 2; break;
        case GL::RGB: numChannels = 3; break;
        case GL::RGBA: numChannels = 4; break;
        default:
            Z_LOG(name << ": unsupported format 0x" << std::hex << std::setw(8) << std::setfill('0') << format << ".");
            return false;
        }

        size_t bytesPerPixel = 0;
        switch (type)
        {
        case GL::BYTE: bytesPerPixel = numChannels; break;
        case GL::UNSIGNED_BYTE: bytesPerPixel = numChannels; break;
        case GL::SHORT: bytesPerPixel = 2 * numChannels; break;
        case GL::UNSIGNED_SHORT: bytesPerPixel = 2 * numChannels; break;
        case GL::INT: bytesPerPixel = 4 * numChannels; break;
        case GL::UNSIGNED_INT: bytesPerPixel = 4 * numChannels; break;
        case GL::FLOAT: bytesPerPixel = 4 * numChannels; break;

        case GL::UNSIGNED_SHORT_4_4_4_4:
            if (format != GL::RGBA) {
                Z_LOG(name << ": type GL::UNSIGNED_SHORT_4_4_4_4 can only be used with format GL::RGBA.");
                return false;
            }
            bytesPerPixel = 2;
            break;

        case GL::UNSIGNED_SHORT_5_5_5_1:
            if (format != GL::RGBA) {
                Z_LOG(name << ": type GL::UNSIGNED_SHORT_5_5_5_1 can only be used with format GL::RGBA.");
                return false;
            }
            bytesPerPixel = 2;
            break;

        case GL::UNSIGNED_SHORT_5_6_5:
            if (format != GL::RGB) {
                Z_LOG(name << ": type GL::UNSIGNED_SHORT_5_6_5 can only be used with format GL::RGBA.");
                return false;
            }
            bytesPerPixel = 2;
            break;

        default:
            Z_LOG(name << ": unsupported type 0x" << std::hex << std::setw(8) << std::setfill('0') << type << ".");
            return false;
        }

        size_t expectedSize = width * height * depth * bytesPerPixel;
        if (expectedSize != dataSize) {
            Z_LOG(name << ": invalid data size (expected " << expectedSize << ", got " << dataSize << ").");
            return false;
        }

        return true;
    }

    bool GLTexture::loadUncompressedImage(const std::string& name, uint32_t target, size_t level, uint32_t internalFormat,
        uint32_t width, uint32_t height, uint32_t depth, uint32_t format, uint32_t type, const void* data)
    {
        gl::PixelStorei(GL::UNPACK_ALIGNMENT, 1);

        switch (target)
        {
        case GL::TEXTURE_2D:
            bind(GL::Enum(target));
            gl::TexImage2D(GL::Enum(target), GL::Int(level), GL::Enum(internalFormat),
                GL::Sizei(width), GL::Sizei(height), 0, GL::Enum(format), GL::Enum(type), data);
            if (level == 0) {
                m_Width = int(width);
                m_Height = int(height);
                m_Depth = 1;
            }
            return true;

        case GL::TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL::TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL::TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL::TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL::TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL::TEXTURE_CUBE_MAP_NEGATIVE_Z:
            bind(GL::TEXTURE_CUBE_MAP);
            gl::TexImage2D(GL::Enum(target), GL::Int(level), GL::Enum(internalFormat),
                GL::Sizei(width), GL::Sizei(height), 0, GL::Enum(format), GL::Enum(type), data);
            if (level == 0) {
                m_Width = int(width);
                m_Height = int(height);
                m_Depth = 6;
            }
            return true;

        case GL3::TEXTURE_3D:
        case GL3::TEXTURE_2D_ARRAY:
            bind(GL3::Enum(target));
            gl3::TexImage3D(GL::Enum(target), GL::Int(level), GL::Enum(internalFormat),
                GL::Sizei(width), GL::Sizei(height), GL::Sizei(depth), 0, GL::Enum(format), GL::Enum(type), data);
            if (level == 0) {
                m_Width = int(width);
                m_Height = int(height);
                m_Depth = int(depth);
            }
            return true;
        }

        Z_LOG(name << ": invalid texture target 0x"
            << std::hex << std::setw(8) << std::setfill('0') << target << ".");
        return false;
    }

    bool GLTexture::loadCompressedImage(const std::string& name, uint32_t target, size_t level, uint32_t internalFormat,
        uint32_t width, uint32_t height, uint32_t depth, size_t dataSize, const void* data)
    {
        switch (target)
        {
        case GL::TEXTURE_2D:
            bind(GL::Enum(target));
            gl::CompressedTexImage2D(GL::Enum(target), GL::Int(level), GL::Enum(internalFormat),
                GL::Sizei(width), GL::Sizei(height), 0, GL::Sizei(dataSize), data);
            if (level == 0) {
                m_Width = int(width);
                m_Height = int(height);
                m_Depth = 1;
            }
            return true;

        case GL::TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL::TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL::TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL::TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL::TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL::TEXTURE_CUBE_MAP_NEGATIVE_Z:
            bind(GL::TEXTURE_CUBE_MAP);
            gl::CompressedTexImage2D(GL::Enum(target), GL::Int(level), GL::Enum(internalFormat),
                GL::Sizei(width), GL::Sizei(height), 0, GL::Sizei(dataSize), data);
            if (level == 0) {
                m_Width = int(width);
                m_Height = int(height);
                m_Depth = 6;
            }
            return true;

        case GL3::TEXTURE_3D:
        case GL3::TEXTURE_2D_ARRAY:
            bind(GL3::Enum(target));
            gl3::CompressedTexImage3D(GL::Enum(target), GL::Int(level), GL::Enum(internalFormat),
                GL::Sizei(width), GL::Sizei(height), GL::Sizei(depth), 0, GL::Sizei(dataSize), data);
            if (level == 0) {
                m_Width = int(width);
                m_Height = int(height);
                m_Depth = int(depth);
            }
            return true;
        }

        Z_LOG(name << ": invalid texture target 0x"
            << std::hex << std::setw(8) << std::setfill('0') << target << ".");
        return false;
    }
}
