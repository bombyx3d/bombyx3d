
#pragma once
#include "engine/interfaces/core/IApplication.h"
#include <memory>
#include <cassert>

namespace Engine
{
    class Application : public IApplication
    {
    public:
        Application();
        ~Application();

        static const glm::ivec2& screenSize() { assert(mInstance); return mInstance->mScreenSize; }

        glm::ivec2 preferredScreenSize() const override;
        int preferredDepthBits() const override;
        int preferredStencilBits() const override;

        void initialize(const glm::ivec2& screenSize) override;
        void shutdown() override;
        void resize(const glm::ivec2& screenSize) override;
        void runFrame(double time) override;

    private:
        static Application* mInstance;
        glm::ivec2 mScreenSize;
    };
}
