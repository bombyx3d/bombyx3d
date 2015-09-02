
#pragma once
#include "engine/interfaces/core/IApplication.h"
#include "engine/interfaces/render/IRenderer.h"
#include <memory>
#include <cassert>

namespace Engine
{
    class Application : public IApplication
    {
    public:
        Application();
        ~Application();

        static IRenderer* renderer() { assert(m_Instance); return m_Instance->m_Renderer.get(); }

        static const glm::ivec2& screenSize() { assert(m_Instance); return m_Instance->m_ScreenSize; }

        glm::ivec2 preferredScreenSize() const override;
        int preferredDepthBits() const override;
        int preferredStencilBits() const override;

        void initialize(const glm::ivec2& screenSize) override;
        void shutdown() override;
        void resize(const glm::ivec2& screenSize) override;
        void runFrame(double time) override;

    private:
        static Application* m_Instance;
        std::unique_ptr<IRenderer> m_Renderer;
        glm::ivec2 m_ScreenSize;
    };
}
