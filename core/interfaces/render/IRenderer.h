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
#include "core/interfaces/render/IViewport.h"
#include "core/interfaces/render/IViewportConfiguration.h"
#include "core/interfaces/render/IViewportDelegate.h"

namespace Engine
{
    /** Interface to the renderer. */
    class IRenderer : public IUnknown
    {
    public:
        Z_INTERFACE(IRenderer)

        /**
         * Creates a viewport.
         * @param configuration Desired viewport configuration.
         * @param delegate Delegate for the viewport.
         * @return Newly created viewport.
         */
        virtual Ptr<IViewport> createViewport(IViewportConfiguration* configuration, IViewportDelegate* delegate) = 0;

        /**
         * Activates the specified render target.
         * After calling this method all rendering will go into the specified render target.
         * @param target Render target. If set to `nullptr`, the following rendering commands will be discarded
         * until valid render target will be set.
         */
        virtual void setRenderTarget(IRenderTarget* target) = 0;
    };
}
