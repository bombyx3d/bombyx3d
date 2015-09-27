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
#include "engine/core/Atom.h"
#include "engine/interfaces/render/lowlevel/IShader.h"
#include "engine/interfaces/render/lowlevel/ITexture.h"
#include "engine/interfaces/render/lowlevel/IVertexBuffer.h"
#include "engine/interfaces/render/lowlevel/IIndexBuffer.h"
#include "engine/interfaces/render/lowlevel/IVertexSource.h"
#include <memory>
#include <glm/glm.hpp>

namespace Engine
{
    enum class PrimitiveType : uint8_t
    {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
    };

    enum class CullFace : uint8_t
    {
        None,
        Front,
        Back,
    };

    enum class FrontFace : uint8_t
    {
        Clockwise,
        CounterClockwise,
    };

    enum class BlendFunc : uint8_t
    {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        SrcAlphaSaturate,
    };

    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void setViewport(int x, int y, int w, int h) = 0;

        virtual void setClearColor(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual ShaderPtr createShader() = 0;
        virtual TexturePtr createTexture() = 0;
        virtual VertexBufferPtr createVertexBuffer() = 0;
        virtual IndexBufferPtr createIndexBuffer() = 0;
        virtual VertexSourcePtr createVertexSource() = 0;

        virtual void setCullFace(CullFace face) = 0;
        virtual void setFrontFace(FrontFace face) = 0;

        virtual void setBlendingEnabled(bool value) = 0;
        virtual void setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor) = 0;

        virtual void setDepthTestingEnabled(bool value) = 0;
        virtual void setDepthWritingEnabled(bool value) = 0;

        virtual void setUniform(const Atom& name, float value) = 0;
        virtual void setUniform(const Atom& name, const glm::vec2& value) = 0;
        virtual void setUniform(const Atom& name, const glm::vec3& value) = 0;
        virtual void setUniform(const Atom& name, const glm::vec4& value) = 0;
        virtual void setUniform(const Atom& name, const glm::mat4& value) = 0;
        virtual void setUniform(const Atom& name, const TexturePtr& texture) = 0;

        virtual void useShader(const ShaderPtr& shader) = 0;
        virtual void bindVertexSource(const VertexSourcePtr& source) = 0;

        virtual void drawPrimitive(PrimitiveType primitiveType, size_t first, size_t count) = 0;
    };

    using RendererPtr = std::shared_ptr<IRenderer>;
}
