
#pragma once
#include "engine/interfaces/render/IRenderer.h"
#include <glm/glm.hpp>

namespace Engine
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        ~Renderer();

        void setViewport(int x, int y, int w, int h) override;

        void clear() override;

        ShaderPtr createShader() override;
        TexturePtr createTexture() override;

        void useShader(const ShaderPtr& shader) override;

    private:
        ShaderPtr mCurrentShader;
    };
}
