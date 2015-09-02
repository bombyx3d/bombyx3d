#include "Application.h"
#include "engine/render/Renderer.h"

namespace Engine
{
    Application* Application::m_Instance;

    Application::Application()
    {
        assert(m_Instance == nullptr);
        m_Instance = this;
    }

    Application::~Application()
    {
        assert(m_Instance == this);
        m_Instance = nullptr;
    }

    glm::ivec2 Application::preferredScreenSize() const
    {
        return glm::ivec2(1024, 768);
    }

    int Application::preferredDepthBits() const
    {
        return 24;
    }

    int Application::preferredStencilBits() const
    {
        return 8;
    }

    void Application::initialize(const glm::ivec2& screenSize)
    {
        m_ScreenSize = screenSize;
        m_Renderer.reset(new Renderer);
    }

    void Application::shutdown()
    {
        m_Renderer.reset();
    }

    void Application::resize(const glm::ivec2& screenSize)
    {
        m_ScreenSize = screenSize;
    }

    void Application::runFrame(double)
    {
        m_Renderer->setViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);
        m_Renderer->clear();
    }
}
