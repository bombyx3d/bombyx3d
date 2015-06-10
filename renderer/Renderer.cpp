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
#include "Renderer.h"
#include "utility/debug.h"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/colored2d.glsl.h"
#include "shaders/textured2d.glsl.h"

namespace Z
{
    namespace
    {
        class DummyShader : public Shader
        {
        public:
            void reload() override {}
        };
    }

    #define BUILTIN_PREFIX "builtin:"
    #define COLORED2D_GLSL "colored2d.glsl"
    #define TEXTURED2D_GLSL "textured2d.glsl"

    const std::string Renderer::DEFAULT_COLORED_2D_SHADER = BUILTIN_PREFIX COLORED2D_GLSL;
    const std::string Renderer::DEFAULT_TEXTURED_2D_SHADER = BUILTIN_PREFIX TEXTURED2D_GLSL;

    Renderer::Renderer(int viewportWidth, int viewportHeight)
        : m_ViewportWidth(viewportWidth)
        , m_ViewportHeight(viewportHeight)
        , m_ProjectionStack(4, [this](){ m_Flags |= ProjectionUniformDirty; })
        , m_ModelViewStack(32, [this](){ m_Flags |= ModelViewUniformDirty; })
        , m_Flags(0)
    {
        gl::InitWrappers();
    }

    Renderer::~Renderer()
    {
        unloadAllResources();
    }

    void Renderer::setViewportSize(int viewportWidth, int viewportHeight)
    {
        m_ViewportWidth = viewportWidth;
        m_ViewportHeight = viewportHeight;
    }

    void Renderer::beginFrame()
    {
        gl::Viewport(0, 0, m_ViewportWidth, m_ViewportHeight);

        m_CurrentTexture0 = nullptr;
        gl::ActiveTexture(GL::TEXTURE0);
        GLTexture::unbindAll();

        m_CurrentShader = dummyShader();
        m_Flags |= AllUniformsDirty;
        GLProgram::unbindAll();

        m_ProjectionStack.reset();
        m_ModelViewStack.reset();

        m_ColorStack.resize(1);
        m_ColorStack[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Renderer::endFrame()
    {
        GLProgram::unbindAll();

        gl::ActiveTexture(GL::TEXTURE0);
        GLTexture::unbindAll();

        m_CurrentShader.reset();
        m_CurrentTexture0.reset();
    }

    void Renderer::suspend()
    {
        if (!isSuspended()) {
            m_Flags |= Suspended;
            unloadAllResources();
        }
    }

    void Renderer::resume()
    {
        if (isSuspended()) {
            m_Flags &= ~Suspended;
            // FIXME
        }
    }

    void Renderer::begin2D(float zNear, float zFar)
    {
        float w = float(m_ViewportWidth);
        float h = float(m_ViewportHeight);
        m_ProjectionStack.pushReplace(glm::ortho(0.0f, w, h, 0.0f, zNear, zFar));
    }

    void Renderer::end2D()
    {
        m_ProjectionStack.pop();
    }

    void Renderer::setClearColor(const glm::vec4& color)
    {
        gl::ClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::clear(GL::Bitfield bits)
    {
        gl::Clear(bits);
    }

    void Renderer::pushApplyColor(const glm::vec4& color)
    {
        Z_ASSERT(!m_ColorStack.empty());
        if (color == glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {
            m_ColorStack.emplace_back(m_ColorStack.back());
        } else {
            m_ColorStack.emplace_back(m_ColorStack.back() * color);
            m_Flags |= ColorUniformDirty;
        }
    }

    void Renderer::pushReplaceColor(const glm::vec4& color)
    {
        Z_ASSERT(!m_ColorStack.empty());
        if (color != m_ColorStack.back())
            m_Flags |= ColorUniformDirty;
        m_ColorStack.emplace_back(color);
    }

    void Renderer::popColor()
    {
        size_t colorStackSize = m_ColorStack.size();
        Z_CHECK(colorStackSize > 1);
        if (colorStackSize > 1) {
            if (m_ColorStack[colorStackSize - 2] != m_ColorStack[colorStackSize - 1])
                m_Flags |= ColorUniformDirty;
            m_ColorStack.pop_back();
        }
    }

    const ShaderPtr& Renderer::dummyShader()
    {
        if (!m_DummyShader)
            m_DummyShader = std::make_shared<DummyShader>();
        return m_DummyShader;
    }

    bool Renderer::setShader(const ShaderPtr& shader)
    {
        Z_CHECK(shader != nullptr);
        if (!shader)
            return false;

        if (m_CurrentShader != shader) {
            if (!useShader(shader)) {
                useShader(m_CurrentShader);
                return false;
            }
            m_CurrentShader = shader;
        }

        return true;
    }

    void Renderer::uploadUniforms()
    {
        if (m_Flags & ProjectionUniformDirty) {
            if (m_CurrentShader->hasUniform(Shader::ProjectionMatrixUniform)) {
                auto location = m_CurrentShader->getUniformLocation(Shader::ProjectionMatrixUniform);
                gl::UniformMatrix4fv(location, 1, GL::FALSE, &projectionStack().top()[0][0]);
            }
            m_Flags &= ~ProjectionUniformDirty;
        }

        if (m_Flags & ModelViewUniformDirty) {
            if (m_CurrentShader->hasUniform(Shader::ModelViewMatrixUniform)) {
                auto location = m_CurrentShader->getUniformLocation(Shader::ModelViewMatrixUniform);
                gl::UniformMatrix4fv(location, 1, GL::FALSE, &modelViewStack().top()[0][0]);
            }
            m_Flags &= ~ModelViewUniformDirty;
        }

        Z_ASSERT(!m_ColorStack.empty());
        if (m_Flags & ColorUniformDirty) {
            if (m_CurrentShader->hasUniform(Shader::ColorUniform)) {
                auto location = m_CurrentShader->getUniformLocation(Shader::ColorUniform);
                gl::Uniform4fv(location, 1, &m_ColorStack.back().x);
            }
            m_Flags &= ~ColorUniformDirty;
        }

        if (m_Flags & Texture0UniformDirty) {
            if (m_CurrentShader->hasUniform(Shader::Texture0Uniform)) {
                auto location = m_CurrentShader->getUniformLocation(Shader::Texture0Uniform);
                gl::Uniform1i(location, 0);
            }
            m_Flags &= ~Texture0UniformDirty;
        }
    }

    bool Renderer::setTexture0(const TexturePtr& texture)
    {
        if (m_CurrentTexture0 != texture) {
            gl::ActiveTexture(GL::TEXTURE0);
            if (!texture)
                GLTexture::unbindAll();
            else if (!texture->bind())
                return false;
            m_CurrentTexture0 = texture;
            m_Flags |= Texture0UniformDirty;
        }
        return true;
    }

    TexturePtr Renderer::loadTexture(const std::string& name)
    {
        auto& ref = m_Textures[name];
        TexturePtr texture = ref.lock();
        if (!texture) {
            texture = std::make_shared<TextureFromFile>(name);
            if (!isSuspended())
                texture->reload();
            ref = texture;
        }
        return texture;
    }

    ShaderPtr Renderer::loadShader(const std::string& name)
    {
        auto& ref = m_Shaders[name];
        ShaderPtr shader = ref.lock();
        if (!shader) {
            if (name.substr(0, sizeof(BUILTIN_PREFIX) - 1) != BUILTIN_PREFIX)
                shader = std::make_shared<ShaderFromFile>(name);
            else {
                struct ShaderSource { const void* data; size_t size; };
                static const std::unordered_map<std::string, ShaderSource> builtinShaders = {
                    { COLORED2D_GLSL, { colored2d_glsl, colored2d_glsl_size } },
                    { TEXTURED2D_GLSL, { textured2d_glsl, textured2d_glsl_size } },
                };

                std::string builtinShaderName = name.substr(sizeof(BUILTIN_PREFIX) - 1);
                auto it = builtinShaders.find(builtinShaderName);
                if (it != builtinShaders.end()) {
                    shader = std::make_shared<ShaderFromStaticMemory>
                        (it->second.data, it->second.size, builtinShaderName);
                } else {
                    Z_LOG("There is no builtin shader named \"" << builtinShaderName << "\".");
                    shader = m_DummyShader;
                }
            }

            if (!isSuspended())
                shader->reload();

            ref = shader;
        }
        return shader;
    }

    SpritePtr Renderer::createSpriteFromTexture(const std::string& name)
    {
        TexturePtr texture = loadTexture(name);
        return std::make_shared<Sprite>(texture);
    }

    SpritePtr Renderer::createSpriteFromTextureAndShader(const std::string& textureName, const std::string& shaderName)
    {
        TexturePtr texture = loadTexture(textureName);
        ShaderPtr shader = loadShader(shaderName);
        return std::make_shared<Sprite>(texture, shader);
    }

    void Renderer::drawRect(const Quad& rect, const glm::vec4& color)
    {
        uploadUniforms();

        const glm::vec2 quads[] = { rect.topLeft, rect.topRight, rect.bottomRight, rect.bottomLeft };
        gl::EnableVertexAttribArray(Shader::PositionAttribute);
        gl::VertexAttribPointer(Shader::PositionAttribute, 2, GL::FLOAT, GL::FALSE, 0, &quads[0].x);

        gl::VertexAttrib4fv(Shader::ColorAttribute, &color.x);

        gl::DrawArrays(GL::LINE_LOOP, 0, 4);

        gl::DisableVertexAttribArray(Shader::PositionAttribute);
    }

    void Renderer::drawQuad(const Quad& rect)
    {
        uploadUniforms();

        gl::EnableVertexAttribArray(Shader::PositionAttribute);
        gl::VertexAttribPointer(Shader::PositionAttribute, 2, GL::FLOAT, GL::FALSE, 0, &rect.topLeft.x);

        gl::VertexAttrib4f(Shader::ColorAttribute, 1.0f, 1.0f, 1.0f, 1.0f);

        gl::DrawArrays(GL::TRIANGLE_STRIP, 0, 4);

        gl::DisableVertexAttribArray(Shader::PositionAttribute);
    }

    void Renderer::drawQuad(const Quad& rect, const Quad& texCoord)
    {
        uploadUniforms();

        gl::EnableVertexAttribArray(Shader::PositionAttribute);
        gl::EnableVertexAttribArray(Shader::TexCoord0Attribute);
        gl::VertexAttribPointer(Shader::PositionAttribute, 2, GL::FLOAT, GL::FALSE, 0, &rect.topLeft.x);
        gl::VertexAttribPointer(Shader::TexCoord0Attribute, 2, GL::FLOAT, GL::FALSE, 0, &texCoord.topLeft.x);

        gl::DrawArrays(GL::TRIANGLE_STRIP, 0, 4);

        gl::DisableVertexAttribArray(Shader::PositionAttribute);
        gl::DisableVertexAttribArray(Shader::TexCoord0Attribute);
    }

    bool Renderer::useShader(const ShaderPtr& shader)
    {
        m_Flags |= AllUniformsDirty;
        return shader->use();
    }

    void Renderer::unloadAllResources()
    {
        Z_LOG("Unloading all renderer resources.");

        for (auto it = m_Textures.begin(); it != m_Textures.end(); ) {
            auto ptr = it->second.lock();
            if (!ptr)
                it = m_Textures.erase(it);
            else {
                ++it;
                ptr->unload();
            }
        }

        for (auto it = m_Shaders.begin(); it != m_Shaders.end(); ) {
            auto ptr = it->second.lock();
            if (!ptr)
                it = m_Shaders.erase(it);
            else {
                ++it;
                ptr->unload();
            }
        }
    }
}
