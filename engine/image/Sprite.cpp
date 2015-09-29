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

namespace B3D
{
    namespace
    {
        struct NullElement : public SpriteSheet::Element
        {
            NullElement()
            {
                originalQuad = Quad::allZero();
                trimmedQuad = Quad::allZero();
                textureCoordinates = Quad::allZero();
            }
        };

        static const std::shared_ptr<SpriteSheet::Element> gNullElement = std::make_shared<NullElement>();
    }

    Sprite::Sprite()
        : mElement(gNullElement)
    {
    }

    Sprite::Sprite(std::shared_ptr<SpriteSheet>&& sheet, const std::shared_ptr<SpriteSheet::Element>& element)
        : mSpriteSheet(sheet)
        , mElement(element)
    {
    }

    Sprite::~Sprite()
    {
    }

    const glm::vec2& Sprite::originalSize() const
    {
        return mElement->originalSize;
    }

    const Quad& Sprite::originalQuad() const
    {
        return mElement->originalQuad;
    }

    const Quad& Sprite::trimmedQuad() const
    {
        return mElement->trimmedQuad;
    }

    const TexturePtr& Sprite::texture() const
    {
        return mElement->texture;
    }

    const Quad& Sprite::textureCoordinates() const
    {
        return mElement->textureCoordinates;
    }
}
