
#pragma once
#include <memory>

namespace Engine
{
    class IShader
    {
    public:
        virtual ~IShader() = default;
    };

    using ShaderPtr = std::shared_ptr<IShader>;
}
