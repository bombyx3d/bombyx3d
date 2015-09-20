/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "Application.h"
#include "engine/interfaces/render/IRenderer.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include <cassert>

namespace Engine
{
    Application* Application::mInstance;

    Application::Application()
    {
        assert(mInstance == nullptr);
        mInstance = this;
        Services::inputManager()->addObserver(this);
    }

    Application::~Application()
    {
        assert(mInstance == this);
        Services::inputManager()->removeObserver(this);
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
        Services::inputManager()->resetAll();
        mPreviousScene = std::move(mCurrentScene);
        mCurrentScene = scene;
        if (mCurrentScene)
            mCurrentScene->resize(mScreenSize);
    }

    void Application::setCurrentScene(ScenePtr&& scene)
    {
        Services::inputManager()->resetAll();
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

        Services::inputManager()->resetAll();
        if (mCurrentScene)
            mCurrentScene->resize(screenSize);
    }

    void Application::runFrame(double time)
    {
        const auto& renderer = Services::renderer();

        renderer->beginFrame();

        renderer->setViewport(0, 0, mScreenSize.x, mScreenSize.y);
        renderer->setClearColor(glm::vec4(0.7f, 0.3f, 0.1f, 1.0f));
        renderer->clear();

        if (mCurrentScene) {
            ScenePtr currentScene = mCurrentScene;
            currentScene->update(time);
            currentScene->draw(renderer.get());
        }

        if (mPreviousScene)
            mPreviousScene.reset();

        renderer->endFrame();
    }

    void Application::onTouchBegan(int fingerIndex, const glm::vec2& position)
    {
        if (mCurrentScene) {
            // FIXME: return value is not handled
            mCurrentScene->onTouchBegan(fingerIndex, glm::ivec2(position)); // FIXME: ivec2
        }
    }

    void Application::onTouchMoved(int fingerIndex, const glm::vec2& position)
    {
        if (mCurrentScene)
            mCurrentScene->onTouchMoved(fingerIndex, glm::ivec2(position)); // FIXME: ivec2
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
}
