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
#include <memory>

namespace Z
{
    class GLTexture
    {
    public:
        explicit GLTexture(GL::Enum type = GL::TEXTURE_2D);
        ~GLTexture();

        GL::UInt handle() const { return m_Handle; }

        bool bind();

        void setMinFilter(GL::Enum filter) { m_MinFilter = filter; m_Dirty = true; }
        void setMagFilter(GL::Enum filter) { m_MagFilter = filter; m_Dirty = true; }

        void setWrap(GL::Enum wrap) { m_WrapS = m_WrapT = wrap; m_Dirty = true; }
        void setWrapS(GL::Enum wrap) { m_WrapS = wrap; m_Dirty = true; }
        void setWrapT(GL::Enum wrap) { m_WrapT = wrap; m_Dirty = true; }

    private:
        GL::UInt m_Handle = 0;
        GL::Enum m_Type = GL::TEXTURE_2D;
        GL::Enum m_MinFilter = GL::LINEAR;
        GL::Enum m_MagFilter = GL::LINEAR;
        GL::Enum m_WrapS = GL::CLAMP_TO_EDGE;
        GL::Enum m_WrapT = GL::CLAMP_TO_EDGE;
        bool m_Dirty = true;
    };

    using GLTexturePtr = std::shared_ptr<GLTexture>;
}
