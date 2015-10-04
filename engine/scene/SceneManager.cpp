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
#include "SceneManager.h"
#include "engine/core/ResourceManager.h"
#include "engine/core/Log.h"
#include "engine/core/Services.h"
#include <cassert>

namespace B3D
{
    SceneManager::SceneManager(const RendererPtr& renderer, const glm::vec2& screenSize)
        : mRenderer(renderer)
        , mDefaultClearColor(0.7f, 0.3f, 0.1f, 1.0f)
    {
        Services::inputManager()->resetAll();
        Services::inputManager()->addObserver(this);
        resize(screenSize);
        mCanvas.reset(new Canvas(renderer));
    }

    SceneManager::~SceneManager()
    {
        setCurrentScene(nullptr);
        mCanvas.reset();
        Services::inputManager()->removeObserver(this);
    }

    void SceneManager::setCurrentScene(const ScenePtr& scene)
    {
        Services::inputManager()->resetAll();
        mCurrentScene = scene;
        if (mCurrentScene)
            mCurrentScene->setSize(mScreenSize);
    }

    void SceneManager::setCurrentScene(ScenePtr&& scene)
    {
        Services::inputManager()->resetAll();
        mCurrentScene = std::move(scene);
        if (mCurrentScene)
            mCurrentScene->setSize(mScreenSize);
    }

    void SceneManager::resize(const glm::vec2& screenSize)
    {
        mScreenSize = screenSize;

        if (mScreenSize.x <= 0.0f || mScreenSize.y <= 0.0f)
            mScreenAspect = 1.0f;
        else
            mScreenAspect = mScreenSize.x / mScreenSize.y;

        Services::inputManager()->resetAll();
        if (mCurrentScene)
            mCurrentScene->setSize(screenSize);
    }

    void SceneManager::runFrame(double time)
    {
        mRenderer->beginFrame();
        mCanvas->resetMatrixStacks();

        mRenderer->setViewport(0, 0, int(mScreenSize.x), int(mScreenSize.y));
        mRenderer->setClearColor(mDefaultClearColor);
        mRenderer->clear();

        if (mCurrentScene) {
            ScenePtr currentScene = mCurrentScene;
            currentScene->performUpdate(time);
            currentScene->performDraw(mCanvas.get());
        }

        mCanvas->flush(true);
        mRenderer->endFrame();
    }

    void SceneManager::onTouchBegan(int fingerIndex, const glm::vec2& position)
    {
        if (mCurrentScene) {
            auto p = adjustTouchPosition(position);
            if (mCurrentScene->beginTouch(fingerIndex, p))
                mActiveTouches.insert(fingerIndex);
        }
    }

    void SceneManager::onTouchMoved(int fingerIndex, const glm::vec2& position)
    {
        if (mCurrentScene && mActiveTouches.find(fingerIndex) != mActiveTouches.end()) {
            auto p = adjustTouchPosition(position);
            mCurrentScene->moveTouch(fingerIndex, p);
        }
    }

    void SceneManager::onTouchEnded(int fingerIndex, const glm::vec2& position)
    {
        auto it = mActiveTouches.find(fingerIndex);
        if (mCurrentScene && it != mActiveTouches.end()) {
            mActiveTouches.erase(it);
            auto p = adjustTouchPosition(position);
            mCurrentScene->endTouch(fingerIndex, p);
        }
    }

    void SceneManager::onTouchCancelled(int fingerIndex, const glm::vec2& position)
    {
        auto it = mActiveTouches.find(fingerIndex);
        if (mCurrentScene && it != mActiveTouches.end()) {
            mActiveTouches.erase(it);
            auto p = adjustTouchPosition(position);
            mCurrentScene->cancelTouch(fingerIndex, p);
        }
    }

    glm::vec2 SceneManager::adjustTouchPosition(const glm::vec2& position) const
    {
        return glm::vec2(
             2.0f * position.x / mScreenSize.x - 1.0f,
            -2.0f * position.y / mScreenSize.y + 1.0f
        );
    }
}
