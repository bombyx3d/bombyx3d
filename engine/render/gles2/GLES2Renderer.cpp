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
#include "GLES2Renderer.h"
#include "engine/render/gles2/GLES2Shader.h"
#include "engine/render/gles2/GLES2Texture.h"
#include "engine/render/gles2/GLES2Buffer.h"
#include "engine/render/gles2/GLES2VertexSource.h"
#include "engine/core/Log.h"
#include "opengl.h"
#include <cassert>

namespace B3D
{
    Renderer::Renderer()
        : mShouldRebindUniforms(true)
        , mShouldRebindAttributes(true)
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::beginFrame()
    {
        for (auto& it : mUniforms)
            it.second.reset();
        resetOpenGLBindings();
    }

    void Renderer::endFrame()
    {
        resetOpenGLBindings();
    }

    void Renderer::setViewport(int x, int y, int w, int h)
    {
        glViewport(x, y, w, h);
    }

    void Renderer::setClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::clear()
    {
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    ShaderPtr Renderer::createShader()
    {
        return std::make_shared<GLES2Shader>();
    }

    TexturePtr Renderer::createTexture()
    {
        return std::make_shared<GLES2Texture>();
    }

    VertexBufferPtr Renderer::createVertexBuffer()
    {
        return std::make_shared<GLES2Buffer>(GL_ARRAY_BUFFER);
    }

    IndexBufferPtr Renderer::createIndexBuffer()
    {
        return std::make_shared<GLES2Buffer>(GL_ELEMENT_ARRAY_BUFFER);
    }

    VertexSourcePtr Renderer::createVertexSource()
    {
        return std::make_shared<GLES2VertexSource>();
    }

    void Renderer::setCullFace(CullFace face)
    {
        if (face == CullFace::None)
            glDisable(GL_CULL_FACE);
        else {
            glEnable(GL_CULL_FACE);
            glCullFace(cullFaceToGL(face));
        }
    }

    void Renderer::setFrontFace(FrontFace face)
    {
        glFrontFace(frontFaceToGL(face));
    }

    void Renderer::setBlendingEnabled(bool value)
    {
        if (value)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    void Renderer::setBlendFunc(BlendFunc srcFactor, BlendFunc dstFactor)
    {
        glBlendFunc(blendFuncToGL(srcFactor), blendFuncToGL(dstFactor));
    }

    void Renderer::setDepthTestingEnabled(bool value)
    {
        if (value)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void Renderer::setDepthWritingEnabled(bool value)
    {
        if (value)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }

    void Renderer::setUniform(const Atom& name, float value)
    {
        mUniforms[name].setFloat(value);
        mShouldRebindUniforms = true;
    }

    void Renderer::setUniform(const Atom& name, const glm::vec2& value)
    {
        mUniforms[name].setVec2(value);
        mShouldRebindUniforms = true;
    }

    void Renderer::setUniform(const Atom& name, const glm::vec3& value)
    {
        mUniforms[name].setVec3(value);
        mShouldRebindUniforms = true;
    }

    void Renderer::setUniform(const Atom& name, const glm::vec4& value)
    {
        mUniforms[name].setVec4(value);
        mShouldRebindUniforms = true;
    }

    void Renderer::setUniform(const Atom& name, const glm::mat4& value)
    {
        mUniforms[name].setMat4(value);
        mShouldRebindUniforms = true;
    }

    void Renderer::setUniform(const Atom& name, const TexturePtr& texture)
    {
        mUniforms[name].setTexture(texture);
        mShouldRebindUniforms = true;
    }

    void Renderer::useShader(const ShaderPtr& shader)
    {
        if (mCurrentShader != shader) {
            if (mCurrentVertexSource)
                mCurrentVertexSource->unbind();

            mCurrentShader = std::static_pointer_cast<GLES2Shader>(shader);
            if (!mCurrentShader)
                glUseProgram(0);
            else
                glUseProgram(GLuint(mCurrentShader->handle()));

            mShouldRebindUniforms = true;
            mShouldRebindAttributes = true;
        }
    }

    void Renderer::bindVertexSource(const VertexSourcePtr& source)
    {
        if (mCurrentVertexSource != source) {
            if (mCurrentVertexSource)
                mCurrentVertexSource->unbind();
            mCurrentVertexSource = std::static_pointer_cast<GLES2VertexSource>(source);
            mShouldRebindAttributes = true;
        }
    }

    void Renderer::drawPrimitive(PrimitiveType primitiveType, size_t first, size_t count)
    {
        if (!setupDrawCall())
            return;

        GLenum mode = primitiveTypeToGL(primitiveType);
        if (!mCurrentVertexSource->indexBuffer())
            glDrawArrays(mode, GLint(first), GLsizei(count));
        else {
            void* offset = reinterpret_cast<void*>(first * sizeof(uint16_t));
            glDrawElements(mode, GLsizei(count), GL_UNSIGNED_SHORT, offset);
        }
    }

    bool Renderer::setupDrawCall()
    {
        assert(mCurrentShader != nullptr);
        assert(mCurrentVertexSource != nullptr);
        if (!mCurrentShader || !mCurrentVertexSource)
            return false;

        if (mShouldRebindAttributes) {
            mCurrentVertexSource->bind(*mCurrentShader);
            mShouldRebindAttributes = false;
        }

        if (mShouldRebindUniforms) {
            bindUniforms();
            mShouldRebindUniforms = false;
        }

        return true;
    }

    void Renderer::bindUniforms()
    {
        if (!mCurrentShader)
            return;

        int textureCount = 0;
        for (const auto& uniform : mCurrentShader->uniforms()) {
            bool bound = false;

            auto it = mUniforms.find(uniform.first);
            if (it != mUniforms.end())
                bound = it->second.upload(uniform.second, &textureCount);

            if (!bound)
                B3D_LOGW("Missing value for uniform \"" << uniform.first.text() << "\".");
        }
    }

    void Renderer::resetOpenGLBindings()
    {
        if (mCurrentVertexSource)
            mCurrentVertexSource->unbind();

        mCurrentShader.reset();
        mCurrentVertexSource.reset();

        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        GLint textureUnitCount = 0;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnitCount);

        for (GLint i = 0; i < textureUnitCount; i++) {
            glActiveTexture(GLenum(GL_TEXTURE0 + i));
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        mShouldRebindUniforms = true;
        mShouldRebindAttributes = true;
    }
}
