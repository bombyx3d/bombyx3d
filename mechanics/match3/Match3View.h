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

namespace Z
{
    class Match3View : public CanvasElement, private Match3Listener
    {
    public:
        explicit Match3View(Match3SpriteFactory* factory);
        ~Match3View();

        void setCellSpacing(float spacing);

        void setCellSize(float w, float h) { m_CellWidth = w; m_CellHeight = h; m_CustomCellSize = true; }
        void resetCellSize();

        void setField(const Match3FieldPtr& field);

        void update(double time) override;

    private:
        Match3FieldPtr m_Field;
        Match3SpriteFactory* m_SpriteFactory;
        std::vector<CanvasSpritePtr> m_Sprites;
        float m_CellWidth;
        float m_CellHeight;
        float m_CellSpacing;
        bool m_ShouldSetSpritesPositions;
        bool m_CustomCellSize;

        void calcCellSize();
        void setSpritesPositions();

        void createSpritesForElements();
        void destroySprites();
    };

    using Match3ViewPtr = std::shared_ptr<Match3View>;
}
