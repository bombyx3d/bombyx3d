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
#include "engine/core/macros.h"
#include "engine/render/ImmediateModeRenderer.h"
#include "engine/interfaces/render/ICanvas.h"

namespace B3D
{
    class Canvas : public ImmediateModeRenderer
    {
    public:
        explicit Canvas(const RendererPtr& renderer);
        ~Canvas();

        void drawSprite(const glm::vec2& position, const SpritePtr& sprite, float z = 0.0f) override;

        void drawWireframeQuad(const Quad& quad, float z = 0.0f, const glm::vec4& colorVal = glm::vec4(1.0f)) override;
        void drawTexturedQuad(const Quad& quad, const Quad& tc, const TexturePtr& texture, float z = 0.0f) override;

        void drawWireframeBoundingBox(const BoundingBox& box, const glm::vec4& colorVal = glm::vec4(1.0f)) override;

    private:
        B3D_DISABLE_COPY(Canvas);
    };
}
