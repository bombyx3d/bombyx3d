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
#include "engine/scene/AbstractScene.h"
#include <vector>

namespace Engine
{
    class LayeredScene : public AbstractScene
    {
    public:
        LayeredScene();
        ~LayeredScene();

        size_t childrenCount() const { return mChildren.size(); }
        void insertChild(size_t index, const ScenePtr& child);
        void insertChild(size_t index, ScenePtr&& child);
        void removeChild(size_t index);
        void removeLastChild();
        void appendChild(const ScenePtr& child);
        void appendChild(ScenePtr&& child);

        void resize(const glm::vec2& newSize) override;

        void update(double time) override;
        void draw(ICanvas* canvas) const override;

        bool beginTouch(int fingerIndex, const glm::vec2& position) override;
        void moveTouch(int fingerIndex, const glm::vec2& position) override;
        void endTouch(int fingerIndex, const glm::vec2& position) override;
        void cancelTouch(int fingerIndex, const glm::vec2& position) override;

    private:
        std::vector<ScenePtr> mChildren;
        ScenePtr mTouchedChild;
        int mActiveTouchCount = 0;

        Z_DISABLE_COPY(LayeredScene);
    };
}
