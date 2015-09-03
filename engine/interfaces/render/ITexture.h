
#pragma once
#include <memory>

namespace Engine
{
    class ITexture
    {
    public:
        virtual ~ITexture() = default;
    };

    using TexturePtr = std::shared_ptr<ITexture>;
}
