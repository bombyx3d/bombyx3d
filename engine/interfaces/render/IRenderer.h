
#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual void setViewport(int x, int y, int w, int h) = 0;

        virtual void clear() = 0;
    };
}
