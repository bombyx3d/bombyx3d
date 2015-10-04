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
#include "UIElement.h"

namespace B3D
{
    UIElement::UIElement()
    {
    }

    UIElement::~UIElement()
    {
    }

    bool UIElement::hasChildren() const
    {
        return mChildren && mChildren->childrenCount() != 0;
    }

    ChildrenListComponent& UIElement::children()
    {
        if (!mChildren) {
            mChildren = std::make_shared<ChildrenListComponent>();
            addComponent(mChildren);
        }
        return *mChildren;
    }

    bool UIElement::isTouchInside(const glm::vec2& position) const
    {
        auto half = size() * 0.5f;
        auto p1 = -half;
        auto p2 =  half;
        return position.x >= p1.x && position.y >= p1.y && position.x <= p2.x && position.y <= p2.y;
    }
}
