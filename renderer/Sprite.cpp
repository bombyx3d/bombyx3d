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
#include "Sprite.h"

namespace Z
{
    Sprite::Sprite()
        : m_Center(0.0f)
        , m_Size(0.0f)
    {
        memset(m_TexCoords, 0, sizeof(m_TexCoords));
    }

    Sprite::Sprite(const TexturePtr& texture)
    {
        m_Size = glm::vec2(float(texture->width()), float(texture->height()));
        m_Center = m_Size * 0.5f;
        m_TexCoords[0] = glm::vec2(0.0f, 0.0f);
        m_TexCoords[1] = glm::vec2(1.0f, 0.0f);
        m_TexCoords[2] = glm::vec2(0.0f, 1.0f);
        m_TexCoords[3] = glm::vec2(1.0f, 1.0f);
    }

    Sprite::~Sprite()
    {
    }
}
