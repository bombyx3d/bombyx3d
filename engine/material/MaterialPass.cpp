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
#include "MaterialPass.h"
#include "engine/core/AtomTable.h"

#ifdef _MSC_VER
 #pragma warning(disable:4316)      // object allocated on the heap may not be aligned 16
#endif

namespace Engine
{
    struct MaterialPass::UniformValue
    {
        virtual ~UniformValue() = default;
        virtual void upload(const RendererPtr& renderer, Atom name) const = 0;
    };

    template <class TYPE> struct MaterialPass::UniformValueT : public UniformValue
    {
        TYPE value;
        UniformValueT(const TYPE& v) : value(v) {}
        void upload(const RendererPtr& renderer, Atom name) const override { renderer->setUniform(name, value); }
    };


    MaterialPass::MaterialPass(const std::string& passName)
        : mName(passName)
    {
    }

    MaterialPass::~MaterialPass()
    {
    }

    const std::string& MaterialPass::name() const
    {
        return mName;
    }

    void MaterialPass::setUniform(const std::string& name, float value)
    {
        setUniform(AtomTable::instance()->getAtom(name), value);
    }

    void MaterialPass::setUniform(const std::string& name, const glm::vec2& value)
    {
        setUniform(AtomTable::instance()->getAtom(name), value);
    }

    void MaterialPass::setUniform(const std::string& name, const glm::vec3& value)
    {
        setUniform(AtomTable::instance()->getAtom(name), value);
    }

    void MaterialPass::setUniform(const std::string& name, const glm::vec4& value)
    {
        setUniform(AtomTable::instance()->getAtom(name), value);
    }

    void MaterialPass::setUniform(const std::string& name, const TexturePtr& value)
    {
        setUniform(AtomTable::instance()->getAtom(name), value);
    }

    void MaterialPass::setUniform(Atom name, float value)
    {
        size_t index = uniformIndex(name);
        mUniforms[index].second.reset(new UniformValueT<float>(value));
    }

    void MaterialPass::setUniform(Atom name, const glm::vec2& value)
    {
        size_t index = uniformIndex(name);
        mUniforms[index].second.reset(new UniformValueT<glm::vec2>(value));
    }

    void MaterialPass::setUniform(Atom name, const glm::vec3& value)
    {
        size_t index = uniformIndex(name);
        mUniforms[index].second.reset(new UniformValueT<glm::vec3>(value));
    }

    void MaterialPass::setUniform(Atom name, const glm::vec4& value)
    {
        size_t index = uniformIndex(name);
        mUniforms[index].second.reset(new UniformValueT<glm::vec4>(value));
    }

    void MaterialPass::setUniform(Atom name, const TexturePtr& value)
    {
        size_t index = uniformIndex(name);
        mUniforms[index].second.reset(new UniformValueT<TexturePtr>(value));
    }

    void MaterialPass::apply(const RendererPtr& renderer) const
    {
        renderer->useShader(mShader);

        renderer->setCullFace(mCullFace);
        renderer->setDepthTestingEnabled((mFlags & DepthTest) != 0);
        renderer->setDepthWritingEnabled((mFlags & DepthWrite) != 0);

        bool blend = (mFlags & Blend) != 0;
        renderer->setBlendingEnabled(blend);
        if (blend)
            renderer->setBlendFunc(mBlendingSourceFactor, mBlendingDestinationFactor);

        for (const auto& it : mUniforms)
            it.second->upload(renderer, it.first);
    }

    size_t MaterialPass::uniformIndex(Atom name)
    {
        auto it = mUniformNames.find(name);
        if (it != mUniformNames.end())
            return it->second;

        size_t index = mUniforms.size();
        mUniforms.resize(index + 1);
        mUniforms[index].first = name;
        mUniformNames.emplace(name, index);

        return index;
    }
}
