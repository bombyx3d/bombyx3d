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
    static const float PIXEL_PERFECTNESS_OFFSET = 0.375f;

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
        drawTexturedQuad(sprite->trimmedQuad() + position, sprite->textureCoordinates(), sprite->texture(), z);
    }

    void Canvas::drawWireframeQuad(const Quad& quad, float z, const glm::vec4& colorVal)
    {
        setTexture(nullptr);
        begin(PrimitiveType::Lines);
            color(colorVal);
            texCoord(0.0f, 0.0f);

            auto i1 = vertex(quad.topLeft + PIXEL_PERFECTNESS_OFFSET, z);                   // Edge #1
            auto i2 = vertex(quad.topRight + PIXEL_PERFECTNESS_OFFSET, z);
            index(i2); auto i3 = vertex(quad.bottomRight + PIXEL_PERFECTNESS_OFFSET, z);    // Edge #2
            index(i3); auto i4 = vertex(quad.bottomLeft + PIXEL_PERFECTNESS_OFFSET, z);     // Edge #3
            index(i4); index(i1);                                                           // Edge #4
        end();
    }

    void Canvas::drawTexturedQuad(const Quad& quad, const Quad& texCoords, const TexturePtr& texture, float z)
    {
        if (!texture)
            return;
        setTexture(texture);

        begin(PrimitiveType::Triangles);
            color(glm::vec4(1.0f));

            // Triangle #1
            texCoord(texCoords.topLeft); vertex(quad.topLeft + PIXEL_PERFECTNESS_OFFSET, z);
            texCoord(texCoords.topRight); auto i2 = vertex(quad.topRight + PIXEL_PERFECTNESS_OFFSET, z);
            texCoord(texCoords.bottomLeft); auto i3 = vertex(quad.bottomLeft + PIXEL_PERFECTNESS_OFFSET, z);

            // Triangle #2
            index(i3);
            index(i2);
            texCoord(texCoords.bottomRight); vertex(quad.bottomRight + PIXEL_PERFECTNESS_OFFSET, z);
        end();
    }

    void Canvas::drawWireframeBoundingBox(const BoundingBox& box, const glm::vec4& colorVal)
    {
        const auto& mn = box.min;
        const auto& mx = box.max;

        setTexture(nullptr);
        begin(PrimitiveType::Lines);
            color(colorVal);
            texCoord(0.0f, 0.0f);

            // -X
            auto i1 = vertex(mn.x, mn.y, mn.z); auto i2 = vertex(mn.x, mx.y, mn.z);     // Edge #1
            index(i2); auto i3 = vertex(mn.x, mx.y, mx.z);                              // Edge #2
            index(i3); auto i4 = vertex(mn.x, mn.y, mx.z);                              // Edge #3
            index(i4); index(i1);                                                       // Edge #4

            // +X
            auto i5 = vertex(mx.x, mn.y, mn.z); auto i6 = vertex(mx.x, mx.y, mn.z);     // Edge #1
            index(i6); auto i7 = vertex(mx.x, mx.y, mx.z);                              // Edge #2
            index(i7); auto i8 = vertex(mx.x, mn.y, mx.z);                              // Edge #3
            index(i8); index(i5);                                                       // Edge #4

            // -Z
            index(i1); index(i5);
            index(i6); index(i2);

            // +Z
            index(i4); index(i8);
            index(i7); index(i3);

        end();
    }
}
