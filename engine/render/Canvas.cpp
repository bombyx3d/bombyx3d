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
#include "Canvas.h"
#include "engine/math/Quad.h"

namespace Engine
{
    Canvas::Canvas()
    {
        setBlend(true);
        setDepthTest(false);
    }

    Canvas::~Canvas()
    {
    }

    void Canvas::drawSprite(const glm::vec2& position, const SpritePtr& sprite, float z)
    {
        if (!sprite)
            return;

        const TexturePtr& texture = sprite->texture();
        if (!texture)
            return;

        setTexture(texture);

        const Quad& coord = sprite->trimmedQuad();
        const Quad& tex = sprite->textureCoordinates();
        begin(PrimitiveType::Triangles);
            color(glm::vec4(1.0f));

            // Triangle #1
            texCoord(tex.topLeft); vertex(position + coord.topLeft, z);
            texCoord(tex.topRight); auto i2 = vertex(position + coord.topRight, z);
            texCoord(tex.bottomLeft); auto i3 = vertex(position + coord.bottomLeft, z);

            // Triangle #2
            index(i3);
            index(i2);
            texCoord(tex.bottomRight); vertex(position + coord.bottomRight, z);
        end();
    }
}
