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
#include "core/render/ViewportConfiguration.h"

namespace Engine
{
    class Game::Delegate : public IViewportDelegate
    {
    public:
        Game* m_Game;

        Delegate(Game* game)
            : m_Game(game)
        {
        }

        bool onViewportCreated(int, int) final override
        {
            if (m_Game)
                m_Game->onGameDidStart();

            return true;
        }

        void onViewportWillClose() final override
        {
            if (m_Game)
                m_Game->onGameWillExit();
        }

        void onViewportWillSuspend() final override
        {
            if (m_Game)
                m_Game->onGameWillSuspend();
        }

        void onViewportDidResume() final override
        {
            if (m_Game)
                m_Game->onGameDidResume();
        }

        void onPointerPressed(int id, float x, float y) final override
        {
            (void)id;
            (void)x;
            (void)y;
        }

        void onPointerMoved(int id, float x, float y) final override
        {
            (void)id;
            (void)x;
            (void)y;
        }

        void onPointerReleased(int id, float x, float y) final override
        {
            (void)id;
            (void)x;
            (void)y;
        }

        void onPointerCancelled(int id, float x, float y) final override
        {
            (void)id;
            (void)x;
            (void)y;
        }

        void onViewportDidResize(int width, int height) final override
        {
            (void)width;
            (void)height;
        }

        void onViewportShouldRender(double frameTime) final override
        {
            (void)frameTime;
        }
    };


    Game::Game()
    {
    }

    Game::~Game()
    {
        if (m_Delegate)
            m_Delegate->m_Game = nullptr;
    }

    void Game::initialize()
    {
        m_Delegate = new Delegate(this);

        ViewportConfiguration configuration;
        m_Viewport = createViewport(&configuration, m_Delegate);
    }

    Ptr<IViewport> Game::createViewport(IViewportConfiguration* configuration, IViewportDelegate* delegate)
    {
        return Core::system().createViewport(configuration, delegate);
    }
}
