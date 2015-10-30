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
#include "engine/core/Event.h"

namespace B3D
{
    static const double PRESSED_TIME = 0.1;

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
        if (mPressedTime > 0.0)
            mPressedTime -= time;
        else if (mClickPending) {
            mClickPending = false;
            Event<Clicked> event;
            sendEvent(&event);
        }

        if (mNormal)
            mNormal->performUpdate(time);
        if (mPressed)
            mPressed->performUpdate(time);
    }

    void UIButton::draw(ICanvas* canvas) const
    {
        if (mFingersInside.empty() && !mClickPending) {
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
            beginTrackFinger(fingerIndex);
            return true;
        }
        return false;
    }

    void UIButton::onTouchMoved(int fingerIndex, const glm::vec2& position)
    {
        if (isTouchInside(position))
            beginTrackFinger(fingerIndex);
        else
            endTrackFinger(fingerIndex);
    }

    void UIButton::onTouchEnded(int fingerIndex, const glm::vec2&)
    {
        --mFingersDown;
        endTrackFinger(fingerIndex, mFingersDown == 0);
    }

    void UIButton::onTouchCancelled(int fingerIndex, const glm::vec2&)
    {
        endTrackFinger(fingerIndex);
    }

    void UIButton::animatePress()
    {
        mPressedTime = PRESSED_TIME;
    }

    void UIButton::animateRelease(bool isClick)
    {
        if (isClick)
            mClickPending = true;
    }

    void UIButton::beginTrackFinger(int fingerIndex)
    {
        bool wasReleased = mFingersInside.empty();
        mFingersInside.insert(fingerIndex);

        if (wasReleased)
            animatePress();
    }

    void UIButton::endTrackFinger(int fingerIndex, bool isClick)
    {
        bool wasReleased = mFingersInside.empty();
        mFingersInside.erase(fingerIndex);
        bool nowReleased = mFingersInside.empty();

        if (!wasReleased && nowReleased)
            animateRelease(isClick);
    }
}
