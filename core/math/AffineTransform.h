/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
    /**
     * 2D affine transformation.
     *
     * An affine transformation is a special type of transformation that preserves parallel lines but
     * does not necessarily preserve lengths or angles.
     *
     * Scaling, rotation and translation are the most commonly used affine transformations. Skewing is
     * also possible.
     *
     * Affine transformation is represented by a 3x3 matrix:
     * ```
     * | a  b  0|
     * | c  d  0|
     * |tx ty  1|
     * ```
     */
    class AffineTransform
    {
    public:
        float a;            /**< Value at position [1,1] in the matrix. */
        float b;            /**< Value at position [1,2] in the matrix. */
        float c;            /**< Value at position [2,1] in the matrix. */
        float d;            /**< Value at position [2,2] in the matrix. */
        float tx;           /**< Value at position [3,1] in the matrix. */
        float ty;           /**< Value at position [3,2] in the matrix. */

        /** Constructs an identity transformation. */
        AffineTransform();

        /**
         * Constructs transformation by applying transformation `t2` to parent transformation `t1`.
         * @param t1 Parent transformation.
         * @param t2 Transformation to apply to `t1`.
         */
        AffineTransform(const AffineTransform& t1, const AffineTransform& t2);

        /**
         * Creates an identity transformation.
         * @return Identity transformation.
         */
        static AffineTransform identity()
        {
            return AffineTransform().setIdentity();
        }

        /**
         * Creates a translation transformation.
         * @param t Translation.
         * @return Translation transformation.
         */
        static AffineTransform translation(const glm::vec2& t)
        {
            return AffineTransform().setTranslation(t);
        }

        /**
         * Creates a translation transformation.
         * @param x Translation by X.
         * @param y Translation by y.
         * @return Translation transformation.
         */
        static AffineTransform translation(float x, float y)
        {
            return AffineTransform().setTranslation(x, y);
        }

        /**
         * Creates a rotation transformation.
         * @param r Rotation angle in radians.
         * @return Rotation transformation.
         */
        static AffineTransform rotation(float r)
        {
            return AffineTransform().setRotation(r);
        }

        /**
         * Creates a scaling transformation.
         * @param s Scale.
         * @return Scaling transformation.
         */
        static AffineTransform scaling(const glm::vec2& s)
        {
            return AffineTransform().setScale(s);
        }

        /**
         * Creates a scaling transformation.
         * @param s Scale.
         * @return Scaling transformation.
         */
        static AffineTransform scaling(float s)
        {
            return AffineTransform().setScale(s);
        }

        /**
         * Creates a scaling transformation.
         * @param x Scale by X.
         * @param y Scale by Y.
         * @return Scaling transformation.
         */
        static AffineTransform scaling(float x, float y)
        {
            return AffineTransform().setScale(x, y);
        }

        /**
         * Creates a transformation combining rotation and translation.
         * @param t Translation.
         * @param r Rotation angle in radians.
         * @return Transformation.
         */
        static AffineTransform translationRotation(const glm::vec2& t, float r)
        {
            return AffineTransform().setTranslationRotation(t, r);
        }

        /**
         * Creates a transformation combining translation and scaling.
         * @param t Translation.
         * @param s Scale.
         * @return Transformation.
         */
        static AffineTransform translationScale(const glm::vec2& t, const glm::vec2& s)
        {
            return AffineTransform().setTranslationScale(t, s);
        }

        /**
         * Creates a transformation combining translation, rotation and scaling.
         * @param t Translation.
         * @param r Rotation angle in radians.
         * @param s Scale.
         * @return Transformation.
         */
        static AffineTransform translationRotationScale(const glm::vec2& t, float r, const glm::vec2& s)
        {
            return AffineTransform().setTranslationRotationScale(t, r, s);
        }

        /**
         * Sets this transformation to an identity transformation.
         * @return Reference to `this`.
         */
        AffineTransform& setIdentity();

        /**
         * Sets this transformation to a translation transformation.
         * @param t Translation.
         * @return Reference to `this`.
         */
        AffineTransform& setTranslation(const glm::vec2& t);

        /**
         * Sets this transformation to a translation transformation.
         * @param x Translation by X.
         * @param y Translation by Y.
         * @return Reference to `this`.
         */
        AffineTransform& setTranslation(float x, float y);

        /**
         * Sets this transformation to a rotation transformation.
         * @param r Rotation angle in radians.
         * @return Reference to `this`.
         */
        AffineTransform& setRotation(float r);

        /**
         * Sets this transformation to a scaling transformation.
         * @param s Scale.
         * @return Reference to `this`.
         */
        AffineTransform& setScale(const glm::vec2& s);

        /**
         * Sets this transformation to a scaling transformation.
         * @param s Scale.
         * @return Reference to `this`.
         */
        AffineTransform& setScale(float s);

        /**
         * Sets this transformation to a scaling transformation.
         * @param x Scale by X.
         * @param y Scale by Y.
         * @return Reference to `this`.
         */
        AffineTransform& setScale(float x, float y);

        /**
         * Sets this transformation to a transformation combining rotation and translation.
         * @param t Translation.
         * @param r Rotation angle in radians.
         * @return Reference to `this`.
         */
        AffineTransform& setTranslationRotation(const glm::vec2& t, float r);

        /**
         * Sets this transformation to a transformation combining translation and scaling.
         * @param t Translation.
         * @param s Scale.
         * @return Reference to `this`.
         */
        AffineTransform& setTranslationScale(const glm::vec2& t, const glm::vec2& s);

        /**
         * Sets this transformation to a transformation combining translation, rotation and scaling.
         * @param t Translation.
         * @param r Rotation angle in radians.
         * @param s Scale.
         * @return Reference to `this`.
         */
        AffineTransform& setTranslationRotationScale(const glm::vec2& t, float r, const glm::vec2& s);

        /**
         * Applies translation to the given transformation.
         * @param T Input transformation.
         * @param t Translation to apply.
         * @return New transformation.
         */
        static AffineTransform translate(const AffineTransform& T, const glm::vec2& t)
        {
            auto T1 = T;
            return T1.translate(t);
        }

        /**
         * Applies translation to the given transformation.
         * @param T Input transformation.
         * @param x Translation by X.
         * @param y Translation by Y.
         * @return New transformation.
         */
        static AffineTransform translate(const AffineTransform& T, float x, float y)
        {
            auto T1 = T;
            return T1.translate(x, y);
        }

        /**
         * Applies rotation to the given transformation.
         * @param T Input transformation.
         * @param r Rotation angle in radians.
         * @return New transformation.
         */
        static AffineTransform rotate(const AffineTransform& T, float r)
        {
            auto T1 = T;
            return T1.rotate(r);
        }

        /**
         * Applies scale to the given transformation.
         * @param T Input transformation.
         * @param s Scale.
         * @return New transformation.
         */
        static AffineTransform scale(const AffineTransform& T, const glm::vec2& s)
        {
            auto T1 = T;
            return T1.scale(s);
        }

        /**
         * Applies scale to the given transformation.
         * @param T Input transformation.
         * @param s Scale.
         * @return New transformation.
         */
        static AffineTransform scale(const AffineTransform& T, float s)
        {
            auto T1 = T;
            return T1.scale(s);
        }

        /**
         * Applies scale to the given transformation.
         * @param T Input transformation.
         * @param x Scale by X.
         * @param x Scale by Y.
         * @return New transformation.
         */
        static AffineTransform scale(const AffineTransform& T, float x, float y)
        {
            auto T1 = T;
            return T1.scale(x, y);
        }

        /**
         * Applies translation to this transformation.
         * @param t Translation.
         * @return Reference to `this`.
         */
        AffineTransform& translate(const glm::vec2& t);

        /**
         * Applies translation to this transformation.
         * @param x Translation by X.
         * @param y Translation by Y.
         * @return Reference to `this`.
         */
        AffineTransform& translate(float x, float y);

        /**
         * Applies rotation to this transformation.
         * @param r Rotation angle in radians.
         * @return Reference to `this`.
         */
        AffineTransform& rotate(float r);

        /**
         * Applies scaling to this transformation.
         * @param s Scale.
         * @return Reference to `this`.
         */
        AffineTransform& scale(const glm::vec2& s);

        /**
         * Applies scaling to this transformation.
         * @param s Scale.
         * @return Reference to `this`.
         */
        AffineTransform& scale(float s);

        /**
         * Applies scaling to this transformation.
         * @param x Scale by X.
         * @param y Scale by Y.
         * @return Reference to `this`.
         */
        AffineTransform& scale(float x, float y);

        /**
         * Calculates inverse transformation for this transformation.
         * @return Inverse transformation.
         */
        AffineTransform inverse() const;

        /**
         * Calculates 3x3 matrix for this transformation.
         * @return 3x3 matrix.
         */
        glm::mat3 toMat3() const;

        /**
         * Calculates 4x4 matrix for this transformation.
         * @return 4x4 matrix.
         */
        glm::mat4 toMat4() const;

        /**
         * Applies this transformation to the specified vector.
         * @param vector Vector to transform.
         * @return Transformed vector.
         */
        glm::vec2 transform(const glm::vec2& vector) const;
    };
}
