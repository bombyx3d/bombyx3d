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
#include "core/interfaces/IViewport.h"
#include "core/interfaces/IViewportDelegate.h"
#include "core/utility/Ptr.h"

namespace Engine
{
    /** Interface to the operating system API. */
    class ISystem : public IUnknown
    {
    public:
        Z_SINGLETON_INTERFACE(ISystem)

      #ifdef DOXYGEN
        /**
         * Retrieves reference to the instance of the operating system API wrapper.
         * @return Reference to the instance of the operating system API wrapper.
         */
        static ISystem& instance();
      #endif

        /**
         * Creates new viewport.
         * @param viewportSettings Desired settings of the viewport.
         * @note Settings passed to this method are hints. Engine will do its best to enforce provided
         * values, but operating system has the power to override any of them.
         * @param delegate Viewport delegate.
         * @return Pointer to the viewport or `nullptr` if error has occured.
         */
        virtual Ptr<IViewport> createViewport(const ViewportSettings& viewportSettings,
            const Ptr<IViewportDelegate>& delegate) = 0;

        /** Runs the event loop. */
        virtual void runEventLoop() = 0;
    };
}
