#include "Application.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/core/Log.h"
#include "engine/core/ResourceManager.h"

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
        //ResourceManager::instance()->getShader("shaders/BasicColored.glsl", true);
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
        IRenderer::instance()->beginFrame();
        IRenderer::instance()->setViewport(0, 0, mScreenSize.x, mScreenSize.y);
        IRenderer::instance()->clear();
        IRenderer::instance()->endFrame();
    }
}
