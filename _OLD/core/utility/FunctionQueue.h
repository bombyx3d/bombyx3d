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
#include "core/utility/macros.h"
#include <deque>
#include <mutex>
#include <functional>

namespace Engine
{
    /** A FIFO queue of `std::function` objects. */
    class FunctionQueue
    {
    public:
        /** Constructor. */
        FunctionQueue() = default;

        /** Destructor. */
        ~FunctionQueue() = default;

        /** Clears the queue. */
        void clear();

        /**
         * Enqueues function to the tail of the queue.
         * @param function Function to enqueue.
         */
        void post(const std::function<void()>& function);

        /**
         * Enqueues function to the tail of the queue.
         * @param function Function to enqueue.
         */
        void post(std::function<void()>&& function);

        /**
         * Dequeues and invokes one function from the head of the queue.
         * @return `true` if function has been dequeued and invoked or `false` if queue was empty.
         */
        bool processOne();

        /**
         * Dequeues and invokes all queued functions, one by one, in the order they were enqueued into this queue.
         * If queue is empty, this method does nothing.
         */
        void processAll();

    private:
        std::mutex m_Mutex;                             /**< Mutex. */
        std::deque<std::function<void()>> m_Queue;      /**< Queue. */

        Z_DISABLE_COPY(FunctionQueue)
    };
}
