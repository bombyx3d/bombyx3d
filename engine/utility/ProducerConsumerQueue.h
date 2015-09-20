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

#pragma once
#include "engine/core/macros.h"
#include <mutex>
#include <condition_variable>
#include <deque>

namespace Engine
{
    namespace Internal
    {
        class ProducerConsumerQueueBase
        {
        protected:
            std::mutex mMutex;
            std::condition_variable mCondition;

            using LockGuard = std::lock_guard<decltype(mMutex)>;
            using UniqueLock = std::unique_lock<decltype(mMutex)>;

            ProducerConsumerQueueBase();
            virtual ~ProducerConsumerQueueBase();

            virtual bool shouldWait() const = 0;

            void wait(UniqueLock& lock);
            void notify();
        };
    }

    template <typename ITEM> class ProducerConsumerQueue : public Internal::ProducerConsumerQueueBase
    {
    public:
        ProducerConsumerQueue() {}
        ~ProducerConsumerQueue() {}

        void clear()
        {
            UniqueLock lock(mMutex);
            std::deque<ITEM> queue = std::move(mQueue);
            mQueue = std::deque<ITEM>();
            lock.unlock();
            queue.clear();
        }

        void enqueue(const ITEM& item)
        {
            LockGuard lock(mMutex);
            mQueue.emplace_back(item);
            notify();
        }

        void enqueue(ITEM&& item)
        {
            LockGuard lock(mMutex);
            mQueue.emplace_back(std::move(item));
            notify();
        }

        ITEM dequeue()
        {
            UniqueLock lock(mMutex);
            wait(lock);

            ITEM item(std::move(mQueue.front()));
            mQueue.pop_front();
            lock.unlock();

            return item;
        }

        void dequeue(ITEM& item)
        {
            UniqueLock lock(mMutex);
            wait(lock);

            item = std::move(mQueue.front());
            mQueue.pop_front();
        }

        bool tryDequeue(ITEM& item)
        {
            UniqueLock lock(mMutex);

            if (mQueue.empty())
                return false;

            item = std::move(mQueue.front());
            mQueue.pop_front();

            return true;
        }

    private:
        std::deque<ITEM> mQueue;

        bool shouldWait() const override { return mQueue.empty(); }

        Z_DISABLE_COPY(ProducerConsumerQueue);
    };
}
