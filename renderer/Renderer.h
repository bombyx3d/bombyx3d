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
#include "Texture.h"
#include "Shader.h"
#include "Sprite.h"
#include "utility/MatrixStack.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace Z
{
    class Renderer
    {
    public:
        static const std::string DEFAULT_DUMMY_SHADER;
        static const std::string DEFAULT_TEXTURED_2D_SHADER;

        Renderer(int viewportWidth, int viewportHeight);
        ~Renderer();

        int viewportWidth() const { return m_ViewportWidth; }
        int viewportHeight() const { return m_ViewportHeight; }
        void setViewportSize(int viewportWidth, int viewportHeight);

        void beginFrame();
        void endFrame();

        void suspend();
        void resume();
        bool isSuspended() const { return (m_Flags & Suspended) != 0; }

        void begin2D(float zNear = -1.0f, float zFar = 1.0f);
        void end2D();

        const MatrixStack& projectionStack() const { return m_ProjectionStack; }
        MatrixStack& projectionStack() { return m_ProjectionStack; }

        const MatrixStack& modelViewStack() const { return m_ModelViewStack; }
        MatrixStack& modelViewStack() { return m_ModelViewStack; }

        void setClearColor(const glm::vec4& color);
        void clear(GL::Bitfield bits);

        const ShaderPtr& dummyShader();
        const ShaderPtr& currentShader() const { return m_CurrentShader; }
        bool setShader(const ShaderPtr& shader);
        void uploadUniforms();

        const TexturePtr& currentTexture0() const { return m_CurrentTexture0; }
        bool setTexture0(const TexturePtr& texture);

        TexturePtr loadTexture(const std::string& name);
        ShaderPtr loadShader(const std::string& name);

        SpritePtr createSpriteFromTexture(const std::string& name);

    private:
        enum {
            Suspended               = 0x00000001,
            ProjectionUniformDirty  = 0x00000002,
            ModelViewUniformDirty   = 0x00000004,
            Texture0UniformDirty    = 0x00000008,
        };

        int m_ViewportWidth;
        int m_ViewportHeight;
        std::unordered_map<std::string, std::weak_ptr<Texture>> m_Textures;
        std::unordered_map<std::string, std::weak_ptr<Shader>> m_Shaders;
        MatrixStack m_ProjectionStack;
        MatrixStack m_ModelViewStack;
        ShaderPtr m_DummyShader;
        ShaderPtr m_CurrentShader;
        TexturePtr m_CurrentTexture0;
        mutable int m_Flags;

        bool useShader(const ShaderPtr& shader);
        void unloadAllResources();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
    };
}
