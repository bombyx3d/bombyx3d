
#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    class IApplication
    {
    public:
        static IApplication* create();
        virtual ~IApplication() = default;

        virtual glm::ivec2 preferredScreenSize() const = 0;
        virtual int preferredDepthBits() const = 0;
        virtual int preferredStencilBits() const = 0;

        virtual void initialize(const glm::ivec2& screenSize) = 0;
        virtual void shutdown() = 0;

        virtual void resize(const glm::ivec2& screenSize) = 0;

        virtual void runFrame(double time) = 0;
    };
}
