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
    {
        m_Sprite = std::make_shared<CanvasSprite>(sprite);
        addChild(m_Sprite);
        setSize(m_Sprite->size());
    }

    bool Match3View::Item::onPointerPressed(int id, const glm::vec2& pos)
    {
        if (id == 0 && localPointInside(pos)) {
            Z_CHECK(m_View);
            if (m_View) {
                m_View->selectItem(m_X, m_Y);
                m_DragAnchor = pos;
                m_LastDragDirectionX = 0;
                m_LastDragDirectionY = 0;
                return true;
            }
        }
        return false;
    }

    void Match3View::Item::onPointerMoved(int id, const glm::vec2& pos)
    {
        if (id == 0) {
            Z_CHECK(m_View);
            if (m_View) {
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
        if (id == 0) {
            Z_CHECK(m_View);
            cancelDrag(true);
        }
    }

    void Match3View::Item::onPointerCancelled(int id, const glm::vec2&)
    {
        if (id == 0) {
            cancelDrag(false);
        }
    }

    void Match3View::Item::update(double time)
    {
        CanvasElement::update(time);

        if (m_Sprite->position() != m_DragTargetPosition) {
            glm::vec2 delta = m_DragTargetPosition - m_Sprite->position();
            if (fabs(delta.x) <= 1.0f && fabs(delta.y) <= 1.0f)
                m_Sprite->setPosition(m_DragTargetPosition);
            else
                m_Sprite->setPosition(m_Sprite->position() + 10.0f * delta * float(time));
        }
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

        return m_Items[x * m_Field->width() + y];
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

        return m_Items[x * m_Field->width() + y];
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
                    auto item = m_Items[x * width + y];
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
                        m_Items[x * width + y] = std::move(item);
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
}
