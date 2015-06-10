/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include "math/AffineTransform.h"
#include <glm/glm.hpp>
#include <list>
#include <functional>
#include <memory>
#include <vector>

namespace Z
{
    class CanvasElement;
    using CanvasElementPtr = std::shared_ptr<CanvasElement>;

    class CanvasElement : public std::enable_shared_from_this<CanvasElement>
    {
    public:
        class EventFilter
        {
        public:
            virtual ~EventFilter() = default;
            virtual bool filterPointerPressEvent(const CanvasElementPtr&, int, const glm::vec2&) { return false; }
            virtual bool filterPointerMoveEvent(const CanvasElementPtr&, int, const glm::vec2&) { return false; }
            virtual bool filterPointerReleaseEvent(const CanvasElementPtr&, int, const glm::vec2&) { return false; }
            virtual bool filterPointerCancelEvent(const CanvasElementPtr&, int, const glm::vec2&) { return false; }
        };
        using EventFilterPtr = std::shared_ptr<EventFilter>;

        CanvasElement();
        virtual ~CanvasElement();

        CanvasElement* parent() const { return m_Parent; }

        const glm::vec2& position() const { return m_Position; }
        void setPosition(const glm::vec2& position);
        void setPosition(float x, float y) { setPosition(glm::vec2(x, y)); }

        const glm::vec2& size() const { return m_Size; }
        void setSize(const glm::vec2& size);
        void setSize(float x, float y) { setSize(glm::vec2(x, y)); }

        float rotation() const { return m_Rotation; }
        void setRotation(float angle);

        const glm::vec2& scale() const { return m_Scale; }
        void setScale(const glm::vec2& scale);
        void setScale(float scale) { setScale(glm::vec2(scale)); }
        void setScale(float x, float y) { setScale(glm::vec2(x, y)); }

        const glm::vec4& color() const { return m_Color; }
        void setColor(const glm::vec4& color) { m_Color = color; }

        const AffineTransform& localTransform() const;
        const AffineTransform& worldTransform() const;
        const AffineTransform& inverseLocalTransform() const;
        const AffineTransform& inverseWorldTransform() const;

        void addChild(const CanvasElementPtr& child);
        void removeFromParent();
        const std::list<CanvasElementPtr>& children() const { return m_Children; }

        virtual void update(double time);
        void drawIfVisible() const;

        CanvasElementPtr sendPointerPressEvent(int id, const glm::vec2& pos);
        void sendPointerMoveEvent(int id, const glm::vec2& pos);
        void sendPointerReleaseEvent(int id, const glm::vec2& pos);
        void sendPointerCancelEvent(int id, const glm::vec2& pos);

        void setVisible(bool flag = true) { m_Flags = (flag ? m_Flags | Visible : m_Flags &~ Visible); }
        bool isVisible() const { return (m_Flags & Visible) != 0; }

        void setDrawDebugBorder(bool flag = true)
            { m_Flags = (flag ? m_Flags | DrawDebugBorder : m_Flags &~ DrawDebugBorder); }
        bool isDrawDebugBorder() const { return (m_Flags & DrawDebugBorder) != 0; }

        void installEventFilter(const EventFilterPtr& eventFilter);
        void removeEventFilter(const EventFilterPtr& eventFilter);

        bool localPointInside(const glm::vec2& localPoint) const;

    protected:
        using EventFilterVisitor = std::function<bool(const EventFilterPtr&)>;

        virtual void draw() const;

        virtual void onSizeChanged();

        virtual bool onPointerPressed(int id, const glm::vec2& pos);
        virtual void onPointerMoved(int id, const glm::vec2& pos);
        virtual void onPointerReleased(int id, const glm::vec2& pos);
        virtual void onPointerCancelled(int id, const glm::vec2& pos);

        virtual void invalidateLocalTransform();
        virtual void invalidateWorldTransform();

        bool filterEvent(const EventFilterVisitor& callback);

    private:
        enum Flag {
            LocalTransformDirty         = 0x00000001,
            InverseLocalTransformDirty  = 0x00000002,
            WorldTransformDirty         = 0x00000004,
            InverseWorldTransformDirty  = 0x00000008,
            InSizeChanged               = 0x00000010,
            Visible                     = 0x00000020,
            DrawDebugBorder             = 0x00000040,
        };

        CanvasElement* m_Parent;
        std::list<CanvasElementPtr> m_Children;
        std::list<CanvasElementPtr>::iterator m_SelfRef;
        std::vector<std::weak_ptr<EventFilter>> m_EventFilters;
        mutable AffineTransform m_LocalTransform;
        mutable AffineTransform m_InverseLocalTransform;
        mutable AffineTransform m_WorldTransform;
        mutable AffineTransform m_InverseWorldTransform;
        glm::vec4 m_Color;
        glm::vec2 m_Position;
        glm::vec2 m_Size;
        glm::vec2 m_Scale;
        float m_Rotation;
        mutable int m_Flags;

        CanvasElement(const CanvasElement&) = delete;
        CanvasElement& operator=(const CanvasElement&) = delete;
    };
}
