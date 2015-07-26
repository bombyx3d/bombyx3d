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
#include "core/interfaces/IUnknown.h"

namespace Engine
{
    /** Viewport delegate. */
    class IViewportDelegate : public IUnknown
    {
    public:
        Z_INTERFACE(IViewportDelegate)

        /**
         * Called immediately after viewport has been created.
         * @param width Width of the viewport.
         * @param height Height of the viewport.
         * @return `true` on success or `false` on failure.
         * In the latter case no other members of the delegate will ever be called.
         */
        virtual bool onViewportCreated(int width, int height) { (void)width; (void)height; return true; }

        /** Called just before viewport will be destroyed. */
        virtual void onViewportWillClose() {}

        /** Called just before viewport will be suspended. */
        virtual void onViewportWillSuspend() {}

        /** Called immediately after viewport has been restored. */
        virtual void onViewportDidResume() {}

        /**
         * Called when user begins a touch event inside the viewport.
         * @param id Finger number. Fingers are counted from 0.
         * @param x X coordinate of the finger.
         * @param y Y coordinate of the finger.
         */
        virtual void onPointerPressed(int id, float x, float y) { (void)id; (void)x; (void)y; }

        /**
         * Called when user moves a finger inside the viewport.
         * @param id Finger number. Fingers are counted from 0.
         * @param x X coordinate of the finger.
         * @param y Y coordinate of the finger.
         */
        virtual void onPointerMoved(int id, float x, float y) { (void)id; (void)x; (void)y; }

        /**
         * Called when user ends a touch event inside the viewport.
         * @param id Finger number. Fingers are counted from 0.
         * @param x X coordinate of the finger.
         * @param y Y coordinate of the finger.
         */
        virtual void onPointerReleased(int id, float x, float y) { (void)id; (void)x; (void)y; }

        /**
         * Called when touch event inside the viewport is cancelled.
         * @param id Finger number. Fingers are counted from 0.
         * @param x X coordinate of the finger.
         * @param y Y coordinate of the finger.
         */
        virtual void onPointerCancelled(int id, float x, float y) { (void)id; (void)x; (void)y; }

        /**
         * Called when size of the viewport changes.
         * @param width New width of the viewport.
         * @param height New height of the viewport.
         */
        virtual void onViewportDidResize(int width, int height) { (void)width; (void)height; }

        /**
         * Called each frame to update and render contents of the viewport.
         * @param frameTime Number of seconds passed from the previous frame.
         */
        virtual void onViewportShouldRender(double frameTime) { (void)frameTime; }
    };
}
