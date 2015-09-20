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
#include "CxxThreadManager.h"
#include <cassert>

namespace Engine
{
    CxxThreadManager::CxxThreadManager()
    {
    }

    CxxThreadManager::~CxxThreadManager()
    {
    }

    void CxxThreadManager::stopWorkerThreads()
    {
        mWorkerThread.stop();
        while (!mWorkerThread.exited())
            std::this_thread::yield();
        mRenderThreadQueue.clear();
    }

    void CxxThreadManager::flushRenderThreadQueue()
    {
        std::function<void()> action;
        while (mRenderThreadQueue.tryDequeue(action)) {
            action();
            action = nullptr;
        }
    }

    void CxxThreadManager::performInRenderThread(const std::function<void()>& action)
    {
        assert(action != nullptr);
        mRenderThreadQueue.enqueue(action);
    }

    void CxxThreadManager::performInRenderThread(std::function<void()>&& action)
    {
        assert(action != nullptr);
        mRenderThreadQueue.enqueue(std::move(action));
    }

    void CxxThreadManager::performInBackgroundThread(const std::function<void()>& action)
    {
        assert(action != nullptr);
        mWorkerThread.perform(action);
    }

    void CxxThreadManager::performInBackgroundThread(std::function<void()>&& action)
    {
        assert(action != nullptr);
        mWorkerThread.perform(std::move(action));
    }
}
