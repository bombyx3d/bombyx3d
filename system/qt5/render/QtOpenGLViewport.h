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
#include "QtOpenGLRenderThread.h"
#include "core/interfaces/render/IRenderer.h"
#include "core/interfaces/render/IViewport.h"
#include "core/interfaces/render/IViewportConfiguration.h"
#include <QGLWidget>
#include <atomic>

namespace Engine
{
    /** An OpenGL-based implementation of @ref IViewport. */
    class QtOpenGLViewport : public QGLWidget, public IViewport
    {
        Q_OBJECT

    public:
        /**
         * Constructor.
         * @param r Renderer.
         * @param conf Viewport configuration.
         * @param d Viewport delegate.
         */
        QtOpenGLViewport(IRenderer* r, IViewportConfiguration* conf, IViewportDelegate* d);

        /** Destructor. */
        ~QtOpenGLViewport();

        /**
         * Retrieves pointer to the viewport delegate.
         * @return Pointer to the viewport delegate.
         */
        IViewportDelegate* delegate() const
        {
            return m_Delegate;
        }

        int viewportWidth() const override;
        int viewportHeight() const override;
        IRenderer& renderer() override;

    protected:
        /** @cond */
        void resizeEvent(QResizeEvent* e) override;
        void paintEvent(QPaintEvent* e) override;

        void showEvent(QShowEvent* e) override;
        void hideEvent(QHideEvent* e) override;
        void closeEvent(QCloseEvent* e) override;

        void mousePressEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;

        void keyPressEvent(QKeyEvent* e) override;
        void keyReleaseEvent(QKeyEvent* e) override;
        /** @endcond */

    private:
        Ptr<IViewportDelegate> m_Delegate;
        Ptr<IRenderer> m_Renderer;
        QtOpenGLRenderThread m_RenderThread;
        volatile std::atomic<int> m_ViewportWidth;
        volatile std::atomic<int> m_ViewportHeight;
        bool m_Initialized = false;
        bool m_InitializationFailed = false;
    };
}
