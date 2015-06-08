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
#include "CanvasElement.h"
#include "Engine.h"
#include "utility/debug.h"

namespace Z
{
    CanvasElement::CanvasElement()
        : m_Parent(nullptr)
        , m_Position(0.0f)
        , m_Size(0.0f)
        , m_Scale(1.0f)
        , m_Rotation(0.0f)
        , m_Flags(LocalTransformDirty | InverseLocalTransformDirty | WorldTransformDirty
            | InverseWorldTransformDirty | Visible)
    {
        m_LocalTransform.setIdentity();
    }

    CanvasElement::~CanvasElement()
    {
        for (const auto& children : m_Children) {
            Z_CHECK(children->m_Parent == this);
            children->m_Parent = nullptr;
        }
    }

    void CanvasElement::setPosition(const glm::vec2& position)
    {
        if (m_Position != position) {
            m_Position = position;
            invalidateLocalTransform();
        }
    }

    void CanvasElement::setSize(const glm::vec2& size)
    {
        if (m_Size != size) {
            m_Size = size;
            if (!(m_Flags & InSizeChanged)) {
                m_Flags |= InSizeChanged;
                onSizeChanged();
                m_Flags &= ~InSizeChanged;
            }
        }
    }

    void CanvasElement::setRotation(float angle)
    {
        if (m_Rotation != angle) {
            m_Rotation = angle;
            invalidateLocalTransform();
        }
    }

    void CanvasElement::setScale(const glm::vec2& scale)
    {
        if (m_Scale != scale) {
            m_Scale = scale;
            invalidateLocalTransform();
        }
    }

    const AffineTransform& CanvasElement::localTransform() const
    {
        if (m_Flags & LocalTransformDirty) {
            m_LocalTransform.setTranslationRotationScale(m_Position, m_Rotation, m_Scale);
            m_Flags &= ~LocalTransformDirty;
        }
        return m_LocalTransform;
    }

    const AffineTransform& CanvasElement::inverseLocalTransform() const
    {
        if (m_Flags & InverseLocalTransformDirty) {
            m_InverseLocalTransform = localTransform().inverse();
            m_Flags &= ~InverseLocalTransformDirty;
        }
        return m_InverseLocalTransform;
    }

    const AffineTransform& CanvasElement::worldTransform() const
    {
        if (m_Flags & WorldTransformDirty) {
            if (!m_Parent)
                m_WorldTransform = m_LocalTransform;
            else
                m_WorldTransform = AffineTransform(m_Parent->worldTransform(), m_LocalTransform);
            m_Flags &= ~WorldTransformDirty;
        }
        return m_WorldTransform;
    }

    const AffineTransform& CanvasElement::inverseWorldTransform() const
    {
        if (m_Flags & InverseWorldTransformDirty) {
            m_InverseWorldTransform = worldTransform().inverse();
            m_Flags &= ~InverseWorldTransformDirty;
        }
        return m_InverseWorldTransform;
    }

    void CanvasElement::addChild(const CanvasElementPtr& child)
    {
        Z_CHECK(child != nullptr);
        if (!child)
            return;

        Z_ASSERT(child->m_Parent == nullptr);
        child->removeFromParent();

        child->m_SelfRef = m_Children.insert(m_Children.end(), child);
        child->m_Parent = this;
        child->invalidateWorldTransform();
    }

    void CanvasElement::removeFromParent()
    {
        if (m_Parent) {
            m_Parent->m_Children.erase(m_SelfRef);
            m_Parent = nullptr;
            invalidateWorldTransform();
        }
    }

    void CanvasElement::update(double time)
    {
        for (const auto& child : m_Children)
            child->update(time);
    }

    void CanvasElement::drawIfVisible() const
    {
        if (isVisible()) {
            auto& renderer = Engine::instance().renderer();
            renderer.modelViewStack().pushApply(localTransform());

            draw();

            if (m_Flags & DrawDebugBorder) {
                auto currentShader = renderer.currentShader();
                static auto debugShader = renderer.loadShader(Renderer::DEFAULT_COLORED_2D_SHADER);
                renderer.setShader(debugShader);
                renderer.drawRect(Quad::fromTopLeftAndSize(glm::vec2(0.0f, 0.0f), m_Size));
                renderer.setShader(currentShader);
            }

            renderer.modelViewStack().pop();
        }
    }

    CanvasElementPtr CanvasElement::sendPointerPressEvent(int id, const glm::vec2& pos)
    {
        if (isVisible()) {
            glm::vec2 point = inverseLocalTransform().transform(pos);

            if (filterEvent([this, id, &point](const EventFilterPtr& filter) -> bool {
                    return filter->filterPointerPressEvent(shared_from_this(), id, point);
                }))
                return shared_from_this();

            for (auto it = m_Children.rbegin(); it != m_Children.rend(); ++it) {
                CanvasElementPtr result = (*it)->sendPointerPressEvent(id, point);
                if (result)
                    return result;
            }

            if (onPointerPressed(id, point))
                return shared_from_this();
        }
        return CanvasElementPtr();
    }

    void CanvasElement::sendPointerMoveEvent(int id, const glm::vec2& pos)
    {
        glm::vec2 point = inverseWorldTransform().transform(pos);

        if (filterEvent([this, id, &point](const EventFilterPtr& filter) -> bool {
                return filter->filterPointerMoveEvent(shared_from_this(), id, point);
            }))
            return;

        onPointerMoved(id, point);
    }

    void CanvasElement::sendPointerReleaseEvent(int id, const glm::vec2& pos)
    {
        glm::vec2 point = inverseWorldTransform().transform(pos);

        if (filterEvent([this, id, &point](const EventFilterPtr& filter) -> bool {
                return filter->filterPointerReleaseEvent(shared_from_this(), id, point);
            }))
            return;

        onPointerReleased(id, point);
    }

    void CanvasElement::sendPointerCancelEvent(int id, const glm::vec2& pos)
    {
        glm::vec2 point = inverseWorldTransform().transform(pos);

        if (filterEvent([this, id, &point](const EventFilterPtr& filter) -> bool {
                return filter->filterPointerCancelEvent(shared_from_this(), id, point);
            }))
            return;

        onPointerCancelled(id, point);
    }

    void CanvasElement::installEventFilter(const EventFilterPtr& eventFilter)
    {
        m_EventFilters.emplace_back(eventFilter);
    }

    void CanvasElement::removeEventFilter(const EventFilterPtr& eventFilter)
    {
        for (auto it = m_EventFilters.begin(); it != m_EventFilters.end(); ) {
            auto filter = it->lock();
            if (filter && filter != eventFilter)
                ++it;
            else
                it = m_EventFilters.erase(it);
        }
    }

    bool CanvasElement::localPointInside(const glm::vec2& localPoint) const
    {
        return (localPoint.x >= 0 && localPoint.y >= 0 && localPoint.x < m_Size.x && localPoint.y < m_Size.y);
    }

    void CanvasElement::draw() const
    {
        for (const auto& child : m_Children)
            child->drawIfVisible();
    }

    void CanvasElement::onSizeChanged()
    {
    }

    bool CanvasElement::onPointerPressed(int, const glm::vec2&)
    {
        return false;
    }

    void CanvasElement::onPointerMoved(int, const glm::vec2&)
    {
    }

    void CanvasElement::onPointerReleased(int, const glm::vec2&)
    {
    }

    void CanvasElement::onPointerCancelled(int, const glm::vec2&)
    {
    }

    void CanvasElement::invalidateLocalTransform()
    {
        m_Flags |= LocalTransformDirty | InverseLocalTransformDirty;
        invalidateWorldTransform();
    }

    void CanvasElement::invalidateWorldTransform()
    {
        if (!(m_Flags & (WorldTransformDirty | InverseWorldTransformDirty))) {
            m_Flags |= WorldTransformDirty | InverseWorldTransformDirty;
            for (const auto& child : m_Children)
                child->invalidateWorldTransform();
        }
    }

    bool CanvasElement::filterEvent(const EventFilterVisitor& callback)
    {
        if (m_EventFilters.empty())
            return false;

        std::vector<EventFilterPtr> eventFilters;
        eventFilters.reserve(m_EventFilters.size());

        for (auto it = m_EventFilters.begin(); it != m_EventFilters.end(); ) {
            auto filter = it->lock();
            if (!filter)
                it = m_EventFilters.erase(it);
            else {
                ++it;
                eventFilters.emplace_back(std::move(filter));
            }
        }

        for (const auto& filter: eventFilters) {
            if (callback(filter))
                return true;
        }

        return false;
    }
}
