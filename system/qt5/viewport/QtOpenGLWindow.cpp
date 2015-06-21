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
#include "QtOpenGLWindow.h"
#include "core/utility/debug.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QOpenGLContext>
#include <QApplication>

namespace Engine
{
    QtOpenGLWindow::QtOpenGLWindow(const ViewportSettings& viewportSettings, const Ptr<IViewportDelegate>& delegate)
        : m_Delegate(delegate)
        , m_RenderThread(this, m_Delegate)
    {
        std::atomic_store_explicit(&m_ViewportWidth, 0, std::memory_order_seq_cst);
        std::atomic_store_explicit(&m_ViewportHeight, 0, std::memory_order_seq_cst);

        QRect desktopGeometry = QDesktopWidget().availableGeometry();
        int windowWidth = viewportSettings.desiredWidth();
        int windowHeight = viewportSettings.desiredHeight();
        bool windowWidthValid = (windowWidth > 0 && windowWidth <= desktopGeometry.width());
        bool windowHeightValid = (windowHeight > 0 && windowHeight <= desktopGeometry.height());
        if (viewportSettings.fullScreen() || !windowWidthValid || !windowHeightValid) {
            windowWidth = desktopGeometry.width();
            windowHeight = desktopGeometry.height();
        }
        resize(windowWidth, windowHeight);

        if (!parent()) {
            QRect position = frameGeometry();
            position.moveCenter(desktopGeometry.center());
            move(position.topLeft());
        }

        QGLFormat openGLFormat;
        openGLFormat.setDoubleBuffer(true);
        openGLFormat.setSwapInterval(1);

        if (viewportSettings.desiredColorBits() >= 32) {
            openGLFormat.setRedBufferSize(8);
            openGLFormat.setGreenBufferSize(8);
            openGLFormat.setBlueBufferSize(8);
            openGLFormat.setAlphaBufferSize(8);
        } else if (viewportSettings.desiredColorBits() >= 24) {
            openGLFormat.setRedBufferSize(8);
            openGLFormat.setGreenBufferSize(8);
            openGLFormat.setBlueBufferSize(8);
            openGLFormat.setAlphaBufferSize(0);
        } else if (viewportSettings.desiredColorBits() >= 16) {
            openGLFormat.setRedBufferSize(5);
            openGLFormat.setGreenBufferSize(6);
            openGLFormat.setBlueBufferSize(5);
            openGLFormat.setAlphaBufferSize(0);
        } else if (viewportSettings.desiredColorBits() >= 15) {
            openGLFormat.setRedBufferSize(5);
            openGLFormat.setGreenBufferSize(5);
            openGLFormat.setBlueBufferSize(5);
            openGLFormat.setAlphaBufferSize(0);
        }

        int depthBits = viewportSettings.desiredDepthBits();
        if (depthBits == 0)
            openGLFormat.setDepth(false);
        else
            openGLFormat.setDepthBufferSize(depthBits);

        int stencilBits = viewportSettings.desiredStencilBits();
        if (stencilBits == 0)
            openGLFormat.setStencil(false);
        else
            openGLFormat.setStencilBufferSize(stencilBits);

        setFormat(openGLFormat);
    }

    QtOpenGLWindow::~QtOpenGLWindow()
    {
        m_RenderThread.postShutdown();
        m_RenderThread.wait();
    }

    int QtOpenGLWindow::viewportWidth() const
    {
        return std::atomic_load_explicit(&m_ViewportWidth, std::memory_order_relaxed);
    }

    int QtOpenGLWindow::viewportHeight() const
    {
        return std::atomic_load_explicit(&m_ViewportHeight, std::memory_order_relaxed);
    }

    void QtOpenGLWindow::resizeEvent(QResizeEvent* resizeEvent)
    {
        if (m_Initialized) {
            int width = resizeEvent->size().width();
            int height = resizeEvent->size().height();
            std::atomic_store_explicit(&m_ViewportWidth, 0, std::memory_order_seq_cst);
            std::atomic_store_explicit(&m_ViewportHeight, 0, std::memory_order_seq_cst);
            m_RenderThread.resize(width, height);
        }
    }

    void QtOpenGLWindow::paintEvent(QPaintEvent*)
    {
        if (!m_Initialized && !m_InitializationFailed) {
            if (!m_RenderThread.start(width(), height())) {
                m_InitializationFailed = true;
                return;
            }
            m_Initialized = true;
        }
    }

    void QtOpenGLWindow::showEvent(QShowEvent*)
    {
        if (m_Initialized)
            m_RenderThread.resume();
    }

    void QtOpenGLWindow::hideEvent(QHideEvent*)
    {
        if (m_Initialized)
            m_RenderThread.suspend();
    }

    void QtOpenGLWindow::closeEvent(QCloseEvent*)
    {
        if (m_Initialized) {
            m_RenderThread.postShutdown();
            m_RenderThread.wait();
        }
    }

    void QtOpenGLWindow::mousePressEvent(QMouseEvent* e)
    {
        if (m_Initialized && e->button() == Qt::LeftButton) {
            auto delegate = m_Delegate;
            float x = e->pos().x();
            float y = e->pos().y();
            m_RenderThread.post([delegate, x, y]() {
                delegate->onPointerPressed(0, x, y);
            });
        }
    }

    void QtOpenGLWindow::mouseMoveEvent(QMouseEvent* e)
    {
        if (m_Initialized) {
            auto delegate = m_Delegate;
            float x = e->pos().x();
            float y = e->pos().y();
            m_RenderThread.post([delegate, x, y]() {
                delegate->onPointerMoved(0, x, y);
            });
        }
    }

    void QtOpenGLWindow::mouseReleaseEvent(QMouseEvent* e)
    {
        if (m_Initialized && e->button() == Qt::LeftButton) {
            auto delegate = m_Delegate;
            float x = e->pos().x();
            float y = e->pos().y();
            m_RenderThread.post([delegate, x, y]() {
                delegate->onPointerReleased(0, x, y);
            });
        }
    }

    void QtOpenGLWindow::keyPressEvent(QKeyEvent* keyEvent)
    {
        if (m_Initialized) {
            auto delegate = m_Delegate;
            int key = keyEvent->key();
            m_RenderThread.post([delegate, key]() {
                //FIXME
                //delegate->onKeyPressed(key);
            });
        }
    }

    void QtOpenGLWindow::keyReleaseEvent(QKeyEvent* keyEvent)
    {
        if (m_Initialized) {
            auto delegate = m_Delegate;
            int key = keyEvent->key();
            m_RenderThread.post([delegate, key]() {
                //FIXME
                //delegate->onKeyReleased(key);
            });
        }
    }
}
