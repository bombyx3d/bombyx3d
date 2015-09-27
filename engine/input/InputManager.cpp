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
#include "InputManager.h"

namespace Engine
{
    InputManager::InputManager()
        : mHasKeyboard(false)
        , mHasMouse(false)
        , mHasTouchScreen(false)
        , mHasMultiTouch(false)
        , mHasAccelerometer(false)
        , mEmulateTouchesFromMouse(true)
        , mEmulateMouseFromTouches(true)
    {
    }

    InputManager::~InputManager()
    {
    }

    void InputManager::resetAll()
    {
        resetMouseButtons();
        resetTouches();
    }

    void InputManager::resetMouseButtons()
    {
        while (!mPressedMouseButtons.empty())
            emitMouseButtonCancel(*mPressedMouseButtons.begin());
    }

    void InputManager::resetTouches()
    {
        while (!mPressedFingers.empty()) {
            auto it = mPressedFingers.begin();
            emitTouchCancel(it->first, it->second);
        }
    }

    void InputManager::addObserver(IInputObserver* observer)
    {
        mObservers.add(observer);
    }

    void InputManager::removeObserver(IInputObserver* observer)
    {
        mObservers.remove(observer);
    }

    bool InputManager::hasKeyboard() const
    {
        return mHasKeyboard;
    }

    bool InputManager::hasMouse() const
    {
        return mHasMouse;
    }

    bool InputManager::hasTouchScreen() const
    {
        return mHasTouchScreen;
    }

    bool InputManager::hasMultiTouch() const
    {
        return mHasMultiTouch;
    }

    bool InputManager::hasAccelerometer() const
    {
        return mHasAccelerometer;
    }

    void InputManager::setHasKeyboard(bool flag)
    {
        mHasKeyboard = flag;
    }

    void InputManager::setHasMouse(bool flag)
    {
        mHasMouse = flag;
    }

    void InputManager::setHasTouchScreen(bool flag)
    {
        mHasTouchScreen = flag;
    }

    void InputManager::setHasMultiTouch(bool flag)
    {
        mHasMultiTouch = flag;
    }

    void InputManager::setHasAccelerometer(bool flag)
    {
        mHasAccelerometer = flag;
    }

    bool InputManager::emulatingTouchesFromMouse() const
    {
        return mEmulateTouchesFromMouse;
    }

    bool InputManager::emulatingMouseFromTouches() const
    {
        return mEmulateMouseFromTouches;
    }

    void InputManager::setEmulateTouchesFromMouse(bool flag)
    {
        mEmulateTouchesFromMouse = flag;
        if (!flag)
            resetTouches();
    }

    void InputManager::setEmulateMouseFromTouches(bool flag)
    {
        mEmulateMouseFromTouches = flag;
        if (!flag)
            resetMouseButtons();
    }

    glm::vec2 InputManager::mousePosition() const
    {
        return mMousePosition;
    }

    void InputManager::injectMouseButtonPress(MouseButton button)
    {
        emitMouseButtonPress(button);
        if (mEmulateTouchesFromMouse && button == MouseButton::Left)
            emitTouchBegin(0, mMousePosition);
    }

    void InputManager::injectMouseButtonRelease(MouseButton button)
    {
        emitMouseButtonRelease(button);
        if (mEmulateTouchesFromMouse && button == MouseButton::Left) {
            emitTouchMove(0, mMousePosition);
            emitTouchEnd(0, mMousePosition);
        }
    }

    void InputManager::injectMouseButtonCancel(MouseButton button)
    {
        emitMouseButtonCancel(button);
        if (mEmulateTouchesFromMouse && button == MouseButton::Left) {
            emitTouchMove(0, mMousePosition);
            emitTouchCancel(0, mMousePosition);
        }
    }

    void InputManager::injectMouseMove(const glm::vec2& position)
    {
        emitMouseMove(position);
        if (mEmulateTouchesFromMouse)
            emitTouchMove(0, position);
    }

    void InputManager::injectTouchBegin(int fingerIndex, const glm::vec2& position)
    {
        emitTouchBegin(fingerIndex, position);
        if (mEmulateMouseFromTouches && fingerIndex == 0) {
            emitMouseMove(position);
            emitMouseButtonPress(MouseButton::Left);
        }
    }

    void InputManager::injectTouchMove(int fingerIndex, const glm::vec2& position)
    {
        emitTouchMove(fingerIndex, position);
        if (mEmulateMouseFromTouches && fingerIndex == 0)
            emitMouseMove(position);
    }

    void InputManager::injectTouchEnd(int fingerIndex, const glm::vec2& position)
    {
        emitTouchMove(fingerIndex, position);
        emitTouchEnd(fingerIndex, position);
        if (mEmulateMouseFromTouches && fingerIndex == 0) {
            emitMouseMove(position);
            emitMouseButtonRelease(MouseButton::Left);
        }
    }

    void InputManager::injectTouchCancel(int fingerIndex, const glm::vec2& position)
    {
        emitTouchCancel(fingerIndex, position);
        if (mEmulateMouseFromTouches && fingerIndex == 0) {
            emitMouseMove(position);
            emitMouseButtonCancel(MouseButton::Left);
        }
    }

    void InputManager::emitMouseButtonPress(MouseButton button)
    {
        if (mPressedMouseButtons.insert(button).second)
            mObservers.forEach([button](IInputObserver* observer) { observer->onMouseButtonPressed(button); });
    }

    void InputManager::emitMouseButtonRelease(MouseButton button)
    {
        auto it = mPressedMouseButtons.find(button);
        if (it != mPressedMouseButtons.end()) {
            mPressedMouseButtons.erase(it);
            mObservers.forEach([button](IInputObserver* observer) { observer->onMouseButtonReleased(button); });
        }
    }

    void InputManager::emitMouseButtonCancel(MouseButton button)
    {
        auto it = mPressedMouseButtons.find(button);
        if (it != mPressedMouseButtons.end()) {
            mPressedMouseButtons.erase(it);
            mObservers.forEach([button](IInputObserver* observer) { observer->onMouseButtonCancelled(button); });
        }
    }

    void InputManager::emitMouseMove(const glm::vec2& position)
    {
        mMousePosition = position;
        mObservers.forEach([&position](IInputObserver* observer) { observer->onMouseMoved(position); });
    }

    void InputManager::emitTouchBegin(int fingerIndex, const glm::vec2& position)
    {
        if (mPressedFingers.emplace(fingerIndex, position).second) {
            mObservers.forEach([fingerIndex, &position](IInputObserver* observer) {
                 observer->onTouchBegan(fingerIndex, position);
            });
        }
    }

    void InputManager::emitTouchMove(int fingerIndex, const glm::vec2& position)
    {
        auto it = mPressedFingers.find(fingerIndex);
        if (it != mPressedFingers.end()) {
            it->second = position;
            mObservers.forEach([fingerIndex, &position](IInputObserver* observer) {
                 observer->onTouchMoved(fingerIndex, position);
            });
        }
    }

    void InputManager::emitTouchEnd(int fingerIndex, const glm::vec2& position)
    {
        auto it = mPressedFingers.find(fingerIndex);
        if (it != mPressedFingers.end()) {
            mPressedFingers.erase(it);
            mObservers.forEach([fingerIndex, &position](IInputObserver* observer) {
                 observer->onTouchEnded(fingerIndex, position);
            });
        }
    }

    void InputManager::emitTouchCancel(int fingerIndex, const glm::vec2& position)
    {
        auto it = mPressedFingers.find(fingerIndex);
        if (it != mPressedFingers.end()) {
            mPressedFingers.erase(it);
            mObservers.forEach([fingerIndex, &position](IInputObserver* observer) {
                 observer->onTouchCancelled(fingerIndex, position);
            });
        }
    }
}
