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
#include "engine/core/Atom.h"
#include "engine/interfaces/render/lowlevel/IRenderer.h"
#include "engine/render/gles2/GLES2Shader.h"
#include "engine/render/gles2/GLES2VertexSource.h"
#include "engine/render/gles2/GLES2Uniform.h"
#include <unordered_map>
#include <glm/glm.hpp>

namespace B3D
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        ~Renderer();

        void beginFrame() override;
        void endFrame() override;

        void setViewport(int x, int y, int w, int h) override;

        void setClearColor(const glm::vec4& color) override;
        void clear() override;

        ShaderPtr createShader() override;
        TexturePtr createTexture() override;
        VertexBufferPtr createVertexBuffer() override;
        IndexBufferPtr createIndexBuffer() override;
        VertexSourcePtr createVertexSource() override;

        void setCullFace(CullFace face) override;
        void setFrontFace(FrontFace face) override;

        void setBlendingEnabled(bool value) override;
        void setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor) override;

        void setDepthTestingEnabled(bool value) override;
        void setDepthWritingEnabled(bool value) override;

        void setUniform(const Atom& name, float value) override;
        void setUniform(const Atom& name, const glm::vec2& value) override;
        void setUniform(const Atom& name, const glm::vec3& value) override;
        void setUniform(const Atom& name, const glm::vec4& value) override;
        void setUniform(const Atom& name, const glm::mat4& value) override;
        void setUniform(const Atom& name, const TexturePtr& texture) override;

        void useShader(const ShaderPtr& shader) override;
        void bindVertexSource(const VertexSourcePtr& source) override;

        void drawPrimitive(PrimitiveType primitiveType, size_t first, size_t count) override;

    private:
        std::unordered_map<Atom, GLES2Uniform> mUniforms;
        std::shared_ptr<GLES2Shader> mCurrentShader;
        std::shared_ptr<GLES2VertexSource> mCurrentVertexSource;
        bool mShouldRebindUniforms;
        bool mShouldRebindAttributes;

        bool setupDrawCall();
        void bindUniforms();

        void resetOpenGLBindings();

        B3D_DISABLE_COPY(Renderer);
    };
}
