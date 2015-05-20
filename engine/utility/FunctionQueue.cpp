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
#include "FunctionQueue.h"
#include <new>

namespace Z
{
    void FunctionQueue::clear()
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Queue.clear();
    }

    void FunctionQueue::post(const std::function<void()>& function)
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Queue.push_back(function);
    }

    void FunctionQueue::post(std::function<void()>&& function)
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Queue.emplace_back(std::move(function));
    }

    bool FunctionQueue::processOne()
    {
        void* buf[(sizeof(std::function<void()>) + sizeof(void*) - 1) / sizeof(void*)];
        std::function<void()>* func;

        {
            std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
            if (m_Queue.empty())
                return false;
            func = new(buf) std::function<void()>(std::move(m_Queue.front()));
            m_Queue.pop_front();
        }

        (*func)();

        return true;
    }

    void FunctionQueue::processAll()
    {
        typedef decltype(m_Queue) queue_t;

        void* buf[(sizeof(queue_t) + sizeof(void*) - 1) / sizeof(void*)];
        queue_t* queue;

        {
            std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);

            if (m_Queue.empty())
                return;

            queue = new(buf) queue_t;
            queue_t q(std::move(m_Queue));
            m_Queue = std::move(*queue);
            *queue = std::move(q);
        }

        for (const auto& func : *queue)
            func();
    }
}
