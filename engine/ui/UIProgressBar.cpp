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
#include "UIProgressBar.h"
#include <utility>
#include <algorithm>

namespace Engine
{
    UIProgressBar::UIProgressBar()
        : mDirection(Direction::Horizontal)
        , mProgress(0.0f)
    {
    }

    UIProgressBar::~UIProgressBar()
    {
    }

    void UIProgressBar::draw(ICanvas* canvas) const
    {
        float h1 = (mBackground ? mBackground->originalSize().y : 0.0f);
        float h2 = (mFiller ? mFiller->originalSize().y : 0.0f);
        float h3 = (mOverlay ? mOverlay->originalSize().y : 0.0f);
        float height = std::max(h1, std::max(h2, h3));
        glm::vec2 pos = glm::vec2(size().x * 0.5f, size().y - 2.0f * height);

        canvas->setBlend(true);

        canvas->drawSprite(pos, mBackground);
        if (mFiller) {
            #define interpolate(from, to) ((to).x = (from).x + ((to).x - (from).x) * mProgress)
            Quad filler = mFiller->trimmedQuad() + pos;
            interpolate(filler.topLeft, filler.topRight);
            interpolate(filler.bottomLeft, filler.bottomRight);
            Quad fillerTexCoord = mFiller->textureCoordinates();
            interpolate(fillerTexCoord.topLeft, fillerTexCoord.topRight);
            interpolate(fillerTexCoord.bottomLeft, fillerTexCoord.bottomRight);
            canvas->drawTexturedQuad(filler, fillerTexCoord, mFiller->texture());
        }
        canvas->drawSprite(pos, mOverlay);
    }
}
