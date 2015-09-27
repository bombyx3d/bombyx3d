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
#include "engine/core/macros.h"
#include "engine/math/AspectRatio.h"
#include "engine/math/Quad.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Engine
{
    class OrthogonalCamera : public AbstractCamera
    {
    public:
        explicit OrthogonalCamera(const glm::vec2& virtResolution, AspectRatio ratio = AspectRatio::Fit);
        ~OrthogonalCamera();

        AspectRatio aspectRatio() const { return mAspectRatio; }
        const glm::vec2& physicalResolution() const { return mPhysicalResolution; }
        const glm::vec2& virtualResolution() const { return mVirtualResolution; }

        void setAspectRatio(AspectRatio ratio) { mAspectRatio = ratio; setProjectionMatrixDirty(); }
        void setPhysicalResolution(const glm::vec2& r) { mPhysicalResolution = r; setProjectionMatrixDirty(); }
        void setVirtualResolution(const glm::vec2& r) { mVirtualResolution = r; setProjectionMatrixDirty(); }

        const glm::vec2& position() const { return mPosition; }
        void setPosition(const glm::vec2& pos) { mPosition = pos; setViewMatrixDirty(); }

        const Quad& visibleArea() const;

        float nearZ() const { return mNearZ; }
        float farZ() const { return mFarZ; }
        void setNearZ(float value) { mNearZ = value; setProjectionMatrixDirty(); }
        void setFarZ(float value) { mFarZ = value; setProjectionMatrixDirty(); }
        void setDepthRange(float nz, float fz) { mNearZ = nz; mFarZ = fz; setProjectionMatrixDirty(); }

    protected:
        void calcProjectionMatrix(glm::mat4& matrix) const override;
        void calcViewMatrix(glm::mat4& matrix) const override;

        void onSceneSizeChanged(IScene* scene, const glm::vec2& newSize) override;

    private:
        mutable Quad mVisibleArea;
        glm::vec2 mVirtualResolution;
        glm::vec2 mPhysicalResolution;
        glm::vec2 mPosition;
        float mNearZ;
        float mFarZ;
        AspectRatio mAspectRatio;

        Z_DISABLE_COPY(OrthogonalCamera);
    };

    using OrthogonalCameraPtr = std::shared_ptr<OrthogonalCamera>;
}
