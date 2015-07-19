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
#include "core/math/AffineTransform.h"
#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace Engine
{
    /** A LIFO stack of 4x4 matrices. */
    class MatrixStack
    {
    public:
        /** Listener function. */
        using ModificationListener = std::function<void()>;

        /**
         * Constructor.
         * @param initialReserve Initial size of preallocated storage for the stack.
         * @param listener Listener function to be called when matrix has been pushed or popped from this stack.
         */
        explicit MatrixStack(size_t initialReserve = 1, const ModificationListener& listener = ModificationListener());

        /** Destructor. */
        ~MatrixStack();

        /** Resets stack to initial state. */
        void reset();

        /**
         * Retrieves the topmost matrix.
         * If stack is empty, this method returns an identity matrix.
         * @return The topmost matrix.
         */
        const glm::mat4& top() const;

        /**
         * Replaces topmost matrix with the specified matrix.
         * @param matrix Matrix to replace with.
         */
        void replaceTop(const glm::mat4& matrix);

        /**
         * Pushes given matrix onto the top of the stack.
         * Given matrix becomes the topmost matrix.
         */
        void pushReplace(const glm::mat4& matrix);

        /**
         * Multiplies current topmost matrix onto the provided matrix and pushes
         * the resulting matrix onto the top of the stack.
         * @param matrix Matrix to apply.
         */
        void pushApply(const glm::mat4& matrix);

        /**
         * Multiplies current topmost matrix onto the provided 2D affine transformation
         * and pushes the resulting matrix onto the top of the stack.
         * @param transform 2D affine transformation to apply.
         */
        void pushApply(const AffineTransform& transform);

        /**
         * Applies translation transformation to the topmost matrix
         * and pushes the resulting matrix onto the top of the stack.
         * @param x Translation by X.
         * @param y Translation by Y.
         * @param z Translation by Z.
         */
        void pushTranslate(float x, float y, float z = 0.0f);

        /**
         * Applies translation transformation to the topmost matrix
         * and pushes the resulting matrix onto the top of the stack.
         * @param translate Translation.
         */
        void pushTranslate(const glm::vec2& translate);

        /**
         * Applies translation transformation to the topmost matrix
         * and pushes the resulting matrix onto the top of the stack.
         * @param translate Translation.
         */
        void pushTranslate(const glm::vec3& translate);

        /**
         * Pops the topmost matrix.
         * Matrix that was next to the popped matrix becomes the topmost one.
         * It is an error to call this method on an empty stack.
         */
        void pop();

    private:
        std::vector<glm::mat4> m_Matrices;          /**< Stack of matrices. */
        ModificationListener m_Listener;            /**< Listener function to be called when stack has been modified. */

        /** Invokes the modification listener. */
        void invokeListener();
    };
}
