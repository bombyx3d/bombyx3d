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
#include "AffineTransform.h"
#include <cmath>

namespace B3D
{
    AffineTransform::AffineTransform()
    {
        setIdentity();
    }

    AffineTransform::AffineTransform(const AffineTransform& t1, const AffineTransform& t2)
    {
        a = t2.a * t1.a + t2.b * t1.c;
        b = t2.a * t1.b + t2.b * t1.d;
        c = t2.c * t1.a + t2.d * t1.c;
        d = t2.c * t1.b + t2.d * t1.d;
        tx = t2.tx * t1.a + t2.ty * t1.c + t1.tx;
        ty = t2.tx * t1.b + t2.ty * t1.d + t1.ty;
    }

    AffineTransform& AffineTransform::setIdentity()
    {
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        d = 1.0f;
        tx = 0.0f;
        ty = 0.0f;
        return *this;
    }

    AffineTransform& AffineTransform::setTranslation(const glm::vec2& t)
    {
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        d = 1.0f;
        tx = t.x;
        ty = t.y;
        return *this;
    }

    AffineTransform& AffineTransform::setTranslation(float x, float y)
    {
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        d = 1.0f;
        tx = x;
        ty = y;
        return *this;
    }

    AffineTransform& AffineTransform::setRotation(float r)
    {
        float sR = sinf(r);
        float cR = cosf(r);

        a = cR;
        b = sR;
        c = -sR;
        d = cR;
        tx = 0.0f;
        ty = 0.0f;
        return *this;
    }

    AffineTransform& AffineTransform::setScale(const glm::vec2& s)
    {
        a = s.x;
        b = 0.0f;
        c = 0.0f;
        d = s.y;
        tx = 0.0f;
        ty = 0.0f;
        return *this;
    }

    AffineTransform& AffineTransform::setScale(float s)
    {
        a = s;
        b = 0.0f;
        c = 0.0f;
        d = s;
        tx = 0.0f;
        ty = 0.0f;
        return *this;
    }

    AffineTransform& AffineTransform::setScale(float x, float y)
    {
        a = x;
        b = 0.0f;
        c = 0.0f;
        d = y;
        tx = 0.0f;
        ty = 0.0f;
        return *this;
    }

    AffineTransform& AffineTransform::setTranslationRotation(const glm::vec2& t, float r)
    {
        float sR = sinf(r);
        float cR = cosf(r);

        a = cR;
        b = sR;
        c = -sR;
        d = cR;
        tx = t.x;
        ty = t.y;

        return *this;
    }

    AffineTransform& AffineTransform::setTranslationScale(const glm::vec2& t, const glm::vec2& s)
    {
        a = s.x;
        b = 0.0f;
        c = 0.0f;
        d = s.y;
        tx = t.x;
        ty = t.y;

        return *this;
    }

    AffineTransform& AffineTransform::setTranslationRotationScale(const glm::vec2& t, float r, const glm::vec2& s)
    {
        float sR = sinf(r);
        float cR = cosf(r);

        a = s.x * cR;
        b = s.x * sR;
        c = s.y * -sR;
        d = s.y * cR;
        tx = t.x;
        ty = t.y;

        return *this;
    }

    AffineTransform& AffineTransform::translate(const glm::vec2& t)
    {
        tx += a * t.x + c * t.y;
        ty += b * t.x + d * t.y;
        return *this;
    }

    AffineTransform& AffineTransform::translate(float x, float y)
    {
        tx += a * x + c * y;
        ty += b * x + d * y;
        return *this;
    }

    AffineTransform& AffineTransform::rotate(float r)
    {
        float sR = sinf(r);
        float cR = cosf(r);

        a = a * cR + c * sR;
        b = b * cR + d * sR;
        c = c * cR - a * sR;
        d = d * cR - b * sR;

        return *this;
    }

    AffineTransform& AffineTransform::scale(const glm::vec2& s)
    {
        a *= s.x;
        b *= s.x;
        c *= s.y;
        d *= s.y;
        return *this;
    }

    AffineTransform& AffineTransform::scale(float s)
    {
        a *= s;
        b *= s;
        c *= s;
        d *= s;
        return *this;
    }

    AffineTransform& AffineTransform::scale(float x, float y)
    {
        a *= x;
        b *= x;
        c *= y;
        d *= y;
        return *this;
    }

    AffineTransform AffineTransform::inverse() const
    {
        AffineTransform t;

        float det = 1.0f / (a * d - b * c);
        t.a = d * det;
        t.b = b * -det;
        t.c = c * -det;
        t.d = a * det;
        t.tx = det * (c * ty - d * tx);
        t.ty = det * (b * tx - a * ty);

        return t;
    }

    glm::mat3 AffineTransform::toMat3() const
    {
        return glm::mat3(
              a ,   b , 0.0f,
              c ,   d , 0.0f,
             tx ,  ty , 1.0f
        );
    }

    glm::mat4 AffineTransform::toMat4() const
    {
        return glm::mat4(
              a ,   b , 0.0f, 0.0f,
              c ,   d , 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
             tx ,  ty , 0.0f, 1.0f
        );
    }

    glm::vec2 AffineTransform::transform(const glm::vec2& vector) const
    {
        return glm::vec2(
            a * vector.x + c * vector.y + tx,
            b * vector.x + d * vector.y + ty
        );
    }
}
