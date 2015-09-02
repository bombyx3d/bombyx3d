
#pragma once
#include "engine/interfaces/core/IApplication.h"

namespace Engine
{
    class Application : public IApplication
    {
    public:
        Application();
        ~Application();

        glm::ivec2 preferredScreenSize() const override;
        int preferredDepthBits() const override;
        int preferredStencilBits() const override;

        void initialize(const glm::ivec2& screenSize) override;
        void shutdown() override;

        void resize(const glm::ivec2& screenSize) override;

        void runFrame(double time) override;
    };
}
