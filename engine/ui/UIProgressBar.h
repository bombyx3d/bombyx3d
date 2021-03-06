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
    class UIProgressBar : public UIElement
    {
    public:
        UIProgressBar();
        ~UIProgressBar();

        Direction direction() const { return mDirection; }
        void setDirection(Direction direction) { mDirection = direction; }

        const SpritePtr& background() const { return mBackground; }
        const SpritePtr& filler() const { return mFiller; }
        const SpritePtr& overlay() const { return mOverlay; }

        void setBackground(const SpritePtr& background) { mBackground = background; }
        void setFiller(const SpritePtr& filler) { mFiller = filler; }
        void setOverlay(const SpritePtr& overlay) { mOverlay = overlay; }

        float progress() const { return mProgress; }
        void setProgress(float progress) { mProgress = progress; }

    protected:
        void draw(ICanvas* canvas) const override;

    private:
        SpritePtr mBackground;
        SpritePtr mFiller;
        SpritePtr mOverlay;
        Direction mDirection;
        float mProgress;

        B3D_DISABLE_COPY(UIProgressBar);
    };

    using UIProgressBarPtr = std::shared_ptr<UIProgressBar>;
}
