
#pragma once
#include "engine/utility/Singleton.h"
#include "engine/core/Atom.h"
#include "engine/interfaces/render/IShader.h"
#include "engine/interfaces/render/ITexture.h"
#include "engine/interfaces/render/IVertexBuffer.h"
#include "engine/interfaces/render/IIndexBuffer.h"
#include "engine/interfaces/render/IVertexSource.h"
#include <glm/glm.hpp>

namespace Engine
{
    enum class PrimitiveType
    {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
    };

    class IRenderer : public Singleton<IRenderer>
    {
    public:
        virtual ~IRenderer() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void setViewport(int x, int y, int w, int h) = 0;

        virtual void clear() = 0;

        virtual ShaderPtr createShader() = 0;
        virtual TexturePtr createTexture() = 0;
        virtual VertexBufferPtr createVertexBuffer() = 0;
        virtual IndexBufferPtr createIndexBuffer() = 0;
        virtual VertexSourcePtr createVertexSource() = 0;

        virtual const glm::mat4& projectionMatrix() const = 0;
        virtual void setProjectionMatrix(const glm::mat4& matrix) = 0;
        virtual void pushProjectionMatrix() = 0;
        virtual void popProjectionMatrix() = 0;

        virtual const glm::mat4& modelViewMatrix() const = 0;
        virtual void setModelViewMatrix(const glm::mat4& matrix) = 0;
        virtual void pushModelViewMatrix() = 0;
        virtual void popModelViewMatrix() = 0;

        virtual void setUniform(const Atom& name, float value) = 0;
        virtual void setUniform(const Atom& name, const glm::vec2& value) = 0;
        virtual void setUniform(const Atom& name, const glm::vec3& value) = 0;
        virtual void setUniform(const Atom& name, const glm::vec4& value) = 0;
        virtual void setUniform(const Atom& name, const glm::mat4& value) = 0;

        virtual void useShader(const ShaderPtr& shader) = 0;
        virtual void bindVertexSource(const VertexSourcePtr& source) = 0;

        virtual void drawPrimitive(PrimitiveType primitiveType, size_t first, size_t count) = 0;
    };
}
