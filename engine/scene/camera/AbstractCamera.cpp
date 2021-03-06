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
#include "AbstractCamera.h"

namespace B3D
{
    AbstractCamera::AbstractCamera()
        : mFlags(ProjectionMatrixDirty | InverseProjectionMatrixDirty | ViewMatrixDirty | InverseViewMatrixDirty)
    {
    }

    AbstractCamera::~AbstractCamera()
    {
    }

    const glm::mat4& AbstractCamera::projectionMatrix() const
    {
        if (mFlags & ProjectionMatrixDirty) {
            calcProjectionMatrix(mProjectionMatrix);
            mFlags &= ~size_t(ProjectionMatrixDirty);
        }
        return mProjectionMatrix;
    }

    const glm::mat4& AbstractCamera::inverseProjectionMatrix() const
    {
        if (mFlags & InverseProjectionMatrixDirty) {
            calcInverseProjectionMatrix(mInverseProjectionMatrix);
            mFlags &= ~size_t(InverseProjectionMatrixDirty);
        }
        return mInverseProjectionMatrix;
    }

    const glm::mat4& AbstractCamera::viewMatrix() const
    {
        if (mFlags & ViewMatrixDirty) {
            calcViewMatrix(mViewMatrix);
            mFlags &= ~size_t(ViewMatrixDirty);
        }
        return mViewMatrix;
    }

    const glm::mat4& AbstractCamera::inverseViewMatrix() const
    {
        if (mFlags & InverseViewMatrixDirty) {
            calcInverseViewMatrix(mInverseViewMatrix);
            mFlags &= ~size_t(InverseViewMatrixDirty);
        }
        return mInverseViewMatrix;
    }

    void AbstractCamera::calcInverseProjectionMatrix(glm::mat4& matrix) const
    {
        matrix = glm::inverse(projectionMatrix());
    }

    void AbstractCamera::calcInverseViewMatrix(glm::mat4& matrix) const
    {
        matrix = glm::inverse(viewMatrix());
    }

    void AbstractCamera::onBeforeDrawScene(const IScene*, ICanvas* canvas)
    {
        canvas->pushProjectionMatrix();
        canvas->pushModelViewMatrix();

        canvas->setProjectionMatrix(projectionMatrix());
        canvas->setModelViewMatrix(viewMatrix());
    }

    void AbstractCamera::onAfterDrawScene(const IScene*, ICanvas* canvas)
    {
        canvas->popModelViewMatrix();
        canvas->popProjectionMatrix();
    }

    void AbstractCamera::onBeforeTouchEvent(TouchEvent, int, glm::vec2& position, bool&)
    {
        position = glm::vec2(inverseProjectionMatrix() * inverseViewMatrix() * glm::vec4(position, 0.0f, 1.0f));
    }
}
