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
#include "ImmediateModeRenderer.h"
#include "engine/utility/RenderUtils.h"
#include "engine/core/Services.h"
#include "engine/core/AtomTable.h"
#include <vector>
#include <cassert>
#include <cstring>

namespace Engine
{
    static const auto GeometryOnly = true;
    static const auto GeometryAndMaterial = false;

    static const std::vector<std::string> gDefaultColoredShader = {
        "varying vec4 vColor;\n",
        "%vertex\n",
        "attribute vec3 position;\n",
        "attribute vec4 color;\n",
        "uniform mat4 uProjection;\n",
        "uniform mat4 uModelView;\n",
        "void main() {\n",
        "    vColor = color;\n",
        "    gl_Position = uProjection * uModelView * vec4(position, 1.0);\n",
        "}\n",
        "%fragment\n",
        "void main() {\n",
        "    gl_FragColor = vColor;\n",
        "}\n",
    };

    static const std::vector<std::string> gDefaultTexturedShader = {
        "varying vec2 vTexCoord;\n",
        "varying vec4 vColor;\n",
        "%vertex\n",
        "attribute vec3 position;\n",
        "attribute vec2 texCoord;\n",
        "attribute vec4 color;\n",
        "uniform mat4 uProjection;\n",
        "uniform mat4 uModelView;\n",
        "void main() {\n",
        "    vTexCoord = texCoord;\n",
        "    vColor = color;\n",
        "    gl_Position = uProjection * uModelView * vec4(position, 1.0);\n",
        "}\n",
        "%fragment\n",
        "uniform sampler2D uTexture;\n",
        "void main() {\n",
        "    gl_FragColor = texture2D(uTexture, vTexCoord) * vColor;\n",
        "}\n",
    };

    ImmediateModeRenderer::ImmediateModeRenderer(const RendererPtr& renderer)
        : mRenderer(renderer)
        , mMaterial(std::make_shared<MaterialPass>(std::string()))
        , mTextureUniform(AtomTable::getAtom("uTexture"))
        , mProjectionMatrixUniform(AtomTable::getAtom("uProjection"))
        , mModelViewMatrixUniform(AtomTable::getAtom("uModelView"))
        , mPrimitiveType(PrimitiveType::Triangles)
        , mProjectionMatrix(1.0f)
        , mModelViewMatrix(1.0f)
        , mInBeginEnd(false)
        , mInDirectRendering(false)
        , mMaterialChanged(true)
    {
        mColoredShader = Services::resourceManager()->compileShader(&gDefaultColoredShader, "<builtin-colored>");
        mTexturedShader = Services::resourceManager()->compileShader(&gDefaultTexturedShader, "<builtin-textured>");

        mVertexBuffer = Services::rendererResourceFactory()->createVertexBuffer();
        mIndexBuffer = Services::rendererResourceFactory()->createIndexBuffer();
        mVertexSource = RenderUtils::createVertexSource<Vertex>(mVertexBuffer, mIndexBuffer);

        mMaterial->setCullFace(CullFace::None);
        mMaterial->setBlendingEnabled(false);
        mMaterial->setBlendingSourceFactor(BlendFunc::SrcAlpha);
        mMaterial->setBlendingDestinationFactor(BlendFunc::OneMinusSrcAlpha);
        mMaterial->setDepthTestingEnabled(true);
        mMaterial->setDepthWritingEnabled(true);
    }

    ImmediateModeRenderer::~ImmediateModeRenderer()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        flush(GeometryAndMaterial);
    }

    void ImmediateModeRenderer::setClearColor(const glm::vec4& color)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        mRenderer->setClearColor(color);
    }

    void ImmediateModeRenderer::clear()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        flush(GeometryOnly);
        mRenderer->clear();
    }

    IRenderer* ImmediateModeRenderer::beginDirectRendering()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        flush(GeometryAndMaterial);
        mInDirectRendering = true;

        return mRenderer.get();
    }

    void ImmediateModeRenderer::endDirectRendering()
    {
        assert(!mInBeginEnd);
        assert(mInDirectRendering);

        mInDirectRendering = false;
    }

    void ImmediateModeRenderer::setCustomShader(const ShaderPtr& shader)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (mCustomShader != shader) {
            flush(GeometryOnly);
            mCustomShader = shader;
            mMaterialChanged = true;
        }
    }

    void ImmediateModeRenderer::setTexture(const TexturePtr& texture)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (mTexture != texture) {
            flush(GeometryOnly);

            mTexture = texture;
            if (!texture)
                mMaterial->unsetUniform(mTextureUniform);
            else
                mMaterial->setUniform(mTextureUniform, texture);

            mMaterialChanged = true;
        }
    }

    void ImmediateModeRenderer::resetMatrixStacks()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        mProjectionMatrixStack.clear();
        mModelViewMatrixStack.clear();
        setProjectionMatrix(glm::mat4(1.0f));
        setModelViewMatrix(glm::mat4(1.0f));
    }

    const glm::mat4& ImmediateModeRenderer::projectionMatrix() const
    {
        return mProjectionMatrix;
    }

    void ImmediateModeRenderer::setProjectionMatrix(const glm::mat4& matrix)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        mProjectionMatrix = matrix;
        mMaterial->setUniform(mProjectionMatrixUniform, matrix);
        mMaterialChanged = true;
    }

    void ImmediateModeRenderer::pushProjectionMatrix()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        mProjectionMatrixStack.push_back(mProjectionMatrix);
    }

    void ImmediateModeRenderer::popProjectionMatrix()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        assert(!mProjectionMatrixStack.empty());
        setProjectionMatrix(mProjectionMatrixStack.back());
        mProjectionMatrixStack.pop_back();
    }

    const glm::mat4& ImmediateModeRenderer::modelViewMatrix() const
    {
        return mModelViewMatrix;
    }

    void ImmediateModeRenderer::setModelViewMatrix(const glm::mat4& matrix)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        mModelViewMatrix = matrix;
        mMaterial->setUniform(mModelViewMatrixUniform, matrix);
        mMaterialChanged = true;
    }

    void ImmediateModeRenderer::pushModelViewMatrix()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        mModelViewMatrixStack.push_back(mModelViewMatrix);
    }

    void ImmediateModeRenderer::popModelViewMatrix()
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        assert(!mModelViewMatrixStack.empty());
        setModelViewMatrix(mModelViewMatrixStack.back());
        mModelViewMatrixStack.pop_back();
    }

    void ImmediateModeRenderer::setBlend(bool flag)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (flag != mMaterial->blendingEnabled()) {
            flush(GeometryOnly);
            mMaterial->setBlendingEnabled(flag);
            mMaterialChanged = true;
        }
    }

    void ImmediateModeRenderer::setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (srcFactor != mMaterial->blendingSourceFactor() || dstFactor != mMaterial->blendingDestinationFactor()) {
            flush(GeometryOnly);
            mMaterial->setBlendingSourceFactor(srcFactor);
            mMaterial->setBlendingDestinationFactor(dstFactor);
            mMaterialChanged = true;
        }
    }

    void ImmediateModeRenderer::setDepthTest(bool flag)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (flag != mMaterial->depthTestingEnabled()) {
            flush(GeometryOnly);
            mMaterial->setDepthTestingEnabled(flag);
            mMaterialChanged = true;
        }
    }

    void ImmediateModeRenderer::setDepthWrite(bool flag)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (flag != mMaterial->depthWritingEnabled()) {
            flush(GeometryOnly);
            mMaterial->setDepthWritingEnabled(flag);
            mMaterialChanged = true;
        }
    }

    void ImmediateModeRenderer::begin(PrimitiveType primitive)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        setPrimitiveType(primitive);
        memset(&mCurrentVertex, 0, sizeof(mCurrentVertex));
        mInBeginEnd = true;
    }

    void ImmediateModeRenderer::texCoord(float x, float y)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.texCoord = glm::vec2(x, y);
    }

    void ImmediateModeRenderer::texCoord(const glm::vec2& coord)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.texCoord = coord;
    }

    void ImmediateModeRenderer::color(float r, float g, float b)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.color = glm::vec4(r, g, b, 1.0f);
    }

    void ImmediateModeRenderer::color(float r, float g, float b, float a)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.color = glm::vec4(r, g, b, a);
    }

    void ImmediateModeRenderer::color(const glm::vec4& color)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.color = color;
    }

    size_t ImmediateModeRenderer::vertex(float x, float y)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.position = glm::vec3(x, y, 0.0f);
        return emitVertex();
    }

    size_t ImmediateModeRenderer::vertex(float x, float y, float z)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.position = glm::vec3(x, y, z);
        return emitVertex();
    }

    size_t ImmediateModeRenderer::vertex(const glm::vec2& vertex)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.position = glm::vec3(vertex, 0.0f);
        return emitVertex();
    }

    size_t ImmediateModeRenderer::vertex(const glm::vec2& vertex, float z)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.position = glm::vec3(vertex, z);
        return emitVertex();
    }

    size_t ImmediateModeRenderer::vertex(const glm::vec3& vertex)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mCurrentVertex.position = vertex;
        return emitVertex();
    }

    void ImmediateModeRenderer::index(size_t index)
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        assert(index <= MAX_INDEX);
        mIndexData.emplace_back(uint16_t(index));
    }

    void ImmediateModeRenderer::end()
    {
        assert(mInBeginEnd);
        assert(!mInDirectRendering);

        mInBeginEnd = false;
    }

    void ImmediateModeRenderer::flush(bool geometryOnly)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        bool haveGeometry = !mIndexData.empty();

        if (haveGeometry || !geometryOnly) {
            const ShaderPtr* shader;
            if (mCustomShader)
                shader = &mCustomShader;
            else if (mTexture)
                shader = &mTexturedShader;
            else
                shader = &mColoredShader;

            if (mMaterial->shader() != *shader)
                mMaterial->setShader(*shader);

            mMaterial->apply(mRenderer);
        }

        if (haveGeometry) {
            const size_t vertexSize = sizeof(decltype(mVertexData)::value_type);
            mVertexBuffer->setData(mVertexData.data(), mVertexData.size() * vertexSize, BufferUsage::Stream);

            size_t indexCount = mIndexData.size();
            const size_t indexSize = sizeof(decltype(mIndexData)::value_type);
            mIndexBuffer->setData(mIndexData.data(), indexCount * indexSize, BufferUsage::Stream);

            mVertexData.clear();
            mIndexData.clear();

            mRenderer->bindVertexSource(mVertexSource);
            mRenderer->drawPrimitive(mPrimitiveType, 0, indexCount);
        }
    }

    void ImmediateModeRenderer::setPrimitiveType(PrimitiveType primitive)
    {
        assert(!mInBeginEnd);
        assert(!mInDirectRendering);

        if (mPrimitiveType != primitive) {
            flush(GeometryOnly);
            mPrimitiveType = primitive;
        }
    }

    size_t ImmediateModeRenderer::emitVertex()
    {
        size_t index = mVertexData.size();
        assert(index <= MAX_INDEX);

        mVertexData.emplace_back(mCurrentVertex);
        mIndexData.emplace_back(uint16_t(index));

        return index;
    }
}
