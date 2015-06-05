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
#include "Canvas.h"
#include "Engine.h"
#include "utility/debug.h"

namespace Z
{
    Canvas::Canvas()
        : m_RootElement(std::make_shared<CanvasElement>())
        , m_NumPressedPointers(0)
        , m_ClearBits(GL::COLOR_BUFFER_BIT)
        , m_ClearColor(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    Canvas::~Canvas()
    {
    }

    void Canvas::update(double time)
    {
        if (m_RootElement)
            m_RootElement->update(time);
    }

    void Canvas::draw() const
    {
        if (m_ClearBits != 0) {
            Engine::instance().renderer().setClearColor(m_ClearColor);
            Engine::instance().renderer().clear(m_ClearBits);
        }

        if (m_RootElement)
            m_RootElement->drawIfVisible();
    }

    void Canvas::sendPointerPressEvent(int id, const glm::vec2& pos)
    {
        ++m_NumPressedPointers;
        if (m_RootElement) {
            auto target = m_RootElement->sendPointerPressEvent(id, pos);
            if (target)
                m_PointerEventsTarget = target;
        }
    }

    void Canvas::sendPointerMoveEvent(int id, const glm::vec2& pos)
    {
        Z_ASSERT(m_NumPressedPointers > 0);
        auto target = m_PointerEventsTarget.lock();
        if (target)
            target->sendPointerMoveEvent(id, pos);
    }

    void Canvas::sendPointerReleaseEvent(int id, const glm::vec2& pos)
    {
        auto target = m_PointerEventsTarget.lock();
        if (target)
            target->sendPointerReleaseEvent(id, pos);

        Z_ASSERT(m_NumPressedPointers > 0);
        if (--m_NumPressedPointers == 0)
            m_PointerEventsTarget.reset();
    }

    void Canvas::sendPointerCancelEvent(int id, const glm::vec2& pos)
    {
        --m_NumPressedPointers;
        auto target = m_PointerEventsTarget.lock();
        if (target)
            target->sendPointerCancelEvent(id, pos);

        Z_ASSERT(m_NumPressedPointers > 0);
        if (--m_NumPressedPointers == 0)
            m_PointerEventsTarget.reset();
    }
}
