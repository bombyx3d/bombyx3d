
#pragma once
#include "engine/interfaces/core/IThreadManager.h"
#include "engine/utility/WorkerThread.h"
#include "engine/utility/ProducerConsumerQueue.h"

namespace Engine
{
    class CxxThreadManager : public IThreadManager
    {
    public:
        CxxThreadManager();
        ~CxxThreadManager();

        void flushRenderThreadQueue();

        void performInRenderThread(const std::function<void()>& action) override;
        void performInRenderThread(std::function<void()>&& action) override;

        void performInBackgroundThread(const std::function<void()>& action) override;
        void performInBackgroundThread(std::function<void()>&& action) override;

    private:
        ProducerConsumerQueue<std::function<void()>> mRenderThreadQueue;
        WorkerThread mWorkerThread;

        Z_DISABLE_COPY(CxxThreadManager);
    };
}
