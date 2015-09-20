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
#include "WorkerThread.h"
#include <cassert>

namespace Engine
{
    WorkerThread::WorkerThread()
        : mShouldExit(false)
        , mExited(false)
    {
        mThread = std::thread(std::bind(&WorkerThread::thread, this));
    }

    WorkerThread::~WorkerThread()
    {
        stop();
        mThread.join();
    }

    void WorkerThread::stop()
    {
        perform([this](){ mShouldExit = true; });
    }

    void WorkerThread::perform(const std::function<void()>& action)
    {
        assert(action != nullptr);
        mQueue.enqueue(action);
    }

    void WorkerThread::perform(std::function<void()>&& action)
    {
        assert(action != nullptr);
        mQueue.enqueue(std::move(action));
    }

    void WorkerThread::thread()
    {
        while (!mShouldExit)
            mQueue.dequeue()();
        mExited.store(true);
    }
}
