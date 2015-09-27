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
    #define FOR_EACH_COMPONENT(METHOD) \
        for (const auto& component : mComponents) \
            component->METHOD;

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

    bool AbstractScene::beginTouch(int, const glm::vec2&)
    {
        return false;
    }

    void AbstractScene::moveTouch(int, const glm::vec2&)
    {
    }

    void AbstractScene::endTouch(int, const glm::vec2&)
    {
    }

    void AbstractScene::cancelTouch(int, const glm::vec2&)
    {
    }

    void AbstractScene::onResize(const glm::vec2& newSize)
    {
        mSize = newSize;
        FOR_EACH_COMPONENT(onSceneSizeChanged(this, newSize));
        resize(newSize);
    }

    void AbstractScene::onUpdate(double time)
    {
        FOR_EACH_COMPONENT(onBeforeUpdateScene(this, time));
        update(time);
        FOR_EACH_COMPONENT(onAfterUpdateScene(this, time));
    }

    void AbstractScene::onDraw(ICanvas* canvas) const
    {
        FOR_EACH_COMPONENT(onBeforeDrawScene(this, canvas));
        draw(canvas);
        FOR_EACH_COMPONENT(onAfterDrawScene(this, canvas));
    }

    bool AbstractScene::onTouchBegan(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = false;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::Begin, p, result));
        if (!result)
            result = beginTouch(fingerIndex, position);
        FOR_EACH_COMPONENT(onAfterTouchEvent(TouchEvent::Begin, p, result));
        return result;
    }

    void AbstractScene::onTouchMoved(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = true;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::Move, p, result));
        moveTouch(fingerIndex, position);
        FOR_EACH_COMPONENT(onAfterTouchEvent(TouchEvent::Move, p, result));
    }

    void AbstractScene::onTouchEnded(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = true;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::End, p, result));
        endTouch(fingerIndex, position);
        FOR_EACH_COMPONENT(onAfterTouchEvent(TouchEvent::End, p, result));
    }

    void AbstractScene::onTouchCancelled(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = true;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::Cancel, p, result));
        cancelTouch(fingerIndex, position);
        FOR_EACH_COMPONENT(onAfterTouchEvent(TouchEvent::Cancel, p, result));
    }
}
