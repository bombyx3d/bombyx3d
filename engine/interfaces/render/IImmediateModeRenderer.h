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
#include "engine/interfaces/render/lowlevel/IShader.h"
#include "engine/interfaces/render/lowlevel/ITexture.h"
#include "engine/interfaces/render/lowlevel/IRenderer.h"
#include <glm/glm.hpp>

namespace Engine
{
    class IImmediateModeRenderer
    {
    public:
        virtual ~IImmediateModeRenderer() = default;

        virtual void setClearColor(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual IRenderer* beginDirectRendering() = 0;
        virtual void endDirectRendering() = 0;

        virtual const ShaderPtr& customShader() const = 0;
        virtual void setCustomShader(const ShaderPtr& shader) = 0;

        virtual const TexturePtr& texture() const = 0;
        virtual void setTexture(const TexturePtr& texture) = 0;

        virtual void resetMatrixStacks() = 0;

        virtual const glm::mat4& projectionMatrix() const = 0;
        virtual void setProjectionMatrix(const glm::mat4& matrix) = 0;
        virtual void pushProjectionMatrix() = 0;
        virtual void popProjectionMatrix() = 0;

        virtual const glm::mat4& modelViewMatrix() const = 0;
        virtual void setModelViewMatrix(const glm::mat4& matrix) = 0;
        virtual void pushModelViewMatrix() = 0;
        virtual void popModelViewMatrix() = 0;

        virtual void setBlend(bool flag) = 0;
        virtual void setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor) = 0;

        virtual void setDepthTest(bool flag) = 0;
        virtual void setDepthWrite(bool flag) = 0;

        virtual void begin(PrimitiveType primitive) = 0;
        virtual void texCoord(float x, float y) = 0;
        virtual void texCoord(const glm::vec2& coord) = 0;
        virtual void color(float r, float g, float b) = 0;
        virtual void color(float r, float g, float b, float a) = 0;
        virtual void color(const glm::vec4& color) = 0;
        virtual size_t vertex(float x, float y) = 0;
        virtual size_t vertex(float x, float y, float z) = 0;
        virtual size_t vertex(const glm::vec2& vertex) = 0;
        virtual size_t vertex(const glm::vec2& vertex, float z) = 0;
        virtual size_t vertex(const glm::vec3& vertex) = 0;
        virtual void index(size_t index) = 0;
        virtual void end() = 0;
    };
}
