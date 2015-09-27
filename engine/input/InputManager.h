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
#include "engine/utility/ObserverList.h"
#include "engine/interfaces/input/IInputManager.h"
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace Engine
{
    class InputManager : public IInputManager
    {
    public:
        InputManager();
        ~InputManager();

        void resetAll() override;
        void resetMouseButtons() override;
        void resetTouches() override;

        void addObserver(IInputObserver* observer) override;
        void removeObserver(IInputObserver* observer) override;

        bool hasKeyboard() const override;
        bool hasMouse() const override;
        bool hasTouchScreen() const override;
        bool hasMultiTouch() const override;
        bool hasAccelerometer() const override;
        void setHasKeyboard(bool flag);
        void setHasMouse(bool flag);
        void setHasTouchScreen(bool flag);
        void setHasMultiTouch(bool flag);
        void setHasAccelerometer(bool flag);

        bool emulatingTouchesFromMouse() const override;
        bool emulatingMouseFromTouches() const override;
        void setEmulateTouchesFromMouse(bool flag) override;
        void setEmulateMouseFromTouches(bool flag) override;

        glm::vec2 mousePosition() const override;

        void injectMouseButtonPress(MouseButton button) override;
        void injectMouseButtonRelease(MouseButton button) override;
        void injectMouseButtonCancel(MouseButton button) override;
        void injectMouseMove(const glm::vec2& position) override;

        void injectTouchBegin(int fingerIndex, const glm::vec2& position) override;
        void injectTouchMove(int fingerIndex, const glm::vec2& position) override;
        void injectTouchEnd(int fingerIndex, const glm::vec2& position) override;
        void injectTouchCancel(int fingerIndex, const glm::vec2& position) override;

    private:
        ObserverList<IInputObserver> mObservers;
        glm::vec2 mMousePosition;
        std::unordered_set<MouseButton> mPressedMouseButtons;
        std::unordered_map<int, glm::vec2> mPressedFingers;
        bool mHasKeyboard;
        bool mHasMouse;
        bool mHasTouchScreen;
        bool mHasMultiTouch;
        bool mHasAccelerometer;
        bool mEmulateTouchesFromMouse;
        bool mEmulateMouseFromTouches;

        void emitMouseButtonPress(MouseButton button);
        void emitMouseButtonRelease(MouseButton button);
        void emitMouseButtonCancel(MouseButton button);
        void emitMouseMove(const glm::vec2& position);

        void emitTouchBegin(int fingerIndex, const glm::vec2& position);
        void emitTouchMove(int fingerIndex, const glm::vec2& position);
        void emitTouchEnd(int fingerIndex, const glm::vec2& position);
        void emitTouchCancel(int fingerIndex, const glm::vec2& position);

        Z_DISABLE_COPY(InputManager);
    };
}
