﻿/*
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
        (void)width;
        (void)height;
        return m_Game->initialize();
    }

    void Engine::onShutdown()
    {
        m_Game->shutdown();
    }

    void Engine::onSuspend()
    {
    }

    void Engine::onResume()
    {
    }

    void Engine::onViewportSizeChanged(int width, int height)
    {
        (void)width;
        (void)height;
    }

    void Engine::onPaintEvent(double time)
    {
        m_Game->runFrame(time);
    }
}