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
#include "AbstractLayoutStrategy.h"

namespace B3D
{
    void AbstractLayoutStrategy::beginLayout(size_t, const glm::vec2&)
    {
    }

    void AbstractLayoutStrategy::measureElement(size_t, const ScenePtr&, const glm::vec2&)
    {
    }

    void AbstractLayoutStrategy::layoutElement(size_t, const ScenePtr& element, const glm::vec2& parentSize)
    {
        element->setSize(parentSize);
    }

    void AbstractLayoutStrategy::endLayout(const glm::vec2&)
    {
    }

    void AbstractLayoutStrategy::onBeforeDrawElement(size_t, const ScenePtr&, ICanvas*)
    {
    }

    void AbstractLayoutStrategy::onAfterDrawElement(size_t, const ScenePtr&, ICanvas*)
    {
    }

    void AbstractLayoutStrategy::adjustTouchPositionForElement(size_t, const ScenePtr&, glm::vec2&)
    {
    }
}
