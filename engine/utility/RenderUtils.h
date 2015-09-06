
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/render/IVertexSource.h"
#include "engine/mesh/VertexFormat.h"
#include <vector>
#include <string>

namespace Engine
{
    namespace RenderUtils
    {
        template <typename TYPE> VertexSourcePtr createVertexSource(const VertexBufferPtr& vertexBuffer)
        {
            VertexSourcePtr vertexSource = IRenderer::instance()->createVertexSource();
            vertexSource->setAttributes(TYPE::attributes(), vertexBuffer);
            return vertexSource;
        }

        template <typename TYPE>
        VertexSourcePtr createVertexSource(const VertexBufferPtr& vertexBuffer, const IndexBufferPtr& indexBuffer)
        {
            VertexSourcePtr vertexSource = IRenderer::instance()->createVertexSource();
            vertexSource->setAttributes(TYPE::attributes(), vertexBuffer);
            vertexSource->setIndexBuffer(indexBuffer);
            return vertexSource;
        }
    }
}
