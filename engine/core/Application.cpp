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
        cancelAllTouches();
        mPreviousScene = std::move(mCurrentScene);
        mCurrentScene = scene;
        if (mCurrentScene)
            mCurrentScene->resize(mScreenSize);
    }

    void Application::setCurrentScene(ScenePtr&& scene)
    {
        cancelAllTouches();
        mPreviousScene = std::move(mCurrentScene);
        mCurrentScene = std::move(scene);
        if (mCurrentScene)
            mCurrentScene->resize(mScreenSize);
    }

    void Application::initialize(const glm::ivec2& screenSize)
    {
        resize(screenSize);
        setCurrentScene(createInitialScene());
    }

    void Application::shutdown()
    {
        setCurrentScene(ScenePtr());
        mPreviousScene.reset();
    }

    void Application::resize(const glm::ivec2& screenSize)
    {
        mScreenSize = screenSize;

        if (mScreenSize.x <= 0 || mScreenSize.y <= 0)
            mScreenAspect = 1.0f;
        else
            mScreenAspect = float(mScreenSize.x) / float(mScreenSize.y);

        if (mCurrentScene)
            mCurrentScene->resize(screenSize);
    }

    void Application::runFrame(double time)
    {
        IRenderer* renderer =IRenderer::instance().get();

        renderer->beginFrame();

        renderer->setViewport(0, 0, mScreenSize.x, mScreenSize.y);
        renderer->clear();

        if (mCurrentScene) {
            ScenePtr currentScene = mCurrentScene;
            currentScene->update(time);
            currentScene->draw(renderer);
        }

        if (mPreviousScene)
            mPreviousScene.reset();

        renderer->endFrame();
    }

    void Application::onTouchBegan(int fingerIndex, const glm::ivec2& position)
    {
        if (mCurrentScene) {
            // FIXME: return value is not handled
            mCurrentScene->onTouchBegan(fingerIndex, position);
        }
    }

    void Application::onTouchMoved(int fingerIndex, const glm::ivec2& position)
    {
        if (mCurrentScene)
            mCurrentScene->onTouchMoved(fingerIndex, position);
    }

    void Application::onTouchEnded(int fingerIndex)
    {
        if (mCurrentScene)
            mCurrentScene->onTouchEnded(fingerIndex);
    }

    void Application::onTouchCancelled(int fingerIndex)
    {
        if (mCurrentScene)
            mCurrentScene->onTouchCancelled(fingerIndex);
    }

    void Application::cancelAllTouches()
    {
        // FIXME
    }
}
