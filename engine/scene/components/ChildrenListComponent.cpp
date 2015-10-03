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
#include "ChildrenListComponent.h"
#include "engine/core/Services.h"
#include <cassert>
#include <algorithm>
#include <utility>

namespace B3D
{
    namespace {
        using diff_t = std::vector<ScenePtr>::difference_type;
    }

    ChildrenListComponent::ChildrenListComponent()
    {
    }

    ChildrenListComponent::~ChildrenListComponent()
    {
    }

    void ChildrenListComponent::insertChild(size_t index, const ScenePtr& child)
    {
        assert(index <= mChildren.size());
        Services::inputManager()->resetAll();
        mChildren.emplace(mChildren.begin() + diff_t(std::min(index, mChildren.size())), child);
    }

    void ChildrenListComponent::insertChild(size_t index, ScenePtr&& child)
    {
        assert(index <= mChildren.size());
        Services::inputManager()->resetAll();
        mChildren.emplace(mChildren.begin() + diff_t(std::min(index, mChildren.size())), std::move(child));
    }

    void ChildrenListComponent::removeChild(size_t index)
    {
        assert(index < mChildren.size());
        if (index < mChildren.size()) {
            Services::inputManager()->resetAll();
            mChildren.erase(mChildren.begin() + diff_t(index));
        }
    }

    void ChildrenListComponent::removeLastChild()
    {
        assert(!mChildren.empty());
        if (!mChildren.empty()) {
            Services::inputManager()->resetAll();
            mChildren.pop_back();
        }
    }

    void ChildrenListComponent::appendChild(const ScenePtr& child)
    {
        assert(child);
        Services::inputManager()->resetAll();
        mChildren.emplace_back(child);
    }

    void ChildrenListComponent::appendChild(ScenePtr&& child)
    {
        assert(child);
        Services::inputManager()->resetAll();
        mChildren.emplace_back(std::move(child));
    }

    void ChildrenListComponent::onSceneSizeChanged(IScene*, const glm::vec2& newSize)
    {
        for (const auto& child : mChildren)
            child->setSize(newSize);
    }

    void ChildrenListComponent::onAfterUpdateScene(IScene*, double time)
    {
        for (const auto& child : mChildren)
            child->performUpdate(time);
    }

    void ChildrenListComponent::onAfterDrawScene(const IScene*, ICanvas* canvas)
    {
        for (const auto& child : mChildren)
            child->performDraw(canvas);
    }

    void ChildrenListComponent::onBeforeTouchEvent(TouchEvent event, int fingerIndex, glm::vec2& position, bool& r)
    {
        if (r)
            return;

        switch (event)
        {
        case TouchEvent::Begin:
            if (mTouchedChild) {
                if (mTouchedChild->beginTouch(fingerIndex, position))
                    mTouchedFingers.insert(fingerIndex);
                r = true;
                return;
            }

            for (auto it = mChildren.crbegin(); it != mChildren.crend(); ++it) {
                if ((*it)->beginTouch(fingerIndex, position)) {
                    r = true;
                    mTouchedFingers.insert(fingerIndex);
                    return;
                }
            }
            return;

        case TouchEvent::Move:
            if (mTouchedChild) {
                if (mTouchedFingers.find(fingerIndex) != mTouchedFingers.end())
                    mTouchedChild->moveTouch(fingerIndex, position);
                r = true;
            }
            return;

        case TouchEvent::End:
            if (mTouchedChild) {
                auto it = mTouchedFingers.find(fingerIndex);
                if (it != mTouchedFingers.end()) {
                    mTouchedFingers.erase(it);
                    mTouchedChild->endTouch(fingerIndex, position);
                    if (mTouchedFingers.empty())
                        mTouchedChild.reset();
                }
                r = true;
            }
            return;

        case TouchEvent::Cancel:
            if (mTouchedChild) {
                auto it = mTouchedFingers.find(fingerIndex);
                if (it != mTouchedFingers.end()) {
                    mTouchedFingers.erase(it);
                    mTouchedChild->cancelTouch(fingerIndex, position);
                    if (mTouchedFingers.empty())
                        mTouchedChild.reset();
                }
                r = true;
            }
            return;
        }
    }
}
