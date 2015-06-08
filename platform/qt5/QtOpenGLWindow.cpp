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
#include "QtFileSystem.h"
#include "platform/PlatformInitOptions.h"
#include "utility/debug.h"
#include <QMessageBox>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QApplication>

namespace Z
{
    QtOpenGLWindow::QtOpenGLWindow(QWidget* parent)
        : QGLWidget(parent)
        , m_RenderThread(this)
    {
        const PlatformInitOptions* initOptions = m_RenderThread.callbacks()->getInitOptions();

        const char* assetsLocation = initOptions->assetsLocationHint();
        if (assetsLocation)
            FileSystem::defaultFileSystem()->add(std::make_shared<QtFileSystem>(assetsLocation));
        FileSystem::defaultFileSystem()->add(std::make_shared<QtFileSystem>(qApp->applicationDirPath()));
        FileSystem::defaultFileSystem()->add(std::make_shared<QtFileSystem>(":/"));

        QRect desktopGeometry = QDesktopWidget().availableGeometry();
        int windowWidth = initOptions->preferredDisplayWidth();
        int windowHeight = initOptions->preferredDisplayHeight();
        if (windowWidth > desktopGeometry.width() || windowHeight > desktopGeometry.height()) {
            windowWidth = desktopGeometry.width();
            windowHeight = desktopGeometry.height();
        }
        resize(windowWidth, windowHeight);

        if (!parent) {
            QRect position = frameGeometry();
            position.moveCenter(desktopGeometry.center());
            move(position.topLeft());
        }

        QGLFormat openGLFormat;
        openGLFormat.setSwapInterval(1);
        openGLFormat.setDoubleBuffer(true);

        int depthBits = initOptions->preferredDepthBufferBits();
        if (depthBits <= 0)
            openGLFormat.setDepth(false);
        else
            openGLFormat.setDepthBufferSize(depthBits);

        int stencilBits = initOptions->preferredStencilBufferBits();
        if (stencilBits <= 0)
            openGLFormat.setStencil(false);
        else
            openGLFormat.setStencilBufferSize(stencilBits);

        setFormat(openGLFormat);
    }

    QtOpenGLWindow::~QtOpenGLWindow()
    {
    }

    void QtOpenGLWindow::resizeEvent(QResizeEvent* resizeEvent)
    {
        if (m_Initialized) {
            int width = resizeEvent->size().width();
            int height = resizeEvent->size().height();
            m_RenderThread.resize(width, height);
        }
    }

    void QtOpenGLWindow::paintEvent(QPaintEvent*)
    {
        if (!m_Initialized && !m_InitializationFailed) {
            if (!m_RenderThread.start(width(), height())) {
                m_InitializationFailed = true;
                QMessageBox::critical(this, tr("Error"), tr("Unable to initialize the game."));
                qApp->exit(1);
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
            float x = e->pos().x();
            float y = e->pos().y();
            m_RenderThread.post([this, x, y]() {
                m_RenderThread.callbacks()->onPointerPressed(0, x, y);
            });
        }
    }

    void QtOpenGLWindow::mouseMoveEvent(QMouseEvent* e)
    {
        if (m_Initialized) {
            float x = e->pos().x();
            float y = e->pos().y();
            m_RenderThread.post([this, x, y]() {
                m_RenderThread.callbacks()->onPointerMoved(0, x, y);
            });
        }
    }

    void QtOpenGLWindow::mouseReleaseEvent(QMouseEvent* e)
    {
        if (m_Initialized && e->button() == Qt::LeftButton) {
            float x = e->pos().x();
            float y = e->pos().y();
            m_RenderThread.post([this, x, y]() {
                m_RenderThread.callbacks()->onPointerReleased(0, x, y);
            });
        }
    }

    void QtOpenGLWindow::keyPressEvent(QKeyEvent* keyEvent)
    {
        if (m_Initialized) {
            int key = keyEvent->key();
            m_RenderThread.post([this, key]() {
                //FIXME
                //m_RenderThread.callbacks()->onKeyPressed(key);
            });
        }
    }

    void QtOpenGLWindow::keyReleaseEvent(QKeyEvent* keyEvent)
    {
        if (m_Initialized) {
            int key = keyEvent->key();
            m_RenderThread.post([this, key]() {
                //FIXME
                //m_RenderThread.callbacks()->onKeyReleased(key);
            });
        }
    }
}
