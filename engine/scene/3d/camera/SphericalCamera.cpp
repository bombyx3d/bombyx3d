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
#include "SphericalCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Engine
{
    SphericalCamera::SphericalCamera()
        : mDistance(1.0f)
        , mHorizontalAngle(0.0f)
        , mVerticalAngle(0.0f)
        , mTarget(0.0f)
    {
    }

    SphericalCamera::~SphericalCamera()
    {
    }

    void SphericalCamera::calcViewMatrix(glm::mat4& matrix) const
    {
        float t = cosf(mVerticalAngle);
        float eyeX = t * cosf(mHorizontalAngle);
        float eyeZ = t * sinf(mHorizontalAngle);
        float eyeY = sinf(mVerticalAngle);
        glm::vec3 eye(eyeX, eyeY, eyeZ);

        glm::vec3 forward(glm::normalize(mTarget - eye));
        glm::vec3 up = glm::cross(eye, forward);

        matrix = glm::lookAt(eye * mDistance, mTarget, up);
    }
}
