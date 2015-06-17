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
#include "GLBuffer.h"
#include "core/utility/debug.h"

namespace Z
{
    GLBuffer::GLBuffer(GL::Enum type)
        : m_Type(type)
    {
        gl::GenBuffers(1, &m_Handle);
    }

    GLBuffer::~GLBuffer()
    {
        gl::DeleteBuffers(1, &m_Handle);
    }

    bool GLBuffer::bind()
    {
        gl::BindBuffer(m_Type, m_Handle);
        return m_Handle != 0;
    }

    void GLBuffer::setData(const void* data, size_t size, GL::Enum usage)
    {
        if (bind())
            gl::BufferData(m_Type, GL::Sizeiptr(size), data, usage);
    }
}
