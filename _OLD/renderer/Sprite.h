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
#include "Texture.h"
#include "Shader.h"
#include "core/math/Quad.h"
#include "core/math/AffineTransform.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace Z
{
    class Sprite
    {
    public:
        Sprite();
        explicit Sprite(const TexturePtr& texture, const ShaderPtr& shader = ShaderPtr());
        virtual ~Sprite();

        glm::vec2 size() const { return m_Position.bottomRight - m_Position.topLeft; }

        void draw() const;

    protected:
        TexturePtr m_Texture;
        ShaderPtr m_Shader;
        Engine::Quad m_Position;
        Engine::Quad m_TexCoords;
    };

    using SpritePtr = std::shared_ptr<Sprite>;
}
