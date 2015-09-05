
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/render/IShader.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace Engine
{
    class ResourceManager
    {
    public:
        static ResourceManager* instance() { return &mInstance; }

        ShaderPtr getShader(const std::string& fileName);

    private:
        static ResourceManager mInstance;
        std::unordered_map<std::string, std::weak_ptr<IShader>> mShaders;

        ResourceManager();
        ~ResourceManager();

        Z_DISABLE_COPY(ResourceManager);
    };
}
