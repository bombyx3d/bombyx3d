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
#include "engine/core/EventDispatcher.h"
#include <vector>
#include <memory>

namespace B3D
{
    class AbstractScene : public IScene, public IEventObserver
    {
    public:
        ~AbstractScene();

        void addComponent(const SceneComponentPtr& component) override;
        void addComponent(SceneComponentPtr&& component) override;
        void removeComponent(const SceneComponentPtr& component) override;

        const glm::vec2& size() const final override { return mSize; }
        void setSize(const glm::vec2& newSize) final override;

        void performUpdate(double time) final override;
        void performDraw(ICanvas* canvas) const final override;

        bool beginTouch(int fingerIndex, const glm::vec2& position) final override;
        void moveTouch(int fingerIndex, const glm::vec2& position) final override;
        void endTouch(int fingerIndex, const glm::vec2& position) final override;
        void cancelTouch(int fingerIndex, const glm::vec2& position) final override;

        void sendEvent(const IEvent* event, bool recursive = true) final override;

        template <class TYPE> void addEventObserver(void* key, const std::function<void()>& handler)
        {
            addEventObserver<TYPE>(key, [handler](const TYPE&){ handler(); });
        }

        template <class TYPE> void addEventObserver(void* key, std::function<void(const TYPE&)>&& handler)
        {
            auto typeId = typeOf<TYPE>();
            EventObserverPtr observer(new EventObserver<TYPE>(std::move(handler)));
            eventDispatcher().addObserver(typeId, observer.get());
            eventObserverMap()[key].emplace_back(typeId, std::move(observer));
        }

        template <class TYPE> void addEventObserver(void* key, const std::function<void(const TYPE&)>& handler)
        {
            EventObserverPtr observer(new EventObserver<TYPE>(handler));
            eventDispatcher().addObserver(typeOf<TYPE>(), observer.get());
            eventObserverMap()[key].emplace_back(std::move(observer));
        }

        void removeEventObservers(void* key);

    protected:
        AbstractScene();

        virtual void onSizeChanged(const glm::vec2& newSize);

        virtual void update(double time);
        virtual void draw(ICanvas* canvas) const;

        virtual bool onTouchBegan(int fingerIndex, const glm::vec2& position);
        virtual void onTouchMoved(int fingerIndex, const glm::vec2& position);
        virtual void onTouchEnded(int fingerIndex, const glm::vec2& position);
        virtual void onTouchCancelled(int fingerIndex, const glm::vec2& position);

        void onEvent(const IEvent* event) override;

    private:
        template <class TYPE> struct EventObserver : public IEventObserver
        {
        public:
            explicit EventObserver(std::function<void(const TYPE&)>&& function)
                : mHandlerFunction(std::move(function)) {}

            explicit EventObserver(const std::function<void(const TYPE&)>& function)
                : mHandlerFunction(function) {}

            void onEvent(const IEvent* event) override
                { mHandlerFunction(*reinterpret_cast<const TYPE*>(event->rawData())); }

        private:
            std::function<void(const TYPE&)> mHandlerFunction;
        };

        using EventObserverPtr = std::unique_ptr<IEventObserver>;
        using EventObserverList = std::vector<std::pair<TypeID, EventObserverPtr>>;
        using EventObserverMap = std::unordered_map<void*, EventObserverList>;

        std::vector<SceneComponentPtr> mComponents;
        std::unique_ptr<EventObserverMap> mEventObserverMap;
        std::unique_ptr<EventDispatcher> mEventDispatcher;
        glm::vec2 mSize;
        mutable int mIterating;

        EventDispatcher& eventDispatcher();
        EventObserverMap& eventObserverMap();

        B3D_DISABLE_COPY(AbstractScene);
    };
}
