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
#include "engine/core/Services.h"
#include <cassert>
#include <algorithm>
#include <utility>

namespace Engine
{
    LayeredScene::LayeredScene()
    {
    }

    LayeredScene::~LayeredScene()
    {
    }

    void LayeredScene::insertChild(size_t index, const ScenePtr& child)
    {
        assert(index <= mChildren.size());
        Services::inputManager()->resetAll();
        mChildren.emplace(mChildren.begin() + std::min(index, mChildren.size()), child);
    }

    void LayeredScene::insertChild(size_t index, ScenePtr&& child)
    {
        assert(index <= mChildren.size());
        Services::inputManager()->resetAll();
        mChildren.emplace(mChildren.begin() + std::min(index, mChildren.size()), std::move(child));
    }

    void LayeredScene::removeChild(size_t index)
    {
        assert(index < mChildren.size());
        if (index < mChildren.size()) {
            Services::inputManager()->resetAll();
            mChildren.erase(mChildren.begin() + index);
        }
    }

    void LayeredScene::removeLastChild()
    {
        assert(!mChildren.empty());
        if (!mChildren.empty()) {
            Services::inputManager()->resetAll();
            mChildren.pop_back();
        }
    }

    void LayeredScene::appendChild(const ScenePtr& child)
    {
        assert(child);
        Services::inputManager()->resetAll();
        mChildren.emplace_back(child);
    }

    void LayeredScene::appendChild(ScenePtr&& child)
    {
        assert(child);
        Services::inputManager()->resetAll();
        mChildren.emplace_back(std::move(child));
    }

    void LayeredScene::resize(const glm::vec2& newSize)
    {
        for (const auto& child : mChildren)
            child->resize(newSize);
    }

    void LayeredScene::update(double time)
    {
        for (const auto& child : mChildren)
            child->update(time);
    }

    void LayeredScene::draw(ICanvas* canvas) const
    {
        for (const auto& child : mChildren)
            child->draw(canvas);
    }

    bool LayeredScene::onTouchBegan(int fingerIndex, const glm::vec2& position)
    {
        if (mTouchedChild) {
            if (mTouchedChild->onTouchBegan(fingerIndex, position)) {
                ++mActiveTouchCount;
                return true;
            }
            return false;
        }

        for (auto it = mChildren.crbegin(); it != mChildren.crend(); ++it) {
            if ((*it)->onTouchBegan(fingerIndex, position)) {
                ++mActiveTouchCount;
                return true;
            }
        }

        return false;
    }

    void LayeredScene::onTouchMoved(int fingerIndex, const glm::vec2& position)
    {
        if (mTouchedChild)
            mTouchedChild->onTouchMoved(fingerIndex, position);
    }

    void LayeredScene::onTouchEnded(int fingerIndex)
    {
        if (mTouchedChild) {
            mTouchedChild->onTouchEnded(fingerIndex);
            assert(mActiveTouchCount > 0);
            if (--mActiveTouchCount == 0)
                mTouchedChild.reset();
        }
    }

    void LayeredScene::onTouchCancelled(int fingerIndex)
    {
        if (mTouchedChild) {
            mTouchedChild->onTouchCancelled(fingerIndex);
            assert(mActiveTouchCount > 0);
            if (--mActiveTouchCount == 0)
                mTouchedChild.reset();
        }
    }
}
