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
#include "engine/scene/3d/camera/AbstractPerspectiveCamera.h"
#include <memory>
#include <glm/glm.hpp>

namespace Engine
{
    class OrbitCamera : public AbstractPerspectiveCamera
    {
    public:
        OrbitCamera();
        ~OrbitCamera();

        float distance() const { return mDistance; }
        void setDistance(float dist) { mDistance = dist; setViewMatrixDirty(); }

        float horizontalAngle() const { return mHorizontalAngle; }
        void setHorizontalAngle(float angle) { mHorizontalAngle = angle; setViewMatrixDirty(); }

        float verticalAngle() const { return mVerticalAngle; }
        void setVerticalAngle(float angle) { mVerticalAngle = angle; setViewMatrixDirty(); }

        const glm::vec3& target() const { return mTarget; }
        void setTarget(const glm::vec3& target) { mTarget = target; setViewMatrixDirty(); }

    protected:
        void calcViewMatrix(glm::mat4& matrix) const override;

    private:
        float mDistance;
        float mHorizontalAngle;     // in radians
        float mVerticalAngle;       // in radians
        glm::vec3 mTarget;
    };

    using OrbitCameraPtr = std::shared_ptr<OrbitCamera>;
}
