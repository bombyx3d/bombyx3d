
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
