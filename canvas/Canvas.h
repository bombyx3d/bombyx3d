/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include "CanvasElement.h"
#include "renderer/opengl/api/opengl.h"
#include <glm/glm.hpp>
#include <memory>

namespace Z
{
    class Canvas;
    using CanvasPtr = std::shared_ptr<Canvas>;

    class Canvas
    {
    public:
        Canvas();
        ~Canvas();

        GL::Bitfield clearBits() const { return m_ClearBits; }
        void setClearBits(GL::Bitfield bits) { m_ClearBits = bits; }

        const glm::vec4& clearColor() const { return m_ClearColor; }
        void setClearColor(const glm::vec4& clearColor) { m_ClearColor = clearColor; }

        const CanvasElementPtr& rootElement() const { return m_RootElement; }
        void setRootElement(const CanvasElementPtr& element) { m_RootElement = element; }

        void update(double time);
        void draw() const;

        void sendPointerPressEvent(int id, const glm::vec2& pos);
        void sendPointerMoveEvent(int id, const glm::vec2& pos);
        void sendPointerReleaseEvent(int id, const glm::vec2& pos);
        void sendPointerCancelEvent(int id, const glm::vec2& pos);

    private:
        CanvasElementPtr m_RootElement;
        std::weak_ptr<CanvasElement> m_PointerEventsTarget;
        size_t m_NumPressedPointers;
        glm::vec4 m_ClearColor;
        GL::Bitfield m_ClearBits;

        Canvas(const Canvas&) = delete;
        Canvas& operator=(const Canvas&) = delete;
    };
}
