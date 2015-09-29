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
#include "engine/interfaces/input/IInputObserver.h"
#include <memory>
#include <glm/glm.hpp>

namespace B3D
{
    class IInputManager
    {
    public:
        virtual ~IInputManager() = default;

        virtual void resetAll() = 0;
        virtual void resetMouseButtons() = 0;
        virtual void resetTouches() = 0;

        virtual void addObserver(IInputObserver* observer) = 0;
        virtual void removeObserver(IInputObserver* observer) = 0;

        virtual bool hasKeyboard() const = 0;
        virtual bool hasMouse() const = 0;
        virtual bool hasTouchScreen() const = 0;
        virtual bool hasMultiTouch() const = 0;
        virtual bool hasAccelerometer() const = 0;

        virtual bool emulatingTouchesFromMouse() const = 0;
        virtual bool emulatingMouseFromTouches() const = 0;
        virtual void setEmulateTouchesFromMouse(bool flag) = 0;
        virtual void setEmulateMouseFromTouches(bool flag) = 0;

        virtual glm::vec2 mousePosition() const = 0;

        virtual void injectMouseButtonPress(MouseButton button) = 0;
        virtual void injectMouseButtonRelease(MouseButton button) = 0;
        virtual void injectMouseButtonCancel(MouseButton button) = 0;
        virtual void injectMouseMove(const glm::vec2& position) = 0;

        virtual void injectTouchBegin(int fingerIndex, const glm::vec2& position) = 0;
        virtual void injectTouchMove(int fingerIndex, const glm::vec2& position) = 0;
        virtual void injectTouchEnd(int fingerIndex, const glm::vec2& position) = 0;
        virtual void injectTouchCancel(int fingerIndex, const glm::vec2& position) = 0;
    };

    using InputManagerPtr = std::shared_ptr<IInputManager>;
}
