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
#include "engine/scene/3d/camera/AbstractCamera.h"
#include <glm/glm.hpp>

namespace Engine
{
    class AbstractOrthogonalCamera : public AbstractCamera
    {
    public:
        AbstractOrthogonalCamera();
        ~AbstractOrthogonalCamera();

        const glm::vec2& topLeft() const { return mTopLeft; }
        const glm::vec2& bottomRight() const { return mBottomRight; }

        float nearZ() const { return mNearZ; }
        float farZ() const { return mFarZ; }

        void setTopLeft(float x, float y) { mTopLeft.x = x; mTopLeft.y = y; setProjectionMatrixDirty(); }
        void setTopLeft(const glm::vec2& p) { mTopLeft = p; setProjectionMatrixDirty(); }
        void setBottomRight(float x, float y) { mBottomRight.x = x; mBottomRight.y = y; setProjectionMatrixDirty(); }
        void setBottomRight(const glm::vec2& p) { mBottomRight = p; setProjectionMatrixDirty(); }
        void setDimensions(const glm::vec2& p1, const glm::vec2& p2)
            { mTopLeft = p1; mBottomRight = p2; setProjectionMatrixDirty(); }
        void setDimensions(float x1, float y1, float x2, float y2)
            { mTopLeft = glm::vec2(x1, y1); mBottomRight = glm::vec2(x2, y2); setProjectionMatrixDirty(); }

        void setNearZ(float value) { mNearZ = value; setProjectionMatrixDirty(); }
        void setFarZ(float value) { mFarZ = value; setProjectionMatrixDirty(); }
        void setDepthRange(float nz, float fz) { mNearZ = nz; mFarZ = fz; setProjectionMatrixDirty(); }

    protected:
        void calcProjectionMatrix(glm::mat4& matrix) const override;
        void calcViewMatrix(glm::mat4& matrix) const override;

    private:
        glm::vec2 mTopLeft;
        glm::vec2 mBottomRight;
        float mNearZ;
        float mFarZ;
    };
}
