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
#include "engine/ui/UIElement.h"
#include <unordered_set>

namespace B3D
{
    class UIButton : public UIElement
    {
    public:
        UIButton();
        ~UIButton();

        const ScenePtr& normal() const { return mNormal; }
        void setNormal(const ScenePtr& scene);
        void setNormal(ScenePtr&& scene);
        void setNormal(const SpritePtr& sprite);
        void setNormal(SpritePtr&& sprite);

        const ScenePtr& pressed() const { return mPressed; }
        void setPressed(const ScenePtr& scene);
        void setPressed(ScenePtr&& scene);
        void setPressed(const SpritePtr& sprite);
        void setPressed(SpritePtr&& sprite);

    protected:
        void onSizeChanged(const glm::vec2& newSize) override;

        void update(double time) override;
        void draw(ICanvas* canvas) const override;

        bool onTouchBegan(int fingerIndex, const glm::vec2& position) override;
        void onTouchMoved(int fingerIndex, const glm::vec2& position) override;
        void onTouchEnded(int fingerIndex, const glm::vec2& position) override;
        void onTouchCancelled(int fingerIndex, const glm::vec2& position) override;

    public:
        ScenePtr mNormal;
        ScenePtr mPressed;
        int mFingersDown = 0;
        std::unordered_set<int> mFingersInside;

        B3D_DISABLE_COPY(UIButton);
    };
}
