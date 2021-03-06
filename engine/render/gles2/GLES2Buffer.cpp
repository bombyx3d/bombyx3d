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
#include "GLES2Buffer.h"
#include "engine/core/Services.h"
#include "opengl.h"
#include <cassert>

namespace B3D
{
    GLES2Buffer::GLES2Buffer(size_t target)
        : mSize(0)
        , mTarget(target)
    {
        GLuint handle = 0;
        glGenBuffers(1, &handle);
        mHandle = handle;
    }

    GLES2Buffer::~GLES2Buffer()
    {
        GLuint handle = GLuint(mHandle);
        Services::threadManager()->performInRenderThread([handle]() {
            glDeleteBuffers(1, &handle);
        });
    }

    size_t GLES2Buffer::currentSize() const
    {
        return mSize;
    }

    void GLES2Buffer::initEmpty(size_t size, BufferUsage usage)
    {
        if (!mHandle)
            return;
        glBindBuffer(GLenum(mTarget), GLuint(mHandle));
        glBufferData(GLenum(mTarget), GLsizeiptr(size), nullptr, bufferUsageToGL(usage));
        mSize = size;
    }

    void GLES2Buffer::setData(const void* data, size_t size, BufferUsage usage)
    {
        if (!mHandle)
            return;
        glBindBuffer(GLenum(mTarget), GLuint(mHandle));
        glBufferData(GLenum(mTarget), GLsizeiptr(size), data, bufferUsageToGL(usage));
        mSize = size;
    }
}
