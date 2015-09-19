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
#include <glm/glm.hpp>
#include <vector>

namespace Engine
{
    struct BoundingBox
    {
        glm::vec3 min;
        glm::vec3 max;

        BoundingBox() : min(0.0f), max(0.0f) {}
        BoundingBox(const glm::vec3& mn, const glm::vec3& mx) : min(mn), max(mx) {}

        glm::vec3 center() const { return (max + min) * 0.5f; }
        glm::vec3 size() const { return max - min; }

        void set(const glm::vec3& mn, const glm::vec3& mx) { min = mn; max = mx; }

        void initFromPoint(const glm::vec3& point) { min = max = point; }
        void initFromPoints(const std::vector<glm::vec3>& points);

        void addPoint(const glm::vec3& point);
        void addBoundingBox(const BoundingBox& box);
    };
}
