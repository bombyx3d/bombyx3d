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
#include "GLES2Texture.h"
#include "engine/core/Services.h"
#include "opengl.h"
#include <cassert>

namespace Engine
{
    GLES2Texture::GLES2Texture()
    {
        GLuint handle = 0;
        glGenTextures(1, &handle);
        mHandle = handle;
    }

    GLES2Texture::~GLES2Texture()
    {
        GLuint handle = GLuint(mHandle);
        Services::threadManager()->performInRenderThread([handle]() {
            glDeleteTextures(1, &handle);
        });
    }

    void GLES2Texture::upload(const IImage& image)
    {
        if (!image.data())
            return;

        GLenum format = 0, type = 0;
        GLint internalFormat = 0;
        switch (image.pixelFormat())
        {
        case PixelFormat::Invalid:
            return;

        case PixelFormat::Luminance8:
            type = GL_UNSIGNED_BYTE;
            format = internalFormat = GL_LUMINANCE;
            break;

        case PixelFormat::LuminanceAlpha16:
            type = GL_UNSIGNED_BYTE;
            format = internalFormat = GL_LUMINANCE_ALPHA;
            break;

        case PixelFormat::RGB24:
            type = GL_UNSIGNED_BYTE;
            format = internalFormat = GL_RGB;
            break;

        case PixelFormat::RGBA32:
            type = GL_UNSIGNED_BYTE;
            format = internalFormat = GL_RGBA;
            break;
        }

        assert(format != 0 && internalFormat != 0 && type != 0);

        GLint previousTexture = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
        glBindTexture(GL_TEXTURE_2D, GLuint(mHandle));

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, GLsizei(image.width()), GLsizei(image.height()),
            0, format, type, image.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, GLuint(previousTexture));
    }
}
