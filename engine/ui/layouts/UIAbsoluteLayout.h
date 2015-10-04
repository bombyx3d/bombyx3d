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
#include "engine/scene/AbstractLayoutStrategy.h"
#include "engine/core/macros.h"
#include "engine/math/AffineTransform.h"
#include <vector>

namespace B3D
{
    class UIAbsoluteLayout : public AbstractLayoutStrategy
    {
    public:
        UIAbsoluteLayout();
        ~UIAbsoluteLayout();

        void setTransform(size_t index, const glm::vec2& position);
        void setTransform(size_t index, float x, float y);
        void setTransform(size_t index, const AffineTransform& transform);

    protected:
        const AffineTransform& transform(size_t index);
        const AffineTransform& inverseTransform(size_t index);

        void beginLayout(size_t elementCount, const glm::vec2& parentSize) override;
        void layoutElement(size_t, const ScenePtr& element, const glm::vec2& parentSize) override;

        void onBeforeDrawElement(size_t index, const ScenePtr& element, ICanvas* canvas) final override;
        void onAfterDrawElement(size_t index, const ScenePtr& element, ICanvas* canvas) final override;

        void adjustTouchPositionForElement(size_t index, const ScenePtr& element, glm::vec2& position) final override;

    private:
        std::vector<AffineTransform> mTransforms;
        std::vector<AffineTransform> mInverseTransforms;
        std::vector<bool> mInverseTransformsValid;

        void ensureIndex(size_t index);

        B3D_DISABLE_COPY(UIAbsoluteLayout);
    };
}
