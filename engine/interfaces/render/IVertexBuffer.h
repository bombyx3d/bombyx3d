
#pragma once
#include "engine/interfaces/render/IBuffer.h"
#include <memory>

namespace Engine
{
    class IVertexBuffer : public IBuffer
    {
    };

    using VertexBufferPtr = std::shared_ptr<IVertexBuffer>;
}
