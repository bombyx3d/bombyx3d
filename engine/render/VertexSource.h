
#pragma once
#include "engine/interfaces/render/IVertexSource.h"
#include "engine/core/macros.h"
#include "engine/render/Buffer.h"
#include "engine/render/Shader.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Engine
{
    class VertexSource : public IVertexSource
    {
    public:
        VertexSource();
        ~VertexSource();

        void setAttribute(const Atom& name, VertexAttributeType type,
            const VertexBufferPtr& buffer, size_t offset = 0, size_t stride = 0) override;

        const std::shared_ptr<Buffer>& indexBuffer() const { return mIndexBuffer; }
        void setIndexBuffer(const IndexBufferPtr& indexBuffer) override;

        void bind(const Shader& shader);
        void unbind();

    private:
        struct Attribute
        {
            std::shared_ptr<Buffer> buffer;
            VertexAttributeType type;
            size_t offset;
            size_t stride;
        };

        std::unordered_map<Atom, Attribute> mAttributes;
        std::vector<int> mEnabledArrays;
        std::shared_ptr<Buffer> mIndexBuffer;

        Z_DISABLE_COPY(VertexSource);
    };
}
