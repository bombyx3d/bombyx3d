﻿/*
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
#include "core/math/AffineTransform.h"
#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace Z
{
    using namespace Engine;

    class MatrixStack
    {
    public:
        using ModificationListener = std::function<void()>;

        explicit MatrixStack(size_t initialReserve = 1, const ModificationListener& listener = ModificationListener());
        ~MatrixStack();

        void reset();

        const glm::mat4& top() const;

        void replaceTop(const glm::mat4& matrix);
        void pushReplace(const glm::mat4& matrix);

        void pushApply(const glm::mat4& matrix);
        void pushApply(const AffineTransform& transform);

        void pushTranslate(float x, float y, float z = 0.0f);
        void pushTranslate(const glm::vec2& translate);
        void pushTranslate(const glm::vec3& translate);

        void pop();

    private:
        std::vector<glm::mat4> m_Matrices;
        ModificationListener m_Listener;

        void invokeListener();
    };
}
