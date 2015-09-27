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

namespace Engine
{
    class AbstractPerspectiveCamera : public AbstractCamera
    {
    public:
        AbstractPerspectiveCamera();
        ~AbstractPerspectiveCamera();

        float fovY() const { return mFovY; }
        float aspectRadio() const { return mAspect; }
        float nearZ() const { return mNearZ; }
        float farZ() const { return mFarZ; }

        void setFovY(float fovY) { mFovY = fovY; setProjectionMatrixDirty(); }

        void setAspectRatio(float ratio) { mAspect = ratio; setProjectionMatrixDirty(); }
        void setAspectRatioFromSize(const glm::vec2& size);

        void setNearZ(float value) { mNearZ = value; setProjectionMatrixDirty(); }
        void setFarZ(float value) { mFarZ = value; setProjectionMatrixDirty(); }
        void setDepthRange(float nz, float fz) { mNearZ = nz; mFarZ = fz; setProjectionMatrixDirty(); }

    protected:
        void calcProjectionMatrix(glm::mat4& matrix) const override;
        void calcViewMatrix(glm::mat4& matrix) const override = 0;

        void onSceneSizeChanged(IScene* scene, const glm::vec2& newSize) override;

    private:
        float mFovY;        // in radians
        float mAspect;
        float mNearZ;
        float mFarZ;
    };
}
