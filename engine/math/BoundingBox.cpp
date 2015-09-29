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
#include "BoundingBox.h"

namespace B3D
{
    void BoundingBox::initFromPoints(const std::vector<glm::vec3>& points)
    {
        if (points.empty())
            min = max = glm::vec3(0.0f);
        else {
            min = max = points[0];
            for (size_t i = 1; i < points.size(); i++) {
                min = glm::min(min, points[i]);
                max = glm::max(max, points[i]);
            }
        }
    }

    void BoundingBox::addPoint(const glm::vec3& point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }

    void BoundingBox::addBoundingBox(const BoundingBox& box)
    {
        min = glm::min(min, box.min);
        max = glm::max(max, box.max);
    }
}
