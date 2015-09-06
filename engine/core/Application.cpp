#include "Application.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/core/Log.h"
#include <cassert>

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

    void Application::setCurrentScene(const ScenePtr& scene)
    {
        mCurrentScene = scene;
        if (mCurrentScene)
            mCurrentScene->resize(mScreenSize);
    }

    void Application::setCurrentScene(ScenePtr&& scene)
    {
        mCurrentScene = std::move(scene);
        if (mCurrentScene)
            mCurrentScene->resize(mScreenSize);
    }

    void Application::initialize(const glm::ivec2& screenSize)
    {
        mScreenSize = screenSize;
        mScreenAspect = float(mScreenSize.x) / float(mScreenSize.y);
        setCurrentScene(createInitialScene());
    }

    void Application::shutdown()
    {
        setCurrentScene(ScenePtr());
    }

    void Application::resize(const glm::ivec2& screenSize)
    {
        mScreenSize = screenSize;
        mScreenAspect = float(mScreenSize.x) / float(mScreenSize.y);
        if (mCurrentScene)
            mCurrentScene->resize(screenSize);
    }

    void Application::runFrame(double time)
    {
        IRenderer::instance()->beginFrame();

        IRenderer::instance()->setViewport(0, 0, mScreenSize.x, mScreenSize.y);
        IRenderer::instance()->clear();

        if (mCurrentScene) {
            mCurrentScene->update(time);
            mCurrentScene->draw();
        }

        IRenderer::instance()->endFrame();
    }
}
