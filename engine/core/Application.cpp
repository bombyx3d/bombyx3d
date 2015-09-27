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
#include "engine/core/ResourceManager.h"
#include "engine/scene/SceneManager.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include <cassert>

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

    void Application::initialize(RendererPtr&& renderer, const glm::vec2& screenSize)
    {
        Services::setRendererResourceFactory(renderer);
        Services::setResourceManager(std::make_shared<ResourceManager>());

        mSceneManager = std::make_shared<SceneManager>(renderer, screenSize);
        Services::setSceneManager(mSceneManager);
        mSceneManager->setCurrentScene(createInitialScene());

        onApplicationDidFinishLaunching();
    }

    void Application::shutdown()
    {
        onApplicationWillTerminate();

        mSceneManager->setCurrentScene(nullptr);
        Services::setSceneManager(nullptr);
        mSceneManager.reset();

        Services::setResourceManager(nullptr);
        Services::setRendererResourceFactory(nullptr);
    }

    void Application::resize(const glm::vec2& screenSize)
    {
        mSceneManager->resize(screenSize);
    }

    void Application::runFrame(double time)
    {
        mSceneManager->runFrame(time);
    }
}
