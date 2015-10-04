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
#include <cassert>

namespace B3D
{
    #define FOR_EACH_COMPONENT(METHOD) \
        ++mIterating; \
        for (const auto& component : mComponents) \
            component->METHOD; \
        --mIterating;

    #define FOR_EACH_COMPONENT_REVERSE(METHOD) \
        ++mIterating; \
        for (auto it = mComponents.crbegin(); it != mComponents.crend(); ++it) \
            (*it)->METHOD; \
        --mIterating;

    AbstractScene::AbstractScene()
        : mSize(0.0f)
        , mIterating(0)
    {
    }

    AbstractScene::~AbstractScene()
    {
    }

    void AbstractScene::addComponent(const SceneComponentPtr& component)
    {
        assert(!mIterating);

        mComponents.emplace_back(component);
        setSize(mSize);
    }

    void AbstractScene::addComponent(SceneComponentPtr&& component)
    {
        assert(!mIterating);

        mComponents.emplace_back(std::move(component));
        setSize(mSize);
    }

    void AbstractScene::removeComponent(const SceneComponentPtr& component)
    {
        assert(!mIterating);

        for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {
            if (*it == component) {
                mComponents.erase(it);
                setSize(mSize);
                return;
            }
        }
    }

    void AbstractScene::setSize(const glm::vec2& newSize)
    {
        mSize = newSize;
        FOR_EACH_COMPONENT(onBeforeSizeChanged(this, mSize));
        onSizeChanged(mSize);
        FOR_EACH_COMPONENT_REVERSE(onAfterSizeChanged(this, mSize))
    }

    void AbstractScene::performUpdate(double time)
    {
        FOR_EACH_COMPONENT(onBeforeUpdateScene(this, time));
        update(time);
        FOR_EACH_COMPONENT_REVERSE(onAfterUpdateScene(this, time));
    }

    void AbstractScene::performDraw(ICanvas* canvas) const
    {
        FOR_EACH_COMPONENT(onBeforeDrawScene(this, canvas));
        draw(canvas);
        FOR_EACH_COMPONENT_REVERSE(onAfterDrawScene(this, canvas));
    }

    bool AbstractScene::beginTouch(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = false;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::Begin, fingerIndex, p, result));
        if (!result)
            result = onTouchBegan(fingerIndex, position);
        FOR_EACH_COMPONENT_REVERSE(onAfterTouchEvent(TouchEvent::Begin, fingerIndex, p, result));
        return result;
    }

    void AbstractScene::moveTouch(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = false;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::Move, fingerIndex, p, result));
        if (!result)
            onTouchMoved(fingerIndex, position);
        FOR_EACH_COMPONENT_REVERSE(onAfterTouchEvent(TouchEvent::Move, fingerIndex, p, result));
    }

    void AbstractScene::endTouch(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = false;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::End, fingerIndex, p, result));
        if (!result)
            onTouchEnded(fingerIndex, position);
        FOR_EACH_COMPONENT_REVERSE(onAfterTouchEvent(TouchEvent::End, fingerIndex, p, result));
    }

    void AbstractScene::cancelTouch(int fingerIndex, const glm::vec2& position)
    {
        glm::vec2 p = position;
        bool result = false;
        FOR_EACH_COMPONENT(onBeforeTouchEvent(TouchEvent::Cancel, fingerIndex, p, result));
        if (!result)
            onTouchCancelled(fingerIndex, position);
        FOR_EACH_COMPONENT_REVERSE(onAfterTouchEvent(TouchEvent::Cancel, fingerIndex, p, result));
    }

    void AbstractScene::sendEvent(const IEvent* event, bool recursive)
    {
        FOR_EACH_COMPONENT(onBeforeSendEvent(event, recursive));
        onEvent(event);
        FOR_EACH_COMPONENT_REVERSE(onAfterSendEvent(event, recursive));
    }

    void AbstractScene::onSizeChanged(const glm::vec2&)
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

    void AbstractScene::onTouchEnded(int, const glm::vec2&)
    {
    }

    void AbstractScene::onTouchCancelled(int, const glm::vec2&)
    {
    }

    void AbstractScene::onEvent(const IEvent*)
    {
    }
}
