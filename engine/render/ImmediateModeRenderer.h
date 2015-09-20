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
#include "engine/interfaces/render/IIndexBuffer.h"
#include "engine/interfaces/render/IVertexBuffer.h"
#include "engine/interfaces/render/IVertexSource.h"
#include "engine/interfaces/render/IShader.h"
#include "engine/mesh/VertexFormat.h"
#include "engine/material/MaterialPass.h"
#include "engine/math/BoundingBox.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <cstdint>

namespace Engine
{
    class ImmediateModeRenderer
    {
    public:
        static const size_t MAX_INDEX = 65534;

        ImmediateModeRenderer();
        ~ImmediateModeRenderer();

        const ShaderPtr& customShader() const { return mCustomShader; }
        void setCustomShader(const ShaderPtr& shader);

        const TexturePtr& texture() const { return mTexture; }
        void setTexture(const TexturePtr& texture);

        void setBlend(bool flag);
        void setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor);

        void begin(PrimitiveType primitive);
        void texCoord(float x, float y);
        void texCoord(const glm::vec2& coord);
        void color(float r, float g, float b);
        void color(float r, float g, float b, float a);
        void color(const glm::vec4& color);
        size_t vertex(float x, float y);
        size_t vertex(float x, float y, float z);
        size_t vertex(const glm::vec3& vertex);
        void index(size_t index);
        void end();

        void flush();

    private:
        Z_VERTEX_FORMAT(Vertex,
            (glm::vec3) position,
            (glm::vec2) texCoord,
            (glm::vec4) color
        )

        ShaderPtr mCustomShader;
        ShaderPtr mTexturedShader;
        ShaderPtr mColoredShader;
        std::shared_ptr<MaterialPass> mMaterial;
        VertexBufferPtr mVertexBuffer;
        IndexBufferPtr mIndexBuffer;
        VertexSourcePtr mVertexSource;
        TexturePtr mTexture;
        Atom mTextureUniform;
        PrimitiveType mPrimitiveType;
        bool mInBeginEnd;
        Vertex mCurrentVertex;
        std::vector<Vertex> mVertexData;
        std::vector<uint16_t> mIndexData;

        void setPrimitiveType(PrimitiveType primitive);
        size_t emitVertex();

        Z_DISABLE_COPY(ImmediateModeRenderer);
    };
}
