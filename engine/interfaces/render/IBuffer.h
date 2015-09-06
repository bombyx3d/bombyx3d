
#pragma once
#include <cstddef>

namespace Engine
{
    enum class BufferUsage
    {
        Static,
        Dynamic,
        Stream,
    };

    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;

        virtual size_t currentSize() const = 0;

        virtual void initEmpty(size_t size, BufferUsage usage) = 0;
        virtual void setData(const void* data, size_t size, BufferUsage usage) = 0;
    };
}
