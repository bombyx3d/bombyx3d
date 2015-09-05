
#pragma once
#include "engine/utility/Singleton.h"
#include "engine/interfaces/render/IShader.h"
#include "engine/interfaces/render/ITexture.h"
#include <glm/glm.hpp>

namespace Engine
{
    class IRenderer : public Singleton<IRenderer>
    {
    public:
        virtual ~IRenderer() = default;

        virtual void setViewport(int x, int y, int w, int h) = 0;

        virtual void clear() = 0;

        virtual ShaderPtr createShader() = 0;
        virtual TexturePtr createTexture() = 0;

        virtual void useShader(const ShaderPtr& shader) = 0;
    };
}
