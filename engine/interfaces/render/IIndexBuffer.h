
#pragma once
#include "engine/interfaces/render/IBuffer.h"
#include <memory>

namespace Engine
{
    class IIndexBuffer : public IBuffer
    {
    };

    using IndexBufferPtr = std::shared_ptr<IIndexBuffer>;
}
