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
#include "engine/scene/AbstractLayoutStrategy.h"
#include "engine/utility/ScopedCounter.h"
#include <cassert>
#include <algorithm>
#include <utility>

namespace B3D
{
    namespace {
        using diff_t = std::vector<ScenePtr>::difference_type;
    }

    static const LayoutStrategyPtr gDummyStrategy = std::make_shared<AbstractLayoutStrategy>();

    ChildrenListComponent::ChildrenListComponent()
        : mLayoutStrategy(gDummyStrategy)
        , mSize(0.0f)
        , mIterating(0)
        , mNeedsLayout(false)
    {
    }

    ChildrenListComponent::~ChildrenListComponent()
    {
    }

    void ChildrenListComponent::insertChild(size_t index, const ScenePtr& child)
    {
        assert(!mIterating);
        assert(index <= mChildren.size());
        Services::inputManager()->resetAll();
        mChildren.emplace(mChildren.begin() + diff_t(std::min(index, mChildren.size())), child);
        mNeedsLayout = true;
    }

    void ChildrenListComponent::insertChild(size_t index, ScenePtr&& child)
    {
        assert(!mIterating);
        assert(index <= mChildren.size());
        Services::inputManager()->resetAll();
        mChildren.emplace(mChildren.begin() + diff_t(std::min(index, mChildren.size())), std::move(child));
        mNeedsLayout = true;
    }

    void ChildrenListComponent::removeChild(size_t index)
    {
        assert(!mIterating);
        assert(index < mChildren.size());
        if (index < mChildren.size()) {
            Services::inputManager()->resetAll();
            mChildren.erase(mChildren.begin() + diff_t(index));
            mNeedsLayout = true;
        }
    }

    void ChildrenListComponent::removeLastChild()
    {
        assert(!mIterating);
        assert(!mChildren.empty());
        if (!mChildren.empty()) {
            Services::inputManager()->resetAll();
            mChildren.pop_back();
            mNeedsLayout = true;
        }
    }

    void ChildrenListComponent::appendChild(const ScenePtr& child)
    {
        assert(!mIterating);
        assert(child);
        Services::inputManager()->resetAll();
        mChildren.emplace_back(child);
        mNeedsLayout = true;
    }

    void ChildrenListComponent::appendChild(ScenePtr&& child)
    {
        assert(!mIterating);
        assert(child);
        Services::inputManager()->resetAll();
        mChildren.emplace_back(std::move(child));
        mNeedsLayout = true;
    }

    void ChildrenListComponent::layoutChildren(bool force)
    {
        assert(!mIterating);

        if (!force && !mNeedsLayout)
            return;

        ScopedCounter counter(&mIterating);
        mLayoutStrategy->beginLayout(mChildren.size(), mSize);

        size_t index = 0;
        for (const auto& it : mChildren)
            mLayoutStrategy->measureElement(index++, it, mSize);

        index = 0;
        for (const auto& it : mChildren)
            mLayoutStrategy->layoutElement(index++, it, mSize);

        mLayoutStrategy->endLayout(mSize);
        mNeedsLayout = false;
    }

    void ChildrenListComponent::onAfterSizeChanged(IScene*, const glm::vec2& newSize)
    {
        mSize = newSize;
        mNeedsLayout = true;
    }

    void ChildrenListComponent::onBeforeUpdateScene(IScene*, double)
    {
        layoutChildren(false);
    }

    void ChildrenListComponent::onAfterUpdateScene(IScene*, double time)
    {
        layoutChildren(false);

        ScopedCounter counter(&mIterating);
        for (const auto& child : mChildren)
            child->performUpdate(time);
    }

    void ChildrenListComponent::onBeforeDrawScene(const IScene*, ICanvas*)
    {
        layoutChildren(false);
    }

    void ChildrenListComponent::onAfterDrawScene(const IScene*, ICanvas* canvas)
    {
        ScopedCounter counter(&mIterating);
        size_t index = 0;
        for (const auto& child : mChildren) {
            mLayoutStrategy->onBeforeDrawElement(index, child, canvas);
            child->performDraw(canvas);
            mLayoutStrategy->onAfterDrawElement(index, child, canvas);
            ++index;
        }
    }

    void ChildrenListComponent::onBeforeTouchEvent(TouchEvent event, int fingerIndex, glm::vec2& position, bool& r)
    {
        layoutChildren(false);

        if (r)
            return;

        switch (event)
        {
        case TouchEvent::Begin:
            if (mTouchedChild) {
                auto adjustedPosition = position;
                mLayoutStrategy->adjustTouchPositionForElement(mTouchedChildIndex, mTouchedChild, adjustedPosition);
                if (mTouchedChild->beginTouch(fingerIndex, adjustedPosition))
                    mTouchedFingers.insert(fingerIndex);
                r = true;
            } else {
                ScopedCounter counter(&mIterating);
                size_t index = mChildren.size();
                for (auto it = mChildren.crbegin(); it != mChildren.crend(); ++it) {
                    --index;
                    auto adjustedPosition = position;
                    mLayoutStrategy->adjustTouchPositionForElement(index, mTouchedChild,adjustedPosition);
                    if ((*it)->beginTouch(fingerIndex, adjustedPosition)) {
                        r = true;
                        mTouchedFingers.insert(fingerIndex);
                        mTouchedChild = *it;
                        mTouchedChildIndex = index;
                        return;
                    }
                }
            }
            return;

        case TouchEvent::Move:
            if (mTouchedChild) {
                if (mTouchedFingers.find(fingerIndex) != mTouchedFingers.end()) {
                    auto adjustedPosition = position;
                    mLayoutStrategy->adjustTouchPositionForElement(mTouchedChildIndex, mTouchedChild, adjustedPosition);
                    mTouchedChild->moveTouch(fingerIndex, adjustedPosition);
                }
                r = true;
            }
            return;

        case TouchEvent::End:
            if (mTouchedChild) {
                auto it = mTouchedFingers.find(fingerIndex);
                if (it != mTouchedFingers.end()) {
                    mTouchedFingers.erase(it);
                    auto adjustedPosition = position;
                    mLayoutStrategy->adjustTouchPositionForElement(mTouchedChildIndex, mTouchedChild, adjustedPosition);
                    mTouchedChild->endTouch(fingerIndex, adjustedPosition);
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
                    auto adjustedPosition = position;
                    mLayoutStrategy->adjustTouchPositionForElement(mTouchedChildIndex, mTouchedChild, adjustedPosition);
                    mTouchedChild->cancelTouch(fingerIndex, adjustedPosition);
                    if (mTouchedFingers.empty())
                        mTouchedChild.reset();
                }
                r = true;
            }
            return;
        }
    }

    void ChildrenListComponent::onAfterSendEvent(const IEvent* event, bool recursive)
    {
        if (recursive) {
            ScopedCounter counter(&mIterating);
            for (const auto& child : mChildren)
                child->sendEvent(event);
        }
    }
}
