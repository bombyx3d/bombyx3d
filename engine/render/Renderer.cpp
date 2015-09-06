#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Buffer.h"
#include "engine/core/Log.h"
#include "engine/core/AtomTable.h"
#include "opengl.h"
#include <cassert>

namespace Engine
{
    Renderer::Renderer()
        : mProjectionMatrixUniform(AtomTable::instance()->getAtom("uProjection"))
        , mModelViewMatrixUniform(AtomTable::instance()->getAtom("uModelView"))
        , mProjectionMatrix(1.0f)
        , mModelViewMatrix(1.0f)
    {
        glClearColor(0.7f, 0.3f, 0.1f, 1.0f);
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::beginFrame()
    {
        mProjectionMatrix = glm::mat4(1.0f);
        mModelViewMatrix = glm::mat4(1.0f);
        mProjectionMatrixStack.clear();
        mModelViewMatrixStack.clear();

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

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    ShaderPtr Renderer::createShader()
    {
        return std::make_shared<Shader>();
    }

    TexturePtr Renderer::createTexture()
    {
        return std::make_shared<Texture>();
    }

    VertexBufferPtr Renderer::createVertexBuffer()
    {
        return std::make_shared<Buffer>(GL_ARRAY_BUFFER);
    }

    IndexBufferPtr Renderer::createIndexBuffer()
    {
        return std::make_shared<Buffer>(GL_ELEMENT_ARRAY_BUFFER);
    }

    const glm::mat4& Renderer::projectionMatrix() const
    {
        return mProjectionMatrix;
    }

    void Renderer::setProjectionMatrix(const glm::mat4& matrix)
    {
        mProjectionMatrix = matrix;
        setUniform(mProjectionMatrixUniform, matrix);
    }

    void Renderer::pushProjectionMatrix()
    {
        mProjectionMatrixStack.push_back(mProjectionMatrix);
    }

    void Renderer::popProjectionMatrix()
    {
        assert(!mProjectionMatrixStack.empty());
        mProjectionMatrixStack.pop_back();
    }

    const glm::mat4& Renderer::modelViewMatrix() const
    {
        return mModelViewMatrix;
    }

    void Renderer::setModelViewMatrix(const glm::mat4& matrix)
    {
        mModelViewMatrix = matrix;
        setUniform(mModelViewMatrixUniform, matrix);
    }

    void Renderer::pushModelViewMatrix()
    {
        mModelViewMatrixStack.push_back(mModelViewMatrix);
    }

    void Renderer::popModelViewMatrix()
    {
        assert(!mModelViewMatrixStack.empty());
        mModelViewMatrixStack.pop_back();
    }

    void Renderer::setUniform(const Atom& name, float value)
    {
        mUniforms[name].setFloat(value);
    }

    void Renderer::setUniform(const Atom& name, const glm::vec2& value)
    {
        mUniforms[name].setVec2(value);
    }

    void Renderer::setUniform(const Atom& name, const glm::vec3& value)
    {
        mUniforms[name].setVec3(value);
    }

    void Renderer::setUniform(const Atom& name, const glm::vec4& value)
    {
        mUniforms[name].setVec4(value);
    }

    void Renderer::setUniform(const Atom& name, const glm::mat4& value)
    {
        mUniforms[name].setMat4(value);
    }

    void Renderer::useShader(const ShaderPtr& shader)
    {
        if (mCurrentShader != shader) {
            mCurrentShader = shader;
            if (!mCurrentShader)
                glUseProgram(0);
            else
                glUseProgram(GLuint(static_cast<Shader&>(*mCurrentShader).handle()));
        }
    }

    void Renderer::bindUniforms()
    {
        if (!mCurrentShader)
            return;

        const auto& currentShader = static_cast<Shader&>(*mCurrentShader);
        for (const auto& uniform : currentShader.uniforms()) {
            bool bound = false;

            auto it = mUniforms.find(uniform.first);
            if (it != mUniforms.end())
                bound = it->second.upload(uniform.second);

            if (!bound)
                Z_LOGW("Missing value for uniform \"" << uniform.first.text() << "\".");
        }
    }

    void Renderer::resetOpenGLBindings()
    {
        mCurrentShader.reset();
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
