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
#include <glm/glm.hpp>

namespace Engine
{
    class IApplication
    {
    public:
        static IApplication* create();
        virtual ~IApplication() = default;

        virtual glm::ivec2 preferredScreenSize() const = 0;
        virtual int preferredDepthBits() const = 0;
        virtual int preferredStencilBits() const = 0;

        virtual void initialize(const glm::ivec2& screenSize) = 0;
        virtual void shutdown() = 0;

        virtual void resize(const glm::ivec2& screenSize) = 0;

        virtual void runFrame(double time) = 0;

        virtual void onTouchBegan(int fingerIndex, const glm::ivec2& position) = 0;
        virtual void onTouchMoved(int fingerIndex, const glm::ivec2& position) = 0;
        virtual void onTouchEnded(int fingerIndex) = 0;
        virtual void onTouchCancelled(int fingerIndex) = 0;
    };
}
