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

namespace Z
{
    class AffineTransform
    {
    public:
        float a;
        float b;
        float c;
        float d;
        float tx;
        float ty;

        AffineTransform() = default;
        AffineTransform(const AffineTransform& t1, const AffineTransform& t2);

        static AffineTransform identity() { return AffineTransform().setIdentity(); }
        static AffineTransform translation(const glm::vec2& t) { return AffineTransform().setTranslation(t); }
        static AffineTransform translation(float x, float y) { return AffineTransform().setTranslation(x, y); }
        static AffineTransform rotation(float r) { return AffineTransform().setRotation(r); }
        static AffineTransform scaling(const glm::vec2& s) { return AffineTransform().setScale(s); }
        static AffineTransform scaling(float s) { return AffineTransform().setScale(s); }
        static AffineTransform scaling(float x, float y) { return AffineTransform().setScale(x, y); }
        static AffineTransform translationRotation(const glm::vec2& t, float r) { return AffineTransform().setTranslationRotation(t, r); }
        static AffineTransform translationScale(const glm::vec2& t, const glm::vec2& s) { return AffineTransform().setTranslationScale(t, s); }
        static AffineTransform translationRotationScale(const glm::vec2& t, float r, const glm::vec2& s) { return AffineTransform().setTranslationRotationScale(t, r, s); }

        AffineTransform& setIdentity();
        AffineTransform& setTranslation(const glm::vec2& t);
        AffineTransform& setTranslation(float x, float y);
        AffineTransform& setRotation(float r);
        AffineTransform& setScale(const glm::vec2& s);
        AffineTransform& setScale(float s);
        AffineTransform& setScale(float x, float y);
        AffineTransform& setTranslationRotation(const glm::vec2& t, float r);
        AffineTransform& setTranslationScale(const glm::vec2& t, const glm::vec2& s);
        AffineTransform& setTranslationRotationScale(const glm::vec2& t, float r, const glm::vec2& s);

        static AffineTransform translate(const AffineTransform& T, const glm::vec2& t) { auto T1 = T; return T1.translate(t); }
        static AffineTransform translate(const AffineTransform& T, float x, float y) { auto T1 = T; return T1.translate(x, y); }
        static AffineTransform rotate(const AffineTransform& T, float r) { auto T1 = T; return T1.rotate(r); }
        static AffineTransform scale(const AffineTransform& T, const glm::vec2& s) { auto T1 = T; return T1.scale(s); }
        static AffineTransform scale(const AffineTransform& T, float s) { auto T1 = T; return T1.scale(s); }
        static AffineTransform scale(const AffineTransform& T, float x, float y) { auto T1 = T; return T1.scale(x, y); }

        AffineTransform& translate(const glm::vec2& t);
        AffineTransform& translate(float x, float y);
        AffineTransform& rotate(float r);
        AffineTransform& scale(const glm::vec2& s);
        AffineTransform& scale(float s);
        AffineTransform& scale(float x, float y);

        AffineTransform inverse() const;

        glm::mat4 toMat4() const;
        glm::vec2 transform(const glm::vec2& vector) const;
    };
}
