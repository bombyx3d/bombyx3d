#include "Application.h"
#include "engine/interfaces/render/IRenderer.h"

namespace Engine
{
    Application* Application::mInstance;

    Application::Application()
    {
        assert(mInstance == nullptr);
        mInstance = this;
    }

    Application::~Application()
    {
        assert(mInstance == this);
        mInstance = nullptr;
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
        mScreenSize = screenSize;
    }

    void Application::shutdown()
    {
    }

    void Application::resize(const glm::ivec2& screenSize)
    {
        mScreenSize = screenSize;
    }

    void Application::runFrame(double)
    {
        IRenderer::instance()->setViewport(0, 0, mScreenSize.x, mScreenSize.y);
        IRenderer::instance()->clear();
    }
}
