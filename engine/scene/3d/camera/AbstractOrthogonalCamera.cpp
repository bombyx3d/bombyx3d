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
#include "AbstractOrthogonalCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    AbstractOrthogonalCamera::AbstractOrthogonalCamera()
        : mTopLeft(-1.0f, -1.0f)
        , mBottomRight(-1.0f, -1.0f)
        , mNearZ(1.0f)
        , mFarZ(100.0f)
    {
    }

    AbstractOrthogonalCamera::~AbstractOrthogonalCamera()
    {
    }

    void AbstractOrthogonalCamera::calcProjectionMatrix(glm::mat4& matrix) const
    {
        matrix = glm::ortho(mTopLeft.x, mBottomRight.x, mBottomRight.y, mTopLeft.y, mNearZ, mFarZ);
    }

    void AbstractOrthogonalCamera::calcViewMatrix(glm::mat4& matrix) const
    {
        matrix = glm::mat4(1.0f);
    }
}
