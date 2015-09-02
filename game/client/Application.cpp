#include "Application.h"

namespace Engine
{
    Application::Application()
    {
    }

    Application::~Application()
    {
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

    void Application::initialize(const glm::ivec2&)
    {
    }

    void Application::shutdown()
    {
    }

    void Application::resize(const glm::ivec2&)
    {
    }

    void Application::runFrame(double)
    {
    }
}
