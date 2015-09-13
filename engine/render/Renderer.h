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
#include "engine/interfaces/render/IRenderer.h"
#include "engine/render/Shader.h"
#include "engine/render/VertexSource.h"
#include "engine/render/Uniform.h"
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Engine
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        ~Renderer();

        void beginFrame() override;
        void endFrame() override;

        void setViewport(int x, int y, int w, int h) override;

        void clear() override;

        ShaderPtr createShader() override;
        TexturePtr createTexture() override;
        VertexBufferPtr createVertexBuffer() override;
        IndexBufferPtr createIndexBuffer() override;
        VertexSourcePtr createVertexSource() override;

        const glm::mat4& projectionMatrix() const override;
        void setProjectionMatrix(const glm::mat4& matrix) override;
        void pushProjectionMatrix() override;
        void popProjectionMatrix() override;

        const glm::mat4& modelViewMatrix() const override;
        void setModelViewMatrix(const glm::mat4& matrix) override;
        void pushModelViewMatrix() override;
        void popModelViewMatrix() override;

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
        Atom mProjectionMatrixUniform;
        Atom mModelViewMatrixUniform;
        glm::mat4 mProjectionMatrix;
        glm::mat4 mModelViewMatrix;
        std::vector<glm::mat4> mProjectionMatrixStack;
        std::vector<glm::mat4> mModelViewMatrixStack;
        std::unordered_map<Atom, Uniform> mUniforms;
        std::shared_ptr<Shader> mCurrentShader;
        std::shared_ptr<VertexSource> mCurrentVertexSource;
        bool mShouldRebindUniforms;
        bool mShouldRebindAttributes;

        bool setupDrawCall();
        void bindUniforms();

        void resetOpenGLBindings();

        Z_DISABLE_COPY(Renderer);
    };
}
