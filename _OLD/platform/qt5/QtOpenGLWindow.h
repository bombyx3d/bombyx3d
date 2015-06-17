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
#include <QGLWidget>

namespace Z
{
    class QtOpenGLWindow : public QGLWidget
    {
        Q_OBJECT

    public:
        explicit QtOpenGLWindow(QWidget* parent = nullptr);
        ~QtOpenGLWindow();

    protected:
        void resizeEvent(QResizeEvent* resizeEvent) override;
        void paintEvent(QPaintEvent* paintEvent) override;

        void showEvent(QShowEvent* showEvent) override;
        void hideEvent(QHideEvent* hideEvent) override;
        void closeEvent(QCloseEvent* closeEvent) override;

        void mousePressEvent(QMouseEvent* mouseEvent) override;
        void mouseMoveEvent(QMouseEvent* mouseEvent) override;
        void mouseReleaseEvent(QMouseEvent* mouseEvent) override;

        void keyPressEvent(QKeyEvent* keyEvent) override;
        void keyReleaseEvent(QKeyEvent* keyEvent) override;

    private:
        QtOpenGLRenderThread m_RenderThread;
        bool m_Initialized = false;
        bool m_InitializationFailed = false;
    };
}
