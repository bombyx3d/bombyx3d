
#pragma once
#include "engine/interfaces/render/IRenderer.h"

namespace Engine
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        ~Renderer();

        void setViewport(int x, int y, int w, int h) override;

        void clear() override;
    };
}
