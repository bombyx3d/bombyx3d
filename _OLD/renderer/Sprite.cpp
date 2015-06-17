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
#include "Engine.h"

namespace Z
{
    Sprite::Sprite()
    {
        m_Position = ::Engine::Quad::allZero();
        m_TexCoords = ::Engine::Quad::fromZeroToOne();
    }

    Sprite::Sprite(const TexturePtr& texture, const ShaderPtr& shader)
        : m_Texture(texture)
        , m_Shader(shader)
    {
        if (!m_Shader)
            m_Shader = Engine::instance().renderer().loadShader(Renderer::DEFAULT_TEXTURED_2D_SHADER);

        glm::vec2 size = glm::vec2(float(texture->width()), float(texture->height()));
        m_Position.setCenterAndSize(glm::vec2(0.0f, 0.0f), size);
        m_TexCoords.setTopLeftAndBottomRight(0.0f, 0.0f, 1.0f, 1.0f);
    }

    Sprite::~Sprite()
    {
    }

    void Sprite::draw() const
    {
        auto& renderer = Engine::instance().renderer();
        renderer.setShader(m_Shader);
        renderer.setTexture0(m_Texture);
        renderer.drawQuad(m_Position, m_TexCoords);
    }
}
