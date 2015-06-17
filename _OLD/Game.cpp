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
#include "Game.h"

namespace Z
{
    int Game::preferredDisplayWidth() const
    {
        return 1024;
    }

    int Game::preferredDisplayHeight() const
    {
        return 768;
    }

    int Game::preferredDepthBufferBits() const
    {
        return 16;
    }

    int Game::preferredStencilBufferBits() const
    {
        return 8;
    }

    void Game::onPointerPressed(int id, const glm::vec2& pos)
    {
        (void)id;
        (void)pos;
    }

    void Game::onPointerMoved(int id, const glm::vec2& pos)
    {
        (void)id;
        (void)pos;
    }

    void Game::onPointerReleased(int id, const glm::vec2& pos)
    {
        (void)id;
        (void)pos;
    }

    void Game::onPointerCancelled(int id, const glm::vec2& pos)
    {
        (void)id;
        (void)pos;
    }

    void Game::setAssetsLocationHint(const char* hint)
    {
        m_AssetsLocationHint = hint;
    }

    const char* Game::assetsLocationHint() const
    {
        return m_AssetsLocationHint;
    }
}
