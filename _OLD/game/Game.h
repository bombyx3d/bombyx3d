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
#include <glm/glm.hpp>

#define Z_GAME_CLASS(CLASS) \
    ::Z::Game* ::Z::Game::create() { \
        ::Z::Game* game = new ::CLASS; \
        return game; \
    }

namespace Z
{
    class Game
    {
    public:
        Game() = default;
        virtual ~Game() = default;

        static Game* create();

        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void runFrame(double time) = 0;

        virtual void onPointerPressed(int id, const glm::vec2& pos);
        virtual void onPointerMoved(int id, const glm::vec2& pos);
        virtual void onPointerReleased(int id, const glm::vec2& pos);
        virtual void onPointerCancelled(int id, const glm::vec2& pos);
    };
}
