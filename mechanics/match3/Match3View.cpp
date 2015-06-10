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
#include "Match3View.h"
#include "utility/debug.h"
#include <algorithm>

namespace Z
{
    static Match3View::ItemPtr g_NullItem;


    Match3View::Item::Item(Match3View* view, int x, int y, const SpritePtr& sprite)
        : m_View(view)
        , m_X(x)
        , m_Y(y)
        , m_Dragging(false)
    {
        m_Sprite = std::make_shared<CanvasSprite>(sprite);
        addChild(m_Sprite);
        setSize(m_Sprite->size());
    }

    bool Match3View::Item::onPointerPressed(int id, const glm::vec2& pos)
    {
        if (id == 0 && localPointInside(pos)) {
            Z_CHECK(m_View);
            Z_CHECK(!m_Dragging);
            if (m_View) {
                Z_CHECK(m_View->m_Field);
                if (!m_View->m_Field)
                    return false;

                if (!m_View->m_Animations.empty()) {
                    m_Dragging = false;
                    return true;
                }

                if (m_View->m_SelectedItem && m_View->m_SelectedItem.get() != this) {
                    int otherX = m_View->m_SelectedItem->x();
                    int otherY = m_View->m_SelectedItem->y();
                    if (m_View->m_Field->swapItems(otherX, otherY, m_X, m_Y, false)) {
                        m_Dragging = false;
                        return true;
                    }
                }

                m_View->selectItem(m_X, m_Y);

                m_DragAnchor = pos;
                m_Dragging = true;
                m_LastDragDirectionX = 0;
                m_LastDragDirectionY = 0;

                return true;
            }
        }

        return false;
    }

    void Match3View::Item::onPointerMoved(int id, const glm::vec2& pos)
    {
        if (id == 0 && m_Dragging) {
            Z_CHECK(m_View);
            if (m_View) {
                if (!m_View->m_Animations.empty())
                    return;

                glm::vec2 offset = pos - m_DragAnchor;
                int dirX = (pos.x >= 0 && pos.x < size().x ? 0 : (offset.x < 0.0f ? -1 : 1));
                int dirY = (pos.y >= 0 && pos.y < size().y ? 0 : (offset.y < 0.0f ? -1 : 1));

                if (dirX == 0 && dirY == 0) {
                    cancelDrag(true);
                    return;
                }

                if (m_LastDragDirectionX != 0) {
                    if (dirX == 0)
                        dirX = m_LastDragDirectionX;
                    dirY = 0;
                } else if (m_LastDragDirectionY != 0) {
                    dirX = 0;
                    if (dirY == 0)
                        dirY = m_LastDragDirectionY;
                }

                if (dirX != 0 && dirY != 0) {
                    cancelDrag(true);
                    return;
                }

                Z_CHECK((dirX == 0 || dirY == 0) && (dirX != 0 || dirY != 0));
                m_LastDragDirectionX = dirX;
                m_LastDragDirectionY = dirY;

                int targetX = m_X + dirX;
                int targetY = m_Y + dirY;

                if (!m_View->isValidItemXY(targetX, targetY)) {
                    cancelDrag(true);
                    return;
                }

                auto item = m_View->itemAt(targetX, targetY);
                if (!item) {
                    cancelDrag(true);
                    return;
                }

                if (m_SwappingWithItem != item) {
                    if (m_SwappingWithItem)
                        m_SwappingWithItem->cancelDrag(true);
                    m_SwappingWithItem = item;
                }

                m_DragTargetPosition = inverseLocalTransform().transform(item->position());
                item->m_DragTargetPosition = item->inverseLocalTransform().transform(position());
            }
        }
    }

    void Match3View::Item::onPointerReleased(int id, const glm::vec2& pos)
    {
        if (id == 0 && m_Dragging) {
            m_Dragging = false;

            auto item2 = m_SwappingWithItem;
            cancelDrag(true);

            if (item2) {
                Z_CHECK(m_View->m_Field);
                if (m_View->m_Field)
                    m_View->m_Field->swapItems(m_X, m_Y, item2->x(), item2->y(), true);
            }
        }
    }

    void Match3View::Item::onPointerCancelled(int id, const glm::vec2&)
    {
        if (id == 0 && m_Dragging) {
            cancelDrag(false);
            m_Dragging = false;
        }
    }

    void Match3View::Item::update(double time)
    {
        CanvasElement::update(time);
        m_View->animateSpritePosition(m_Sprite, m_DragTargetPosition, time);
    }

    void Match3View::Item::cancelDrag(bool animated)
    {
        if (m_SwappingWithItem) {
            m_SwappingWithItem->m_DragTargetPosition = glm::vec2(0.0f);
            if (!animated)
                m_SwappingWithItem->sprite()->setPosition(glm::vec2(0.0f));
            m_SwappingWithItem.reset();
        }

        m_DragTargetPosition = glm::vec2(0.0f);
        if (!animated)
            m_Sprite->setPosition(glm::vec2(0.0f));

        m_LastDragDirectionX = 0;
        m_LastDragDirectionY = 0;
    }


    Match3View::Match3View(Match3SpriteFactory* factory)
        : m_SpriteFactory(factory)
        , m_CellWidth(0.0f)
        , m_CellHeight(0.0f)
        , m_CellSpacing(5.0f)
        , m_ShouldSetItemsPositions(false)
        , m_CustomCellSize(false)
    {
        Z_ASSERT(factory != nullptr);
    }

    Match3View::~Match3View()
    {
        if (m_Field)
            m_Field->removeListener(this);
    }

    void Match3View::setCellSpacing(float spacing)
    {
        if (m_CellSpacing != spacing) {
            m_CellSpacing = spacing;
            m_ShouldSetItemsPositions = true;
        }
    }

    void Match3View::resetCellSize()
    {
        if (m_CustomCellSize) {
            m_CustomCellSize = false;
            calcCellSize();
        }
    }

    void Match3View::setField(const Match3FieldPtr& field)
    {
        if (m_Field != field) {
            m_Animations.clear();
            destroyItems();

            if (m_Field)
                m_Field->removeListener(this);

            m_Field = field;

            if (m_Field)
                m_Field->addListener(this);

            createItems();
            calcCellSize();
        }
    }

    void Match3View::update(double time)
    {
        if (m_ShouldSetItemsPositions) {
            setItemsPositions();
            m_ShouldSetItemsPositions = false;
        }

        if (!m_Animations.empty()) {
            const auto& func = m_Animations.front();
            if (func(time))
                m_Animations.pop_front();
        }

        CanvasElement::update(time);
    }

    bool Match3View::isValidItemXY(int x, int y) const
    {
        Z_CHECK(m_Field);
        return (m_Field && x >= 0 && y >= 0 && x < m_Field->width() && y < m_Field->height());
    }

    const Match3View::ItemPtr& Match3View::itemAt(int x, int y)
    {
        Z_CHECK(m_Field);
        if (!m_Field)
            return g_NullItem;

        Z_CHECK(isValidItemXY(x, y));
        if (!isValidItemXY(x, y))
            return g_NullItem;

        return m_Items[y * m_Field->width() + x];
    }

    const Match3View::ItemPtr& Match3View::itemAtPoint(const glm::vec2& position)
    {
        Z_CHECK(m_Field);
        if (!m_Field)
            return g_NullItem;

        int x = int(floor(position.x / (m_CellWidth + m_CellSpacing)));
        int y = int(floor(position.y / (m_CellHeight + m_CellSpacing)));
        if (!isValidItemXY(x, y))
            return g_NullItem;

        return m_Items[y * m_Field->width() + x];
    }

    void Match3View::selectItem(int x, int y)
    {
        auto item = itemAt(x, y);
        if (item != m_SelectedItem) {
            deselectSelectedItem();
            m_SelectedItem = item;
            if (item) {
                item->removeFromParent();
                addChild(item);             // reinsert item to put it over all other items
                selectItem(item);
            }
        }
    }

    void Match3View::deselectSelectedItem()
    {
        if (m_SelectedItem) {
            deselectItem(m_SelectedItem);
            m_SelectedItem.reset();
        }
    }

    void Match3View::selectItem(const ItemPtr& item)
    {
        item->setDrawDebugBorder(true);
    }

    void Match3View::deselectItem(const ItemPtr& item)
    {
        item->setDrawDebugBorder(false);
    }

    void Match3View::calcCellSize()
    {
        if (!m_CustomCellSize) {
            float cellWidth = 0.0f;
            float cellHeight = 0.0f;
            for (const auto& item : m_Items) {
                if (item) {
                    cellWidth = std::max(cellWidth, item->size().x);
                    cellHeight = std::max(cellHeight, item->size().y);
                }
            }

            if (m_CellWidth != cellWidth || m_CellHeight != cellHeight) {
                m_CellWidth = cellWidth;
                m_CellHeight = cellHeight;
                m_ShouldSetItemsPositions = true;
            }
        }
    }

    void Match3View::setItemsPositions()
    {
        if (!m_Field) {
            Z_CHECK(m_Items.size() == 0);
            setSize(0.0f, 0.0f);
        } else {
            int width = m_Field->width();
            int height = m_Field->height();
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    auto item = m_Items[y * width + x];
                    if (item) {
                        float posX = float(x) * (m_CellWidth + m_CellSpacing);
                        float posY = float(y) * (m_CellHeight + m_CellSpacing);
                        item->setPosition(posX, posY);
                    }
                }
            }

            glm::vec2 size;
            size.x = std::max(0.0f, float(width) * (m_CellWidth + m_CellSpacing) - m_CellSpacing);
            size.y = std::max(0.0f, float(height) * (m_CellHeight + m_CellSpacing) - m_CellSpacing);
            setSize(size);
        }
    }

    void Match3View::createItems()
    {
        Z_CHECK(m_Items.size() == 0);
        if (m_Field) {
            int width = m_Field->width();
            int height = m_Field->height();
            m_Items.resize(width * height);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int8_t index = m_Field->elementAt(x, y);
                    if (index >= 0) {
                        auto item = std::make_shared<Item>(this, x, y, m_SpriteFactory->spriteForElement(index));
                        addChild(item);
                        m_Items[y * width + x] = std::move(item);
                    }
                }
            }
            m_ShouldSetItemsPositions = true;
        }
    }

    void Match3View::destroyItems()
    {
        deselectSelectedItem();
        for (const auto& item : m_Items) {
            if (item) {
                item->m_View = nullptr;
                item->removeFromParent();
            }
        }
        m_Items.clear();
    }

    void Match3View::cancelItemsDrag()
    {
        if (m_Field) {
            int width = m_Field->width();
            int height = m_Field->height();
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    auto item = m_Items[y * width + x];
                    if (item) {
                        item->cancelDrag(false);
                        item->m_Dragging = false;
                    }
                }
            }
            m_ShouldSetItemsPositions = true;
        }
    }

    void Match3View::onItemsSwapped(int x1, int y1, int x2, int y2)
    {
        deselectSelectedItem();

        Z_CHECK(m_Field);
        if (!m_Field) {
            cancelItemsDrag();
            return;
        }

        Z_CHECK(isValidItemXY(x1, y1));
        if (!isValidItemXY(x1, y1)) {
            cancelItemsDrag();
            return;
        }

        Z_CHECK(isValidItemXY(x2, y2));
        if (!isValidItemXY(x2, y2)) {
            cancelItemsDrag();
            return;
        }

        auto& item1 = m_Items[y1 * m_Field->width() + x1];
        auto& item2 = m_Items[y2 * m_Field->width() + x2];

        Z_ASSERT(item1 != nullptr);
        Z_ASSERT(item2 != nullptr);

        std::swap(item1, item2);
        item1->setXY(x1, y1);
        item2->setXY(x2, y2);

        float posX1 = float(x1) * (m_CellWidth + m_CellSpacing);
        float posY1 = float(y1) * (m_CellHeight + m_CellSpacing);
        float posX2 = float(x2) * (m_CellWidth + m_CellSpacing);
        float posY2 = float(y2) * (m_CellHeight + m_CellSpacing);

        item1->setPosition(posX1, posY1);
        item2->setPosition(posX2, posY2);

        auto sprite1 = item1->sprite();
        auto sprite2 = item2->sprite();

        glm::vec2 sprite1Position = item2->localTransform().transform(sprite1->position());
        glm::vec2 sprite2Position = item1->localTransform().transform(sprite2->position());
        cancelItemsDrag();

        sprite1->setPosition(item1->inverseLocalTransform().transform(sprite1Position));
        sprite2->setPosition(item2->inverseLocalTransform().transform(sprite2Position));

        m_Animations.push_back([this, sprite1, sprite2](double time) -> bool {
            bool done1 = !animateSpritePosition(sprite1, glm::vec2(0.0f), time);
            bool done2 = !animateSpritePosition(sprite2, glm::vec2(0.0f), time);
            return done1 && done2;
        });

      #if Z_ASSERTIONS_ENABLED
        int width = m_Field->width();
        int height = m_Field->height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int8_t index = m_Field->elementAt(x, y);
                auto item = m_Items[y * width + x];
                if (index < 0) {
                    Z_ASSERT(item == nullptr);
                } else {
                    Z_ASSERT(item != nullptr);
                    Z_ASSERT(item->sprite()->sprite() == m_SpriteFactory->spriteForElement(index));
                }
            }
        }
      #endif
    }

    bool Match3View::animateSpritePosition(const CanvasSpritePtr& sprite, const glm::vec2& targetPosition, double time)
    {
        if (sprite->position() != targetPosition) {
            glm::vec2 delta = targetPosition - sprite->position();
            if (fabs(delta.x) <= 1.0f && fabs(delta.y) <= 1.0f)
                sprite->setPosition(targetPosition);
            else
                sprite->setPosition(sprite->position() + 10.0f * delta * float(time));
            return true;
        }
        return false;
    }
}
