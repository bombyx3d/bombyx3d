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
#include "core/utility/FunctionQueue.h"
#include "core/utility/Ptr.h"
#include "core/interfaces/IViewportDelegate.h"
#include <atomic>
#include <future>
#include <cstdint>
#include <QElapsedTimer>
#include <QThread>
#include <QGLWidget>

namespace Engine
{
    /** Rendering thread. */
    class QtOpenGLRenderThread : public QThread, protected FunctionQueue
    {
        Q_OBJECT

    public:
        /**
         * Constructor.
         * @param gl Pointer to the corresponding OpenGL widget.
         * @param delegate Pointer to the viewport delegate.
         */
        QtOpenGLRenderThread(QGLWidget* gl, const Ptr<IViewportDelegate>& delegate);

        /** Destructor. */
        ~QtOpenGLRenderThread();

        /**
         * Starts the thread.
         * @param width Width of the viewport.
         * @param height Height of the viewport.
         */
        bool start(int width, int height);

      #ifdef DOXYGEN
        /**
         * Enqueues function to be executed in the context of this thread.
         * @param function Function to enqueue.
         */
        void post(const std::function<void()>& function);

        /**
         * Enqueues function to be executed in the context of this thread.
         * @param function Function to enqueue.
         */
        void post(std::function<void()>&& function);
      #else
        using FunctionQueue::post;
      #endif

        /**
         * Posts the shutdown message to the thread.
         * @note This method does not wait for thread to shutdown, but posts message into the internal queue
         * and immediately returns to the caller.
         */
        void postShutdown();

        /** Suspends the rendering thread. */
        void suspend();

        /** Resumes the rendering thread. */
        void resume();

        /**
         * Posts the viewport resize message to the thread.
         * @param width New width of the viewport.
         * @param height New height of the viewport.
         */
        void resize(int width, int height);

    protected:
        /** @cond */
        void run() override;
        /** @endcond */

    private:
        QGLWidget* m_GL;
        Ptr<IViewportDelegate> m_Delegate;
        QElapsedTimer m_Timer;
        std::atomic<uint32_t> m_ViewportSize;
        std::promise<bool> m_ThreadStartPromise;
        std::atomic<bool> m_Suspended;
        std::atomic<bool> m_ShuttingDown;

      #ifdef DOXYGEN
        void clear();
        bool processOne();
        void processAll();
      #endif
    };
}
