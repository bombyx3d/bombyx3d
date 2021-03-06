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
#include "AbstractLoadingScene.h"
#include "engine/core/Application.h"
#include "engine/core/Services.h"
#include <utility>
#include <algorithm>

namespace B3D
{
    AbstractLoadingScene::AbstractLoadingScene(const glm::vec2& virtualSize, AspectRatio aspect)
        : UIScene(virtualSize, aspect)
        , mCurrentProgress(0.0f)
        , mAutoSwitchScene(true)
        , mLoadingComplete(false)
    {
    }

    void AbstractLoadingScene::beginLoading(const ScenePtr& scene)
    {
        mNextScene = scene;
        mCurrentProgress = 0.0f;
        mLoadingComplete = false;
    }

    void AbstractLoadingScene::beginLoading(ScenePtr&& scene)
    {
        mNextScene = std::move(scene);
        mCurrentProgress = 0.0f;
        mLoadingComplete = false;
    }

    void AbstractLoadingScene::switchToNextScene()
    {
        Services::sceneManager()->setCurrentScene(mNextScene);
    }

    void AbstractLoadingScene::update(double)
    {
        if (!mNextScene)
            return;

        float progress = Services::resourceManager()->resourceLoadProgress();
        mCurrentProgress = std::max(mCurrentProgress, progress);

        if (!Services::resourceManager()->resourcesAreLoading()) {
            mLoadingComplete = true;
            if (mAutoSwitchScene)
                switchToNextScene();
        }
    }
}
