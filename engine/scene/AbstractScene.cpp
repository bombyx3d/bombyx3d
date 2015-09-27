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
#include "AbstractScene.h"

namespace Engine
{
    AbstractScene::AbstractScene()
        : mSize(0.0f)
    {
    }

    AbstractScene::~AbstractScene()
    {
    }

    void AbstractScene::addComponent(const SceneComponentPtr& component)
    {
        mComponents.emplace_back(component);
        mComponents.back()->onSceneSizeChanged(this, mSize);
    }

    void AbstractScene::addComponent(SceneComponentPtr&& component)
    {
        mComponents.emplace_back(std::move(component));
        mComponents.back()->onSceneSizeChanged(this, mSize);
    }

    void AbstractScene::removeComponent(const SceneComponentPtr& component)
    {
        for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {
            if (*it == component) {
                mComponents.erase(it);
                return;
            }
        }
    }

    void AbstractScene::resize(const glm::vec2&)
    {
    }

    void AbstractScene::update(double)
    {
    }

    void AbstractScene::draw(ICanvas*) const
    {
    }

    bool AbstractScene::onTouchBegan(int, const glm::vec2&)
    {
        return false;
    }

    void AbstractScene::onTouchMoved(int, const glm::vec2&)
    {
    }

    void AbstractScene::onTouchEnded(int)
    {
    }

    void AbstractScene::onTouchCancelled(int)
    {
    }

    void AbstractScene::onResize(const glm::vec2& newSize)
    {
        mSize = newSize;
        for (const auto& component : mComponents)
            component->onSceneSizeChanged(this, newSize);
        resize(newSize);
    }

    void AbstractScene::onUpdate(double time)
    {
        for (const auto& component : mComponents)
            component->onBeforeUpdateScene(this, time);
        update(time);
        for (const auto& component : mComponents)
            component->onAfterUpdateScene(this, time);
    }

    void AbstractScene::onDraw(ICanvas* canvas) const
    {
        for (const auto& component : mComponents)
            component->onBeforeDrawScene(this, canvas);
        draw(canvas);
        for (const auto& component : mComponents)
            component->onAfterDrawScene(this, canvas);
    }
}
