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
#include "QtOpenGLViewport.h"
#include "core/utility/debug.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QOpenGLContext>
#include <QApplication>

namespace Engine
{
    QtOpenGLViewport::QtOpenGLViewport(const Ptr<IRenderer>& r, IViewportConfiguration* conf, IViewportDelegate* d)
        : m_Delegate(d)
        , m_Renderer(r)
        , m_RenderThread(this, m_Delegate)
    {
        std::atomic_store_explicit(&m_ViewportWidth, 0, std::memory_order_seq_cst);
        std::atomic_store_explicit(&m_ViewportHeight, 0, std::memory_order_seq_cst);

        QRect desktopGeometry = QDesktopWidget().availableGeometry();
        int windowWidth = conf->desiredWidth();
        int windowHeight = conf->desiredHeight();
        bool windowWidthValid = (windowWidth > 0 && windowWidth <= desktopGeometry.width());
        bool windowHeightValid = (windowHeight > 0 && windowHeight <= desktopGeometry.height());
        if (conf->desiredFullScreen() || !windowWidthValid || !windowHeightValid) {
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

        if (conf->desiredColorBits() >= 32) {
            openGLFormat.setRedBufferSize(8);
            openGLFormat.setGreenBufferSize(8);
            openGLFormat.setBlueBufferSize(8);
            openGLFormat.setAlphaBufferSize(8);
        } else if (conf->desiredColorBits() >= 24) {
            openGLFormat.setRedBufferSize(8);
            openGLFormat.setGreenBufferSize(8);
            openGLFormat.setBlueBufferSize(8);
            openGLFormat.setAlphaBufferSize(0);
        } else if (conf->desiredColorBits() >= 16) {
            openGLFormat.setRedBufferSize(5);
            openGLFormat.setGreenBufferSize(6);
            openGLFormat.setBlueBufferSize(5);
            openGLFormat.setAlphaBufferSize(0);
        } else if (conf->desiredColorBits() >= 15) {
            openGLFormat.setRedBufferSize(5);
            openGLFormat.setGreenBufferSize(5);
            openGLFormat.setBlueBufferSize(5);
            openGLFormat.setAlphaBufferSize(0);
        }

        int depthBits = conf->desiredDepthBits();
        if (depthBits == 0)
            openGLFormat.setDepth(false);
        else
            openGLFormat.setDepthBufferSize(depthBits);

        int stencilBits = conf->desiredStencilBits();
        if (stencilBits == 0)
            openGLFormat.setStencil(false);
        else
            openGLFormat.setStencilBufferSize(stencilBits);

        setFormat(openGLFormat);
    }

    QtOpenGLViewport::~QtOpenGLViewport()
    {
        m_RenderThread.postShutdown();
        m_RenderThread.wait();
    }

    int QtOpenGLViewport::viewportWidth() const
    {
        return std::atomic_load_explicit(&m_ViewportWidth, std::memory_order_relaxed);
    }

    int QtOpenGLViewport::viewportHeight() const
    {
        return std::atomic_load_explicit(&m_ViewportHeight, std::memory_order_relaxed);
    }

    IRenderer& QtOpenGLViewport::renderer()
    {
        return *m_Renderer;
    }

    void QtOpenGLViewport::resizeEvent(QResizeEvent* e)
    {
        if (m_Initialized) {
            int newWidth = e->size().width();
            int newHeight = e->size().height();
            std::atomic_store_explicit(&m_ViewportWidth, newWidth, std::memory_order_seq_cst);
            std::atomic_store_explicit(&m_ViewportHeight, newHeight, std::memory_order_seq_cst);
            m_RenderThread.resize(newWidth, newHeight);
        }
    }

    void QtOpenGLViewport::paintEvent(QPaintEvent*)
    {
        if (!m_Initialized && !m_InitializationFailed) {
            if (!m_RenderThread.start(width(), height())) {
                m_InitializationFailed = true;
                return;
            }
            m_Initialized = true;
        }
    }

    void QtOpenGLViewport::showEvent(QShowEvent*)
    {
        if (m_Initialized)
            m_RenderThread.resume();
    }

    void QtOpenGLViewport::hideEvent(QHideEvent*)
    {
        if (m_Initialized)
            m_RenderThread.suspend();
    }

    void QtOpenGLViewport::closeEvent(QCloseEvent*)
    {
        if (m_Initialized) {
            m_RenderThread.postShutdown();
            m_RenderThread.wait();
        }
    }

    void QtOpenGLViewport::mousePressEvent(QMouseEvent* e)
    {
        if (m_Initialized && e->button() == Qt::LeftButton) {
            auto delegateInstance = m_Delegate;
            float mouseX = float(e->pos().x());
            float mouseY = float(e->pos().y());
            m_RenderThread.post([delegateInstance, mouseX, mouseY]() {
                delegateInstance->onPointerPressed(0, mouseX, mouseY);
            });
        }
    }

    void QtOpenGLViewport::mouseMoveEvent(QMouseEvent* e)
    {
        if (m_Initialized) {
            auto delegateInstance = m_Delegate;
            float mouseX = float(e->pos().x());
            float mouseY = float(e->pos().y());
            m_RenderThread.post([delegateInstance, mouseX, mouseY]() {
                delegateInstance->onPointerMoved(0, mouseX, mouseY);
            });
        }
    }

    void QtOpenGLViewport::mouseReleaseEvent(QMouseEvent* e)
    {
        if (m_Initialized && e->button() == Qt::LeftButton) {
            auto delegateInstance = m_Delegate;
            float mouseX = float(e->pos().x());
            float mouseY = float(e->pos().y());
            m_RenderThread.post([delegateInstance, mouseX, mouseY]() {
                delegateInstance->onPointerReleased(0, mouseX, mouseY);
            });
        }
    }

    void QtOpenGLViewport::keyPressEvent(QKeyEvent*)
    {
        if (m_Initialized) {
            /*
            auto delegateInstance = m_Delegate;
            int key = e->key();
            m_RenderThread.post([delegateInstance, key]() {
                //FIXME
                //delegateInstance->onKeyPressed(key);
            });
            */
        }
    }

    void QtOpenGLViewport::keyReleaseEvent(QKeyEvent*)
    {
        if (m_Initialized) {
            /*
            auto delegateInstance = m_Delegate;
            int key = e->key();
            m_RenderThread.post([delegateInstance, key]() {
                //FIXME
                //delegateInstance->onKeyReleased(key);
            });
            */
        }
    }
}
