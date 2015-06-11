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
#include "Match3Field.h"
#include <glm/glm.hpp>

namespace Z
{
    class Match3Listener
    {
    public:
        virtual ~Match3Listener() = default;

        virtual void onItemsSwapped(int x1, int y1, int x2, int y2) { (void)x1; (void)y1; (void)x2; (void)y2; }
        virtual void onItemsSwapFailed() {}

        virtual void onChainsMatched(const std::vector<Match3Field::Chain>& chains) { (void)chains; }

        virtual void onMatchesKilled(const std::vector<glm::ivec2>& fallenItems,
            const std::vector<glm::ivec2>& spawnedItems) { (void)fallenItems; (void)spawnedItems; }
        virtual void onAllMatchesKilled() {}
    };
}
