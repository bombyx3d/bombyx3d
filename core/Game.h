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
#include "core/interfaces/IGame.h"
#include "core/interfaces/render/IViewport.h"
#include "core/interfaces/render/IViewportConfiguration.h"
#include "core/interfaces/render/IViewportDelegate.h"

namespace Engine
{
    /** Base class for games. */
    class Game : public IGame
    {
    public:
        Z_IMPLEMENTATION(Game)

        /** Constructor. */
        Game();

        /** Destructor. */
        ~Game();

        /**
         * Retrieves an instance of the viewport.
         * @return Viewport.
         */
        IViewport* viewport() const { return m_Viewport; }

    protected:
        /**
         * Creates a viewport.
         * @param configuration Viewport configuration.
         * @param delegate Viewport delegate.
         */
        virtual Ptr<IViewport> createViewport(IViewportConfiguration* configuration, IViewportDelegate* delegate);

        /** Called immediately after viewport has been created. */
        virtual void onGameDidStart() {}

        /** Called immediately before viewport will be destroyed. */
        virtual void onGameWillExit() {}

        /** Called immediately before game will be suspended. */
        virtual void onGameWillSuspend() {}

        /** Called immediately after game has been resumed. */
        virtual void onGameDidResume() {}

    private:
        class Delegate;

        Ptr<IViewport> m_Viewport;
        Ptr<Delegate> m_Delegate;

        void initialize() final override;
    };
}
