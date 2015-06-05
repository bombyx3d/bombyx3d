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
#include "Engine.h"
#include "Game.h"
#include "utility/debug.h"

namespace Z
{
    Engine* Engine::m_Instance;

    Engine::Engine()
    {
        Z_ASSERT(m_Instance == nullptr);
        m_Instance = this;
        m_Game.reset(Game::create());
    }

    Engine::~Engine()
    {
        Z_ASSERT(m_Instance == this);
        m_Game.reset();
        m_Instance = nullptr;
    }

    PlatformCallbacks* Engine::create()
    {
        return new Engine;
    }

    const PlatformInitOptions* Engine::getInitOptions() const
    {
        return m_Game.get();
    }

    bool Engine::onInitialize(int width, int height)
    {
        m_Renderer.reset(new Renderer(width, height));
        return m_Game->initialize();
    }

    void Engine::onShutdown()
    {
        m_Game->shutdown();
        m_Renderer.reset();
    }

    void Engine::onSuspend()
    {
        if (m_Renderer)
            m_Renderer->suspend();
    }

    void Engine::onResume()
    {
        if (m_Renderer)
            m_Renderer->resume();
    }

    void Engine::onPointerPressed(int id, float x, float y)
    {
    }

    void Engine::onPointerMoved(int id, float x, float y)
    {
    }

    void Engine::onPointerReleased(int id, float x, float y)
    {
    }

    void Engine::onPointerCancelled(int id, float x, float y)
    {
    }

    void Engine::onViewportSizeChanged(int width, int height)
    {
        if (m_Renderer)
            m_Renderer->setViewportSize(width, height);
    }

    void Engine::onPaintEvent(double time)
    {
        if (m_Renderer) {
            m_Renderer->beginFrame();
            m_Game->runFrame(time);
            m_Renderer->endFrame();
        }
    }
}
