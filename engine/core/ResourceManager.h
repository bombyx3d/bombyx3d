
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/render/IShader.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <atomic>

namespace Engine
{
    class ResourceManager
    {
    public:
        static ResourceManager* instance() { return &mInstance; }

        int numPendingResources() const;

        ShaderPtr getShader(const std::string& fileName, bool async = true);

    private:
        static ResourceManager mInstance;
        std::unordered_map<std::string, std::weak_ptr<IShader>> mShaders;
        std::shared_ptr<std::atomic<int>> mNumPendingResources;

        ResourceManager();
        ~ResourceManager();

        Z_DISABLE_COPY(ResourceManager);
    };
}
