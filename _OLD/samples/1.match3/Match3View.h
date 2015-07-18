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
#include "canvas/CanvasElement.h"
#include "canvas/CanvasSprite.h"
#include "Match3Field.h"
#include "Match3SpriteFactory.h"
#include "Match3Listener.h"
#include <memory>
#include <functional>
#include <deque>

namespace Z
{
    class Match3View : public CanvasElement, private Match3Listener
    {
    public:
        class Item : public CanvasElement
        {
        public:
            Item(Match3View* view, int x, int y, const SpritePtr& sprite);

            const CanvasSpritePtr& sprite() const { return m_Sprite; }

            int x() const { return m_X; }
            int y() const { return m_Y; }
            void setXY(int posX, int posY) { m_X = posX; m_Y = posY; }

            void setSpritePosition(float x, float y);
            void setSpritePosition(const glm::vec2& pos);

            bool onPointerPressed(int id, const glm::vec2& pos) override;
            void onPointerMoved(int id, const glm::vec2& pos) override;
            void onPointerReleased(int id, const glm::vec2& pos) override;
            void onPointerCancelled(int id, const glm::vec2& pos) override;

            void update(double time) override;

        private:
            CanvasSpritePtr m_Sprite;
            Match3View* m_View;
            std::shared_ptr<Item> m_SwappingWithItem;
            glm::vec2 m_DragTargetPosition;
            glm::vec2 m_DragAnchor;
            int m_LastDragDirectionX;
            int m_LastDragDirectionY;
            int m_X;
            int m_Y;
            bool m_Dragging;

            void cancelDrag(bool animated);

            friend class Match3View;
        };
        using ItemPtr = std::shared_ptr<Item>;

        explicit Match3View(Match3SpriteFactory* factory);
        ~Match3View();

        void setCellSpacing(float spacing);

        void setCellSize(float w, float h) { m_CellWidth = w; m_CellHeight = h; m_CustomCellSize = true; }
        void resetCellSize();

        const Match3FieldPtr& field() const { return m_Field; }
        void setField(const Match3FieldPtr& field);

        void update(double time) override;

        bool isValidItemXY(int x, int y) const;
        const ItemPtr& itemAt(int x, int y);
        const ItemPtr& itemAtPoint(const glm::vec2& position);

        const ItemPtr& selectedItem() const { return m_SelectedItem; }
        void selectItem(int x, int y);
        void deselectSelectedItem();

    protected:
        virtual void selectItem(const ItemPtr& item);
        virtual void deselectItem(const ItemPtr& item);

    private:
        Match3FieldPtr m_Field;
        Match3SpriteFactory* m_SpriteFactory;
        std::vector<ItemPtr> m_Items;
        std::deque<std::function<bool(double)>> m_Animations;
        ItemPtr m_SelectedItem;
        float m_CellWidth;
        float m_CellHeight;
        float m_CellSpacing;
        bool m_ShouldSetItemsPositions;
        bool m_CustomCellSize;

        void calcCellSize();
        void setItemsPositions();

        void createItems();
        void destroyItems();

        void cancelItemsDrag();

        void onItemsSwapped(int x1, int y1, int x2, int y2) override;
        void onMatchesKilled(const std::vector<glm::ivec2>& fallenItems,
            const std::vector<glm::ivec2>& spawnedItems) override;
        void onAllMatchesKilled() override;
        void onChainsMatched(const std::vector<Match3Field::Chain>& chains) override;

        bool animateSpritePosition(const ItemPtr& item, const glm::vec2& targetPosition, double time);
        bool animateSpriteFade(const CanvasSpritePtr& sprite, double time);

        void validateSprites() const;

        friend class Item;
    };

    using Match3ViewPtr = std::shared_ptr<Match3View>;
}
