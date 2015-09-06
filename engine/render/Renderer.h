
#pragma once
#include "engine/core/macros.h"
#include "engine/core/Atom.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/render/Shader.h"
#include "engine/render/VertexSource.h"
#include "engine/render/Uniform.h"
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Engine
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        ~Renderer();

        void beginFrame() override;
        void endFrame() override;

        void setViewport(int x, int y, int w, int h) override;

        void clear() override;

        ShaderPtr createShader() override;
        TexturePtr createTexture() override;
        VertexBufferPtr createVertexBuffer() override;
        IndexBufferPtr createIndexBuffer() override;
        VertexSourcePtr createVertexSource() override;

        const glm::mat4& projectionMatrix() const override;
        void setProjectionMatrix(const glm::mat4& matrix) override;
        void pushProjectionMatrix() override;
        void popProjectionMatrix() override;

        const glm::mat4& modelViewMatrix() const override;
        void setModelViewMatrix(const glm::mat4& matrix) override;
        void pushModelViewMatrix() override;
        void popModelViewMatrix() override;

        void setUniform(const Atom& name, float value) override;
        void setUniform(const Atom& name, const glm::vec2& value) override;
        void setUniform(const Atom& name, const glm::vec3& value) override;
        void setUniform(const Atom& name, const glm::vec4& value) override;
        void setUniform(const Atom& name, const glm::mat4& value) override;

        void useShader(const ShaderPtr& shader) override;
        void bindVertexSource(const VertexSourcePtr& source) override;

        void drawPrimitive(PrimitiveType primitiveType, size_t first, size_t count) override;

    private:
        Atom mProjectionMatrixUniform;
        Atom mModelViewMatrixUniform;
        glm::mat4 mProjectionMatrix;
        glm::mat4 mModelViewMatrix;
        std::vector<glm::mat4> mProjectionMatrixStack;
        std::vector<glm::mat4> mModelViewMatrixStack;
        std::unordered_map<Atom, Uniform> mUniforms;
        std::shared_ptr<Shader> mCurrentShader;
        std::shared_ptr<VertexSource> mCurrentVertexSource;
        bool mShouldRebindUniforms;
        bool mShouldRebindAttributes;

        bool setupDrawCall();
        void bindUniforms();

        void resetOpenGLBindings();

        Z_DISABLE_COPY(Renderer);
    };
}
