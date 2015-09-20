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
#include <vector>
#include <cassert>

namespace Engine
{
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

    ImmediateModeRenderer::ImmediateModeRenderer()
        : mMaterial(std::make_shared<MaterialPass>(std::string()))
        , mPrimitiveType(PrimitiveType::Triangles)
        , mInBeginEnd(false)
    {
        mTextureUniform = AtomTable::getAtom("uTexture");

        mColoredShader = Services::resourceManager()->compileShader(&gDefaultColoredShader, "<builtin-colored>");
        mTexturedShader = Services::resourceManager()->compileShader(&gDefaultTexturedShader, "<builtin-textured>");

        mVertexBuffer = Services::renderer()->createVertexBuffer();
        mIndexBuffer = Services::renderer()->createIndexBuffer();
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
        flush();
    }

    void ImmediateModeRenderer::setCustomShader(const ShaderPtr& shader)
    {
        assert(!mInBeginEnd);
        if (mCustomShader != shader) {
            flush();
            mCustomShader = shader;
        }
    }

    void ImmediateModeRenderer::setTexture(const TexturePtr& texture)
    {
        assert(!mInBeginEnd);
        if (mTexture != texture) {
            flush();

            mTexture = texture;
            if (!texture)
                mMaterial->unsetUniform(mTextureUniform);
            else
                mMaterial->setUniform(mTextureUniform, texture);
        }
    }

    void ImmediateModeRenderer::setBlend(bool flag)
    {
        if (flag != mMaterial->blendingEnabled()) {
            flush();
            mMaterial->setBlendingEnabled(flag);
        }
    }

    void ImmediateModeRenderer::setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor)
    {
        if (srcFactor != mMaterial->blendingSourceFactor() || dstFactor != mMaterial->blendingDestinationFactor()) {
            flush();
            mMaterial->setBlendingSourceFactor(srcFactor);
            mMaterial->setBlendingDestinationFactor(dstFactor);
        }
    }

    void ImmediateModeRenderer::begin(PrimitiveType primitive)
    {
        assert(!mInBeginEnd);
        setPrimitiveType(primitive);
        mInBeginEnd = true;
    }

    void ImmediateModeRenderer::texCoord(float x, float y)
    {
        assert(mInBeginEnd);
        mCurrentVertex.texCoord = glm::vec2(x, y);
    }

    void ImmediateModeRenderer::texCoord(const glm::vec2& coord)
    {
        assert(mInBeginEnd);
        mCurrentVertex.texCoord = coord;
    }

    void ImmediateModeRenderer::color(float r, float g, float b)
    {
        assert(mInBeginEnd);
        mCurrentVertex.color = glm::vec4(r, g, b, 1.0f);
    }

    void ImmediateModeRenderer::color(float r, float g, float b, float a)
    {
        assert(mInBeginEnd);
        mCurrentVertex.color = glm::vec4(r, g, b, a);
    }

    void ImmediateModeRenderer::color(const glm::vec4& color)
    {
        assert(mInBeginEnd);
        mCurrentVertex.color = color;
    }

    size_t ImmediateModeRenderer::vertex(float x, float y)
    {
        assert(mInBeginEnd);
        mCurrentVertex.position = glm::vec3(x, y, 0.0f);
        return emitVertex();
    }

    size_t ImmediateModeRenderer::vertex(float x, float y, float z)
    {
        assert(mInBeginEnd);
        mCurrentVertex.position = glm::vec3(x, y, z);
        return emitVertex();
    }

    size_t ImmediateModeRenderer::vertex(const glm::vec3& vertex)
    {
        assert(mInBeginEnd);
        mCurrentVertex.position = vertex;
        return emitVertex();
    }

    void ImmediateModeRenderer::index(size_t index)
    {
        assert(mInBeginEnd);
        assert(index <= MAX_INDEX);
        mIndexData.emplace_back(uint16_t(index));
    }

    void ImmediateModeRenderer::end()
    {
        assert(mInBeginEnd);
        mInBeginEnd = false;
    }

    void ImmediateModeRenderer::flush()
    {
        assert(!mInBeginEnd);
        if (!mIndexData.empty()) {
            const size_t vertexSize = sizeof(decltype(mVertexData)::value_type);
            mVertexBuffer->setData(mVertexData.data(), mVertexData.size() * vertexSize, BufferUsage::Stream);

            size_t indexCount = mIndexData.size();
            const size_t indexSize = sizeof(decltype(mIndexData)::value_type);
            mIndexBuffer->setData(mIndexData.data(), indexCount * indexSize, BufferUsage::Stream);

            mVertexData.clear();
            mIndexData.clear();

            const ShaderPtr* shader;
            if (mCustomShader)
                shader = &mCustomShader;
            else if (mTexture)
                shader = &mTexturedShader;
            else
                shader = &mColoredShader;

            if (mMaterial->shader() != *shader)
                mMaterial->setShader(*shader);

            mMaterial->apply(Services::renderer());
            Services::renderer()->bindVertexSource(mVertexSource);
            Services::renderer()->drawPrimitive(mPrimitiveType, 0, indexCount);
        }
    }

    void ImmediateModeRenderer::setPrimitiveType(PrimitiveType primitive)
    {
        assert(!mInBeginEnd);
        if (mPrimitiveType != primitive) {
            flush();
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
