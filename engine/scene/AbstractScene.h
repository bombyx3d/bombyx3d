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
#include "engine/interfaces/scene/IScene.h"
#include <vector>

namespace B3D
{
    class AbstractScene : public IScene
    {
    public:
        ~AbstractScene();

        void addComponent(const SceneComponentPtr& component) override;
        void addComponent(SceneComponentPtr&& component) override;
        void removeComponent(const SceneComponentPtr& component) override;

        void setSize(const glm::vec2& newSize) final override;
        const glm::vec2& size() const { return mSize; }

        void performUpdate(double time) final override;
        void performDraw(ICanvas* canvas) const final override;

        bool beginTouch(int fingerIndex, const glm::vec2& position) final override;
        void moveTouch(int fingerIndex, const glm::vec2& position) final override;
        void endTouch(int fingerIndex, const glm::vec2& position) final override;
        void cancelTouch(int fingerIndex, const glm::vec2& position) final override;

        void sendEvent(const IEvent* event, bool recursive = true) final override;

    protected:
        AbstractScene();

        virtual void onSizeChanged(const glm::vec2& newSize);

        virtual void update(double time);
        virtual void draw(ICanvas* canvas) const;

        virtual bool onTouchBegan(int fingerIndex, const glm::vec2& position);
        virtual void onTouchMoved(int fingerIndex, const glm::vec2& position);
        virtual void onTouchEnded(int fingerIndex, const glm::vec2& position);
        virtual void onTouchCancelled(int fingerIndex, const glm::vec2& position);

        virtual void onEvent(const IEvent* event);

    private:
        std::vector<SceneComponentPtr> mComponents;
        glm::vec2 mSize;
        mutable int mIterating;

        B3D_DISABLE_COPY(AbstractScene);
    };
}
