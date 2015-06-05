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
#include "MatrixStack.h"
#include "utility/debug.h"
#include <algorithm>

namespace Z
{
    MatrixStack::MatrixStack(size_t initialReserve, const ModificationListener& listener)
        : m_Listener(listener)
    {
        m_Matrices.reserve(std::max(size_t(1), initialReserve));
        m_Matrices.resize(1);
        m_Matrices.back() = glm::mat4(1.0f);
    }

    MatrixStack::~MatrixStack()
    {
    }

    void MatrixStack::reset()
    {
        m_Matrices.resize(1);
        m_Matrices.back() = glm::mat4(1.0f);
        invokeListener();
    }

    const glm::mat4& MatrixStack::top() const
    {
        Z_ASSERT(!m_Matrices.empty());
        return m_Matrices.back();
    }

    void MatrixStack::replaceTop(const glm::mat4& matrix)
    {
        Z_ASSERT(!m_Matrices.empty());
        m_Matrices.back() = matrix;
        invokeListener();
    }

    void MatrixStack::pushApply(const glm::mat4& matrix)
    {
        Z_ASSERT(!m_Matrices.empty());
        m_Matrices.back() *= matrix;
        invokeListener();
    }

    void MatrixStack::pushReplace(const glm::mat4& matrix)
    {
        m_Matrices.emplace_back(matrix);
        invokeListener();
    }

    void MatrixStack::pop()
    {
        Z_CHECK(m_Matrices.size() > 1);
        if (m_Matrices.size() > 1) {
            m_Matrices.pop_back();
            invokeListener();
        }
    }

    void MatrixStack::invokeListener()
    {
        if (m_Listener)
            m_Listener();
    }
}
