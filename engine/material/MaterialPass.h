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
#include "engine/interfaces/material/IMaterialPass.h"
#include <vector>
#include <memory>
#include <unordered_map>

namespace Engine
{
    class MaterialPass : public IMaterialPass
    {
    public:
        explicit MaterialPass(const std::string& passName);
        ~MaterialPass();

        const std::string& name() const override;

        CullFace cullFace() const { return mCullFace; }
        void setCullFace(CullFace face) { mCullFace = face; }

        bool blendingEnabled() const { return (mFlags & Blend) != 0; }
        bool depthTestingEnabled() const { return (mFlags & DepthTest) != 0; }
        bool depthWritingEnabled() const { return (mFlags & DepthWrite) != 0; }
        void setBlendingEnabled(bool value) { setFlag(Blend, value); }
        void setDepthTestingEnabled(bool value) { setFlag(DepthTest, value); }
        void setDepthWritingEnabled(bool value) { setFlag(DepthWrite, value); }

        BlendFunc blendingSourceFactor() const { return mBlendingSourceFactor; }
        BlendFunc blendingDestinationFactor() const { return mBlendingDestinationFactor; }
        void setBlendingSourceFactor(BlendFunc factor) { mBlendingSourceFactor = factor; }
        void setBlendingDestinationFactor(BlendFunc factor) { mBlendingDestinationFactor = factor; }

        const ShaderPtr& shader() const;
        void setShader(const std::string& fileName);
        void setShader(const ShaderPtr& shader);

        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const glm::vec2& value);
        void setUniform(const std::string& name, const glm::vec3& value);
        void setUniform(const std::string& name, const glm::vec4& value);
        void setUniform(const std::string& name, const glm::mat4& value);
        void setUniform(const std::string& name, const std::string& textureName);
        void setUniform(const std::string& name, const TexturePtr& texture);
        void unsetUniform(const std::string& name);

        void setUniform(Atom name, float value);
        void setUniform(Atom name, const glm::vec2& value);
        void setUniform(Atom name, const glm::vec3& value);
        void setUniform(Atom name, const glm::vec4& value);
        void setUniform(Atom name, const glm::mat4& value);
        void setUniform(Atom name, const std::string& textureName);
        void setUniform(Atom name, const TexturePtr& texture);
        void unsetUniform(Atom name);

        void apply(const RendererPtr& renderer) const override;
        void apply(IRenderer* renderer) const override;

        void loadPendingResources(bool async) override;

    private:
        enum Flag {
            Blend = 0x00000001,
            DepthTest = 0x00000002,
            DepthWrite = 0x00000004,
        };

        struct UniformValue;
        template <class TYPE> struct UniformValueT;
        class UniformTexture;

        std::string mName;
        mutable ShaderPtr mShader;
        mutable std::unique_ptr<std::string> mShaderPath;
        unsigned mFlags = 0;
        BlendFunc mBlendingSourceFactor = BlendFunc::SrcAlpha;
        BlendFunc mBlendingDestinationFactor = BlendFunc::OneMinusSrcAlpha;
        CullFace mCullFace = CullFace::Back;
        std::vector<std::pair<Atom, std::unique_ptr<UniformValue>>> mUniforms;
        std::unordered_map<Atom, size_t> mUniformNames;

        void setFlag(Flag flag, bool value) { mFlags = value ? (mFlags | flag) : (mFlags & ~flag); }
        void ensureShaderLoaded(bool async) const;
        size_t uniformIndex(Atom name);

        B3D_DISABLE_COPY(MaterialPass);
    };
}
