
#pragma once
#include <vector>
#include <string>
#include <memory>

namespace Engine
{
    class IShader
    {
    public:
        virtual ~IShader() = default;

        virtual void setVertexSource(const std::vector<std::string>& source) = 0;
        virtual void setFragmentSource(const std::vector<std::string>& source) = 0;

        virtual bool compile() = 0;
    };

    using ShaderPtr = std::shared_ptr<IShader>;
}
