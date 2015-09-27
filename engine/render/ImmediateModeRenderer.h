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
#include "engine/interfaces/render/lowlevel/IIndexBuffer.h"
#include "engine/interfaces/render/lowlevel/IVertexBuffer.h"
#include "engine/interfaces/render/lowlevel/IVertexSource.h"
#include "engine/interfaces/render/ICanvas.h"
#include "engine/core/macros.h"
#include "engine/core/Atom.h"
#include "engine/mesh/VertexFormat.h"
#include "engine/material/MaterialPass.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <cstdint>

namespace Engine
{
    class ImmediateModeRenderer : public ICanvas
    {
    public:
        static const size_t MAX_INDEX = 65534;

        explicit ImmediateModeRenderer(const RendererPtr& renderer);
        ~ImmediateModeRenderer();

        void setClearColor(const glm::vec4& color) override;
        void clear() override;

        IRenderer* beginDirectRendering() override;
        void endDirectRendering() override;

        const ShaderPtr& customShader() const override { return mCustomShader; }
        void setCustomShader(const ShaderPtr& shader) override;

        const TexturePtr& texture() const override { return mTexture; }
        void setTexture(const TexturePtr& texture) override;

        void resetMatrixStacks() override;

        const glm::mat4& projectionMatrix() const override;
        void setProjectionMatrix(const glm::mat4& matrix) override;
        void pushProjectionMatrix() override;
        void popProjectionMatrix() override;

        const glm::mat4& modelViewMatrix() const override;
        void setModelViewMatrix(const glm::mat4& matrix) override;
        void pushModelViewMatrix() override;
        void popModelViewMatrix() override;

        void applyCamera(const ICamera* camera) override;
        void applyCamera(const CameraPtr& camera) override;

        void setBlend(bool flag) override;
        void setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor) override;

        void setDepthTest(bool flag) override;
        void setDepthWrite(bool flag) override;

        void begin(PrimitiveType primitive) override;
        void texCoord(float x, float y) override;
        void texCoord(const glm::vec2& coord) override;
        void color(float r, float g, float b) override;
        void color(float r, float g, float b, float a) override;
        void color(const glm::vec4& color) override;
        size_t vertex(float x, float y) override;
        size_t vertex(float x, float y, float z) override;
        size_t vertex(const glm::vec2& vertex) override;
        size_t vertex(const glm::vec2& vertex, float z) override;
        size_t vertex(const glm::vec3& vertex) override;
        void index(size_t index) override;
        void end() override;

        void flush(bool geometryOnly);

    private:
        Z_VERTEX_FORMAT(Vertex,
            (glm::vec3) position,
            (glm::vec2) texCoord,
            (glm::vec4) color
        )

        RendererPtr mRenderer;
        ShaderPtr mCustomShader;
        ShaderPtr mTexturedShader;
        ShaderPtr mColoredShader;
        std::shared_ptr<MaterialPass> mMaterial;
        VertexBufferPtr mVertexBuffer;
        IndexBufferPtr mIndexBuffer;
        VertexSourcePtr mVertexSource;
        TexturePtr mTexture;
        Atom mTextureUniform;
        Atom mProjectionMatrixUniform;
        Atom mModelViewMatrixUniform;
        PrimitiveType mPrimitiveType;
        Vertex mCurrentVertex;
        std::vector<Vertex> mVertexData;
        std::vector<uint16_t> mIndexData;
        std::vector<glm::mat4> mProjectionMatrixStack;
        std::vector<glm::mat4> mModelViewMatrixStack;
        glm::mat4 mProjectionMatrix;
        glm::mat4 mModelViewMatrix;
        bool mInBeginEnd;
        bool mInDirectRendering;
        bool mMaterialChanged;

        void setPrimitiveType(PrimitiveType primitive);
        size_t emitVertex();

        Z_DISABLE_COPY(ImmediateModeRenderer);
    };
}
