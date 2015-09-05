#include "ProducerConsumerQueue.h"

namespace Engine
{
    namespace Internal
    {
        ProducerConsumerQueueBase::ProducerConsumerQueueBase()
        {
        }

        ProducerConsumerQueueBase::~ProducerConsumerQueueBase()
        {
        }

        void ProducerConsumerQueueBase::wait(UniqueLock& lock)
        {
            while (shouldWait())
                mCondition.wait(lock);
        }

        void ProducerConsumerQueueBase::notify()
        {
            mCondition.notify_one();
        }
    }
}
