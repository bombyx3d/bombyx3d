
#pragma once
#include "engine/interfaces/render/IVertexBuffer.h"
#include "engine/interfaces/render/IIndexBuffer.h"
#include "engine/core/Atom.h"
#include <memory>

namespace Engine
{
    enum class VertexAttributeType
    {
        Float,
        Float2,
        Float3,
        Float4,
    };

    class IVertexFormatAttributeList;

    class IVertexSource
    {
    public:
        virtual ~IVertexSource() = default;

        virtual void setAttribute(const Atom& name, VertexAttributeType type,
            const VertexBufferPtr& buffer, size_t offset = 0, size_t stride = 0, bool normalize = false) = 0;

        virtual void setAttributes(const IVertexFormatAttributeList& attributes, const VertexBufferPtr& buffer) = 0;
        template <typename T> void setAttributes(const VertexBufferPtr& buf) { setAttributes(buf, T::attributes()); }

        virtual void setIndexBuffer(const IndexBufferPtr& indexBuffer) = 0;
    };

    using VertexSourcePtr = std::shared_ptr<IVertexSource>;
}
