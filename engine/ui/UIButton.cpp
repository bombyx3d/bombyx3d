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
#include "UIButton.h"
#include "engine/ui/UIImage.h"
#include "engine/core/Log.h"

namespace B3D
{
    UIButton::UIButton()
    {
    }

    UIButton::~UIButton()
    {
    }

    void UIButton::setNormal(const ScenePtr& scene)
    {
        mNormal = scene;
        // FIXME: relayout parent
        setSize(mNormal->size());
    }

    void UIButton::setNormal(ScenePtr&& scene)
    {
        mNormal = std::move(scene);
        // FIXME: relayout parent
        setSize(mNormal->size());
    }

    void UIButton::setNormal(const SpritePtr& sprite)
    {
        setNormal(std::make_shared<UIImage>(sprite));
    }

    void UIButton::setNormal(SpritePtr&& sprite)
    {
        setNormal(std::make_shared<UIImage>(std::move(sprite)));
    }

    void UIButton::setPressed(const ScenePtr& scene)
    {
        mPressed = scene;
        // FIXME: relayout parent
    }

    void UIButton::setPressed(ScenePtr&& scene)
    {
        mPressed = std::move(scene);
        // FIXME: relayout parent
    }

    void UIButton::setPressed(const SpritePtr& sprite)
    {
        setPressed(std::make_shared<UIImage>(sprite));
    }

    void UIButton::setPressed(SpritePtr&& sprite)
    {
        setPressed(std::make_shared<UIImage>(std::move(sprite)));
    }

    void UIButton::onSizeChanged(const glm::vec2& newSize)
    {
        if (mNormal)
            mNormal->setSize(newSize);
        if (mPressed)
            mPressed->setSize(newSize);
    }

    void UIButton::update(double time)
    {
        if (mNormal)
            mNormal->performUpdate(time);
        if (mPressed)
            mPressed->performUpdate(time);
    }

    void UIButton::draw(ICanvas* canvas) const
    {
        if (mFingersInside.empty()) {
            if (mNormal)
                mNormal->performDraw(canvas);
        } else {
            if (mPressed)
                mPressed->performDraw(canvas);
        }
    }

    bool UIButton::onTouchBegan(int fingerIndex, const glm::vec2& position)
    {
        if (isTouchInside(position)) {
            ++mFingersDown;
            mFingersInside.insert(fingerIndex);
            return true;
        }
        return false;
    }

    void UIButton::onTouchMoved(int fingerIndex, const glm::vec2& position)
    {
        if (isTouchInside(position))
            mFingersInside.insert(fingerIndex);
        else
            mFingersInside.erase(fingerIndex);
    }

    void UIButton::onTouchEnded(int fingerIndex, const glm::vec2& position)
    {
        if (--mFingersDown > 0)
            mFingersInside.erase(fingerIndex);
        else {
            auto it = mFingersInside.find(fingerIndex);
            if (it != mFingersInside.end()) {
                mFingersInside.erase(it);
                B3D_LOGI("PRESS!");
            }
        }
    }

    void UIButton::onTouchCancelled(int fingerIndex, const glm::vec2& position)
    {
        mFingersInside.erase(fingerIndex);
        --mFingersDown;
    }
}
