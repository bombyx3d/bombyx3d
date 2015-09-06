
#pragma once
#include "engine/interfaces/render/IIndexBuffer.h"
#include "engine/interfaces/render/IVertexBuffer.h"
#include "engine/core/macros.h"

namespace Engine
{
    class Buffer : public IVertexBuffer, public IIndexBuffer
    {
    public:
        explicit Buffer(size_t target);
        ~Buffer();

        size_t handle() const { return mHandle; }

        size_t currentSize() const override;

        void initEmpty(size_t size, BufferUsage usage) override;
        void setData(const void* data, size_t size, BufferUsage usage) override;

    private:
        size_t mHandle;
        size_t mSize;
        size_t mTarget;
        BufferUsage mUsage;

        Z_DISABLE_COPY(Buffer);
    };
}
