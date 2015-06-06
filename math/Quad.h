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

namespace Z
{
    struct Quad
    {
        // The order is important! Do not change.
        glm::vec2 topLeft;
        glm::vec2 topRight;
        glm::vec2 bottomLeft;
        glm::vec2 bottomRight;

        Quad() = default;
        ~Quad() = default;

        static Quad allZero()
        {
            return Quad().setAllZero();
        }

        static Quad fromZeroToOne()
        {
            return Quad().setZeroToOne();
        }

        static Quad fromTopLeftAndSize(float x, float y, float w, float h)
        {
            return Quad().setTopLeftAndSize(x, y, w, h);
        }

        static Quad fromTopLeftAndSize(const glm::vec2& p, const glm::vec2& s)
        {
            return Quad().setTopLeftAndSize(p, s);
        }

        static Quad fromCenterAndSize(float x, float y, float w, float h)
        {
            return Quad().setCenterAndSize(x, y, w, h);
        }

        static Quad fromCenterAndSize(const glm::vec2& c, const glm::vec2& s)
        {
            return Quad().setCenterAndSize(c, s);
        }

        static Quad fromTopLeftAndBottomRight(float x1, float y1, float x2, float y2)
        {
            return Quad().setTopLeftAndBottomRight(x1, y1, x2, y2);
        }

        static Quad fromTopLeftAndBottomRight(const glm::vec2& topLeft, const glm::vec2& bottomRight)
        {
            return Quad().setTopLeftAndBottomRight(topLeft, bottomRight);
        }

        Quad& setAllZero()
        {
            setTopLeftAndBottomRight(0.0f, 0.0f, 0.0f, 0.0f);
            return *this;
        }

        Quad& setZeroToOne()
        {
            setTopLeftAndBottomRight(0.0f, 0.0f, 1.0f, 1.0f);
            return *this;
        }

        Quad& setTopLeftAndSize(float x, float y, float w, float h)
        {
            setTopLeftAndBottomRight(x, y, x + w, y + h);
            return *this;
        }

        Quad& setTopLeftAndSize(const glm::vec2& p, const glm::vec2& s)
        {
            setTopLeftAndBottomRight(p, p + s);
            return *this;
        }

        Quad& setCenterAndSize(float x, float y, float w, float h)
        {
            float halfW = w * 0.5f;
            float halfH = h * 0.5f;
            setTopLeftAndBottomRight(x - halfW, y - halfH, x + halfW, y + halfH);
            return *this;
        }

        Quad& setCenterAndSize(const glm::vec2& c, const glm::vec2& s)
        {
            glm::vec2& halfSize = s * 0.5f;
            setTopLeftAndBottomRight(c - halfSize, c + halfSize);
            return *this;
        }

        Quad& setTopLeftAndBottomRight(float x1, float y1, float x2, float y2)
        {
            topLeft.x = x1;
            topLeft.y = y1;
            topRight.x = x2;
            topRight.y = y1;
            bottomLeft.x = x1;
            bottomLeft.y = y2;
            bottomRight.x = x2;
            bottomRight.y = y2;
            return *this;
        }

        Quad& setTopLeftAndBottomRight(const glm::vec2& p1, const glm::vec2& p2)
        {
            topLeft = p1;
            topRight.x = p2.x;
            topRight.y = p1.y;
            bottomLeft.x = p1.x;
            bottomLeft.y = p2.y;
            bottomRight = p2;
            return *this;
        }
    };
}
