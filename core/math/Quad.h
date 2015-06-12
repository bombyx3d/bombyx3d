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

namespace Engine
{
    /** 2D quad consisting of four points. */
    struct Quad
    {
        // The order is important! Do not change.
        glm::vec2 topLeft;                      /**< Coordinates of the top left corner. */
        glm::vec2 topRight;                     /**< Coordinates of the top right corner. */
        glm::vec2 bottomLeft;                   /**< Coordinates of the bottom left corner. */
        glm::vec2 bottomRight;                  /**< Coordinates of the bottom right corner. */

        /**
         * Creates a quad with all coordinates set to (0, 0).
         * @return Quad with all coodrinates set to (0, 0).
         */
        static Quad allZero()
        {
            return Quad().setAllZero();
        }

        /**
         * Creates a quad with top left corner at (0, 0) and with size of (1, 1).
         * @return Quad with top left corner at (0, 0) and with size of (1, 1).
         */
        static Quad fromZeroToOne()
        {
            return Quad().setZeroToOne();
        }

        /**
         * Creates a quad with the specified position and size.
         * @param x X coordinate of the top left corner.
         * @param y Y coordinate of the top left corner.
         * @param w Width.
         * @param h Height.
         * @return Quad with the specified position and size.
         */
        static Quad fromTopLeftAndSize(float x, float y, float w, float h)
        {
            return Quad().setTopLeftAndSize(x, y, w, h);
        }

        /**
         * Creates a quad with the specified position and size.
         * @param p Coordinates of the top left corner.
         * @param s Size.
         * @return Quad with the specified position and size.
         */
        static Quad fromTopLeftAndSize(const glm::vec2& p, const glm::vec2& s)
        {
            return Quad().setTopLeftAndSize(p, s);
        }

        /**
         * Creates a quad with the specified position and size.
         * @param x X coordinate of the quad center.
         * @param y Y coordinate of the quad center.
         * @param w Width.
         * @param h Height.
         * @return Quad with the specified position and size.
         */
        static Quad fromCenterAndSize(float x, float y, float w, float h)
        {
            return Quad().setCenterAndSize(x, y, w, h);
        }

        /**
         * Creates a quad with the specified position and size.
         * @param c Coordinates of the quad center.
         * @param s Size.
         * @return Quad with the specified position and size.
         */
        static Quad fromCenterAndSize(const glm::vec2& c, const glm::vec2& s)
        {
            return Quad().setCenterAndSize(c, s);
        }

        /**
         * Creates a quad with the specified coordinates.
         * @param x1 X coordinate of the top left corner.
         * @param y1 Y coordinate of the top left corner.
         * @param x2 X coordinate of the bottom right corner.
         * @param y2 Y coordinate of the bottom right corner.
         * @return Quad with the specified coordinates.
         */
        static Quad fromTopLeftAndBottomRight(float x1, float y1, float x2, float y2)
        {
            return Quad().setTopLeftAndBottomRight(x1, y1, x2, y2);
        }

        /**
         * Creates a quad with the specified coordinates.
         * @param p1 Coordinates of the top left corner.
         * @param p2 Coordinates of the bottom right corner.
         * @return Quad with the specified coordinates.
         */
        static Quad fromTopLeftAndBottomRight(const glm::vec2& p1, const glm::vec2& p2)
        {
            return Quad().setTopLeftAndBottomRight(p1, p2);
        }

        /**
         * Initializes all coordinates of this quad to (0, 0).
         * @return Reference to `this`.
         */
        Quad& setAllZero()
        {
            setTopLeftAndBottomRight(0.0f, 0.0f, 0.0f, 0.0f);
            return *this;
        }

        /**
         * Initializes this quad to have top left corner at (0, 0) and size of (1, 1).
         * @return Reference to `this`.
         */
        Quad& setZeroToOne()
        {
            setTopLeftAndBottomRight(0.0f, 0.0f, 1.0f, 1.0f);
            return *this;
        }

        /**
         * Initializes coordinates of this quad.
         * @param x X coordinate of the top left corner.
         * @param y Y coordinate of the top left corner.
         * @param w Width.
         * @param h Height.
         * @return Reference to `this`.
         */
        Quad& setTopLeftAndSize(float x, float y, float w, float h)
        {
            setTopLeftAndBottomRight(x, y, x + w, y + h);
            return *this;
        }

        /**
         * Initializes coordinates of this quad.
         * @param p Coordinates of the top left corner.
         * @param s Size.
         * @return Reference to `this`.
         */
        Quad& setTopLeftAndSize(const glm::vec2& p, const glm::vec2& s)
        {
            setTopLeftAndBottomRight(p, p + s);
            return *this;
        }

        /**
         * Initializes coordinates of this quad.
         * @param x X coordinate of the quad center.
         * @param y Y coordinate of the quad center.
         * @param w Width.
         * @param h Height.
         * @return Reference to `this`.
         */
        Quad& setCenterAndSize(float x, float y, float w, float h)
        {
            float halfW = w * 0.5f;
            float halfH = h * 0.5f;
            setTopLeftAndBottomRight(x - halfW, y - halfH, x + halfW, y + halfH);
            return *this;
        }

        /**
         * Initializes coordinates of this quad.
         * @param c Coordinates of the quad center.
         * @param s Size.
         * @return Reference to `this`.
         */
        Quad& setCenterAndSize(const glm::vec2& c, const glm::vec2& s)
        {
            glm::vec2& halfSize = s * 0.5f;
            setTopLeftAndBottomRight(c - halfSize, c + halfSize);
            return *this;
        }

        /**
         * Initializes coordinates of this quad.
         * @param x1 X coordinate of the top left corner.
         * @param y1 Y coordinate of the top left corner.
         * @param x2 X coordinate of the bottom right corner.
         * @param y2 Y coordinate of the bottom right corner.
         * @return Reference to `this`.
         */
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

        /**
         * Initializes coordinates of this quad.
         * @param p1 Coordinates of the top left corner.
         * @param p2 Coordinates of the bottom right corner.
         * @return Reference to `this`.
         */
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
