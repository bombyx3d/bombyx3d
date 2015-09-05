
#pragma once
#include "engine/core/macros.h"
#include "engine/utility/ProducerConsumerQueue.h"
#include <thread>

namespace Engine
{
    class WorkerThread
    {
    public:
        WorkerThread();
        ~WorkerThread();

        void stop();

        void perform(const std::function<void()>& action);
        void perform(std::function<void()>&& action);

    private:
        std::thread mThread;
        ProducerConsumerQueue<std::function<void()>> mQueue;
        bool mShouldExit;

        void thread();

        Z_DISABLE_COPY(WorkerThread);
    };
}
