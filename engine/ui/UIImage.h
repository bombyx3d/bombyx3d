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
#include "engine/interfaces/image/ISprite.h"
#include "engine/ui/UIElement.h"
#include "engine/math/Direction.h"

namespace B3D
{
    class UIImage : public UIElement
    {
    public:
        UIImage();
        explicit UIImage(const SpritePtr& image);
        explicit UIImage(SpritePtr&& image);
        ~UIImage();

        const SpritePtr& image() const { return mImage; }
        void setImage(const SpritePtr& image);
        void setImage(SpritePtr&& image);

    protected:
        void draw(ICanvas* canvas) const override;

    private:
        SpritePtr mImage;

        B3D_DISABLE_COPY(UIImage);
    };

    using UIImagePtr = std::shared_ptr<UIImage>;
}
