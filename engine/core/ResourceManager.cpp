#include "ResourceManager.h"
#include "engine/core/Log.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/material/ShaderLoader.h"

namespace Engine
{
    ResourceManager ResourceManager::mInstance;

    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    ShaderPtr ResourceManager::getShader(const std::string& fileName)
    {
        auto& weakRef = mShaders[fileName];

        ShaderPtr shader = weakRef.lock();
        if (shader)
            return shader;

        Z_LOGI("Loading shader \"" << fileName << "\".");

        shader = IRenderer::instance()->createShader();

        ShaderLoader loader;
        if (loader.loadFile(fileName)) {
            shader->setVertexSource(loader.vertexSource());
            shader->setFragmentSource(loader.fragmentSource());
        }

        shader->compile();

        weakRef = shader;
        return shader;
    }
}
