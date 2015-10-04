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
#include "UIAbsoluteLayout.h"

namespace B3D
{
    UIAbsoluteLayout::UIAbsoluteLayout()
    {
    }

    UIAbsoluteLayout::~UIAbsoluteLayout()
    {
    }

    void UIAbsoluteLayout::setTransform(size_t index, const glm::vec2& position)
    {
        setTransform(index, AffineTransform::translation(position));
    }

    void UIAbsoluteLayout::setTransform(size_t index, float x, float y)
    {
        setTransform(index, AffineTransform::translation(x, y));
    }

    void UIAbsoluteLayout::setTransform(size_t index, const AffineTransform& transform)
    {
        ensureIndex(index);
        mTransforms[index] = transform;
        mInverseTransformsValid[index] = false;
    }

    const AffineTransform& UIAbsoluteLayout::transform(size_t index)
    {
        ensureIndex(index);
        return mTransforms[index];
    }

    const AffineTransform& UIAbsoluteLayout::inverseTransform(size_t index)
    {
        ensureIndex(index);
        if (!mInverseTransformsValid[index]) {
            mInverseTransforms[index] = mTransforms[index].inverse();
            mInverseTransformsValid[index] = true;
        }
        return mInverseTransforms[index];
    }

    void UIAbsoluteLayout::beginLayout(size_t elementCount, const glm::vec2&)
    {
        if (elementCount > 0)
            ensureIndex(elementCount - 1);
    }

    void UIAbsoluteLayout::layoutElement(size_t, const ScenePtr&, const glm::vec2&)
    {
    }

    void UIAbsoluteLayout::onBeforeDrawElement(size_t index, const ScenePtr&, ICanvas* canvas)
    {
        canvas->pushModelViewMatrix();
        canvas->setModelViewMatrix(canvas->modelViewMatrix() * transform(index).toMat4());
    }

    void UIAbsoluteLayout::onAfterDrawElement(size_t, const ScenePtr&, ICanvas* canvas)
    {
        canvas->popModelViewMatrix();
    }

    void UIAbsoluteLayout::adjustTouchPositionForElement(size_t index, const ScenePtr&, glm::vec2& position)
    {
        position = inverseTransform(index).transform(position);
    }

    void UIAbsoluteLayout::ensureIndex(size_t index)
    {
        if (index >= mTransforms.size()) {
            mTransforms.resize(index + 1);
            mInverseTransforms.resize(index + 1);
            mInverseTransformsValid.resize(index + 1);
        }
    }
}
