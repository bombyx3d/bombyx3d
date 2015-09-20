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
#include "LayeredScene.h"
#include <cassert>

namespace Engine
{
    LayeredScene::LayeredScene()
    {
    }

    LayeredScene::~LayeredScene()
    {
    }

    void LayeredScene::insertScene(size_t index, const ScenePtr& scene)
    {
        assert(index <= mScenes.size());
        Services::inputManager()->resetAll();
        mScenes.emplace(mScenes.begin() + std::min(index, mScenes.size()), scene);
    }

    void LayeredScene::insertScene(size_t index, ScenePtr&& scene)
    {
        assert(index <= mScenes.size());
        Services::inputManager()->resetAll();
        mScenes.emplace(mScenes.begin() + std::min(index, mScenes.size()), std::move(scene));
    }

    void LayeredScene::removeScene(size_t index)
    {
        assert(index < mScenes.size());
        if (index < mScenes.size()) {
            Services::inputManager()->resetAll();
            mScenes.erase(mScenes.begin() + index);
        }
    }

    void LayeredScene::pushScene(const ScenePtr& scene)
    {
        assert(scene);
        Services::inputManager()->resetAll();
        mScenes.emplace_back(scene);
    }

    void LayeredScene::pushScene(ScenePtr&& scene)
    {
        assert(scene);
        Services::inputManager()->resetAll();
        mScenes.emplace_back(std::move(scene));
    }

    void LayeredScene::popScene()
    {
        assert(!mScenes.empty());
        if (!mScenes.empty()) {
            Services::inputManager()->resetAll();
            mScenes.pop_back();
        }
    }

    void LayeredScene::resize(const glm::ivec2& newSize)
    {
        for (const auto& scene : mScenes)
            scene->resize(newSize);
    }

    void LayeredScene::update(double time)
    {
        for (const auto& scene : mScenes)
            scene->update(time);
    }

    void LayeredScene::draw(IRenderer* renderer) const
    {
        for (const auto& scene : mScenes)
            scene->draw(renderer);
    }

    bool LayeredScene::onTouchBegan(int fingerIndex, const glm::ivec2& position)
    {
        if (mTouchedScene) {
            if (mTouchedScene->onTouchBegan(fingerIndex, position)) {
                ++mActiveTouchCount;
                return true;
            }
            return false;
        }

        for (auto it = mScenes.crbegin(); it != mScenes.crend(); ++it) {
            if ((*it)->onTouchBegan(fingerIndex, position)) {
                ++mActiveTouchCount;
                return true;
            }
        }

        return false;
    }

    void LayeredScene::onTouchMoved(int fingerIndex, const glm::ivec2& position)
    {
        if (mTouchedScene)
            mTouchedScene->onTouchMoved(fingerIndex, position);
    }

    void LayeredScene::onTouchEnded(int fingerIndex)
    {
        if (mTouchedScene) {
            mTouchedScene->onTouchEnded(fingerIndex);
            assert(mActiveTouchCount > 0);
            if (--mActiveTouchCount == 0)
                mTouchedScene.reset();
        }
    }

    void LayeredScene::onTouchCancelled(int fingerIndex)
    {
        if (mTouchedScene) {
            mTouchedScene->onTouchCancelled(fingerIndex);
            assert(mActiveTouchCount > 0);
            if (--mActiveTouchCount == 0)
                mTouchedScene.reset();
        }
    }
}
