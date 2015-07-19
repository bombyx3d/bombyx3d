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
#include "core/private/Core.h"
#include "core/interfaces/IViewportDelegate.h"
#include "core/utility/debug.h"
#include "renderer/Renderer.h"
#include <memory>
#include <unordered_map>

namespace Z
{
    class Game;

    class Engine : public IViewportDelegate
    {
    public:
        using TouchMap = std::unordered_map<int, glm::vec2>;

        explicit Engine();
        ~Engine();

        static Engine& instance() { Z_ASSERT(m_Instance != nullptr); return *m_Instance; }

        Renderer& renderer() { Z_ASSERT(m_Renderer != nullptr); return *m_Renderer; }

        const TouchMap& activeTouches() const { return m_ActiveTouches; }
        void cancelAllActiveTouches();

    private:
        static Engine* m_Instance;
        std::unique_ptr<Renderer> m_Renderer;
        std::unique_ptr<Game> m_Game;
        TouchMap m_ActiveTouches;

        bool onViewportCreated(int width, int height) final override;
        void onViewportWillClose() final override;
        void onViewportWillSuspend() final override;
        void onViewportDidResume() final override;

        void onPointerPressed(int id, float x, float y) final override;
        void onPointerMoved(int id, float x, float y) final override;
        void onPointerReleased(int id, float x, float y) final override;
        void onPointerCancelled(int id, float x, float y) final override;

        void onViewportDidResize(int width, int height) final override;
        void onViewportShouldRender(double time) final override;
    };
}
