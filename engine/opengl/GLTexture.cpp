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
#include "utility/debug.h"

namespace Z
{
    GLTexture::GLTexture(GL::Enum type)
        : m_Type(type)
    {
        gl::GenTextures(1, &m_Handle);
    }

    GLTexture::~GLTexture()
    {
        gl::DeleteTextures(1, &m_Handle);
    }

    bool GLTexture::bind()
    {
        if (m_Handle == 0)
            return false;

        gl::BindTexture(m_Type, m_Handle);
        if (m_Dirty) {
            gl::TexParameteri(m_Type, GL::TEXTURE_MIN_FILTER, m_MinFilter);
            gl::TexParameteri(m_Type, GL::TEXTURE_MAG_FILTER, m_MagFilter);
            gl::TexParameteri(m_Type, GL::TEXTURE_WRAP_S, m_WrapS);
            gl::TexParameteri(m_Type, GL::TEXTURE_WRAP_T, m_WrapT);
            m_Dirty = false;
        }

        return true;
    }
}
