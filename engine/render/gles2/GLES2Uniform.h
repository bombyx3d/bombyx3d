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
#include "engine/core/macros.h"
#include "engine/interfaces/render/lowlevel/ITexture.h"
#include "engine/utility/PoolAllocator.h"
#include <glm/glm.hpp>

namespace Engine
{
    class GLES2Uniform
    {
    public:
        struct IUniformValue;

        GLES2Uniform();
        GLES2Uniform(GLES2Uniform&& other);
        ~GLES2Uniform();

        GLES2Uniform& operator=(GLES2Uniform&& other);

        void reset();

        void setFloat(float value);
        void setVec2(const glm::vec2& value);
        void setVec3(const glm::vec3& value);
        void setVec4(const glm::vec4& value);
        void setMat4(const glm::mat4& value);
        void setTexture(const TexturePtr& texture);

        bool upload(int location, int* textureCount);

    private:
        IUniformValue* mValue;

        Z_DISABLE_COPY(GLES2Uniform);
    };
}
