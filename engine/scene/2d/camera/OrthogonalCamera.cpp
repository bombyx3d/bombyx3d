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
#include "OrthogonalCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    OrthogonalCamera::OrthogonalCamera(const glm::vec2& virtResolution, AspectRatio ratio)
        : mVirtualResolution(virtResolution)
        , mPhysicalResolution(virtResolution)
        , mPosition(0.0f)
        , mNearZ(-1.0f)
        , mFarZ(1.0f)
        , mAspectRatio(ratio)
    {
    }

    OrthogonalCamera::~OrthogonalCamera()
    {
    }

    const Quad& OrthogonalCamera::visibleArea() const
    {
        projectionMatrix();     // Ensure that visible area has been calculated
        return mVisibleArea;
    }

    void OrthogonalCamera::calcProjectionMatrix(glm::mat4& matrix) const
    {
        glm::vec2 scale = mPhysicalResolution / mVirtualResolution;
        glm::vec2 virtualScale = scale;

        switch (mAspectRatio)
        {
        case AspectRatio::Ignore: break;
        case AspectRatio::Fit: virtualScale = glm::vec2(std::min(scale.x, scale.y)); break;
        case AspectRatio::Expand: virtualScale = glm::vec2(std::max(scale.x, scale.y)); break;
        }

        glm::vec2 size = mPhysicalResolution / virtualScale;
        glm::vec2 p1 = (mVirtualResolution - size) * 0.5f;
        glm::vec2 p2 = p1 + size;

        matrix = glm::ortho(p1.x, p2.x, p2.y, p1.y, -1.0f, 1.0f);
        mVisibleArea = Quad::fromTopLeftAndBottomRight(p1, p2);
    }

    void OrthogonalCamera::calcViewMatrix(glm::mat4& matrix) const
    {
        matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-mPosition, 0.0f));
    }

    void OrthogonalCamera::onSceneSizeChanged(IScene*, const glm::vec2& newSize)
    {
        setPhysicalResolution(newSize);
    }
}
