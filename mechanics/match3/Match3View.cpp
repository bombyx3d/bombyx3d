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
    Match3View::Match3View(Match3SpriteFactory* factory)
        : m_SpriteFactory(factory)
        , m_CellWidth(0.0f)
        , m_CellHeight(0.0f)
        , m_CellSpacing(0.0f)
        , m_ShouldSetSpritesPositions(false)
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
            m_ShouldSetSpritesPositions = true;
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
            destroySprites();

            if (m_Field)
                m_Field->removeListener(this);

            m_Field = field;

            if (m_Field)
                m_Field->addListener(this);

            createSpritesForElements();
            calcCellSize();
        }
    }

    void Match3View::update(double time)
    {
        if (m_ShouldSetSpritesPositions) {
            setSpritesPositions();
            m_ShouldSetSpritesPositions = false;
        }

        CanvasElement::update(time);
    }

    void Match3View::calcCellSize()
    {
        if (!m_CustomCellSize) {
            float cellWidth = 0.0f;
            float cellHeight = 0.0f;
            for (const auto& sprite : m_Sprites) {
                cellWidth = std::max(cellWidth, sprite->size().x);
                cellHeight = std::max(cellHeight, sprite->size().y);
            }

            if (m_CellWidth != cellWidth || m_CellHeight != cellHeight) {
                m_CellWidth = cellWidth;
                m_CellHeight = cellHeight;
                m_ShouldSetSpritesPositions = true;
            }
        }
    }

    void Match3View::setSpritesPositions()
    {
        if (!m_Field) {
            Z_CHECK(m_Sprites.size() == 0);
            setSize(0.0f, 0.0f);
        } else {
            int width = m_Field->width();
            int height = m_Field->height();
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float posX = float(x) * (m_CellWidth + m_CellSpacing);
                    float posY = float(y) * (m_CellHeight + m_CellSpacing);
                    m_Sprites[x * width + y]->setPosition(posX, posY);
                }
            }

            glm::vec2 size;
            size.x = std::max(0.0f, float(width) * (m_CellWidth + m_CellSpacing) - m_CellSpacing);
            size.y = std::max(0.0f, float(height) * (m_CellHeight + m_CellSpacing) - m_CellSpacing);
            setSize(size);
        }
    }

    void Match3View::createSpritesForElements()
    {
        Z_CHECK(m_Sprites.size() == 0);
        if (m_Field) {
            int width = m_Field->width();
            int height = m_Field->height();
            m_Sprites.resize(width * height);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int8_t index = m_Field->elementAt(x, y);
                    if (index >= 0) {
                        auto sprite = std::make_shared<CanvasSprite>(m_SpriteFactory->spriteForElement(index));
                        addChild(sprite);
                        m_Sprites[x * width + y] = std::move(sprite);
                    }
                }
            }
            m_ShouldSetSpritesPositions = true;
        }
    }

    void Match3View::destroySprites()
    {
        for (const auto& sprite : m_Sprites)
            sprite->removeFromParent();
        m_Sprites.clear();
    }
}
