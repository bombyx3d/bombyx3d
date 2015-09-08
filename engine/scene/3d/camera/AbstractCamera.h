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
#include "engine/core/macros.h"
#include "engine/interfaces/scene/3d/camera/ICamera.h"

namespace Engine
{
    class AbstractCamera : public ICamera
    {
    public:
        AbstractCamera();
        ~AbstractCamera();

        const glm::mat4& projectionMatrix() const override;
        const glm::mat4& inverseProjectionMatrix() const override;

        const glm::mat4& viewMatrix() const override = 0;
        const glm::mat4& inverseViewMatrix() const override;

    protected:
        void setProjectionMatrixDirty() { mFlags |= ProjectionMatrixDirty | InverseProjectionMatrixDirty; }
        virtual void calcProjectionMatrix(glm::mat4& matrix) const = 0;
        virtual void calcInverseProjectionMatrix(glm::mat4& matrix) const;

        void setViewMatrixDirty() { mFlags |= ViewMatrixDirty | InverseViewMatrixDirty; }
        virtual void calcViewMatrix(glm::mat4& matrix) const = 0;
        virtual void calcInverseViewMatrix(glm::mat4& matrix) const;

    private:
        enum Flag
        {
            ProjectionMatrixDirty = 0x0001,
            InverseProjectionMatrixDirty = 0x0002,
            ViewMatrixDirty = 0x0004,
            InverseViewMatrixDirty = 0x0008,
        };

        mutable glm::mat4 mProjectionMatrix;
        mutable glm::mat4 mInverseProjectionMatrix;
        mutable glm::mat4 mViewMatrix;
        mutable glm::mat4 mInverseViewMatrix;
        mutable size_t mFlags;

        Z_DISABLE_COPY(AbstractCamera);
    };
}
