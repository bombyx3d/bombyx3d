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

#pragma once
#include "engine/core/macros.h"
#include "engine/scene/AbstractSceneComponent.h"
#include "engine/interfaces/scene/IScene.h"
#include "engine/interfaces/scene/ILayoutStrategy.h"
#include <vector>
#include <unordered_set>

namespace B3D
{
    class ChildrenListComponent : public AbstractSceneComponent
    {
    public:
        ChildrenListComponent();
        ~ChildrenListComponent();

        const LayoutStrategyPtr& layoutStrategy() const { return mLayoutStrategy; }
        void setLayoutStrategy(const LayoutStrategyPtr& strategy) { mLayoutStrategy = strategy; mNeedsLayout = true; }
        void setLayoutStrategy(LayoutStrategyPtr&& s) { mLayoutStrategy = std::move(s); mNeedsLayout = true; }
        template <class TYPE, class... ARGS> void setLayoutStrategy(ARGS&&... args)
            { setLayoutStrategy(std::make_shared<TYPE>(std::forward<ARGS>(args)...)); }

        size_t childrenCount() const { return mChildren.size(); }
        void insertChild(size_t index, const ScenePtr& child);
        void insertChild(size_t index, ScenePtr&& child);
        void removeChild(size_t index);
        void removeLastChild();
        void appendChild(const ScenePtr& child);
        void appendChild(ScenePtr&& child);

        void layoutChildren(bool force);

    protected:
        void onAfterSizeChanged(IScene* scene, const glm::vec2& newSize) override;

        void onBeforeUpdateScene(IScene* scene, double time) override;
        void onAfterUpdateScene(IScene* scene, double time) override;

        void onBeforeDrawScene(const IScene* scene, ICanvas* canvas) override;
        void onAfterDrawScene(const IScene* scene, ICanvas* canvas) override;

        void onBeforeTouchEvent(TouchEvent event, int fingerIndex, glm::vec2& position, bool& result) override;

        void onAfterSendEvent(const IEvent* event, bool recursive) override;

    private:
        LayoutStrategyPtr mLayoutStrategy;
        std::vector<ScenePtr> mChildren;
        std::unordered_set<int> mTouchedFingers;
        ScenePtr mTouchedChild;
        size_t mTouchedChildIndex;
        glm::vec2 mSize;
        mutable int mIterating;
        bool mNeedsLayout;

        B3D_DISABLE_COPY(ChildrenListComponent);
    };

    using ChildrenListComponentPtr = std::shared_ptr<ChildrenListComponent>;
}
