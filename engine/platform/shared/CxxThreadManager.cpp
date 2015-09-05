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

    void CxxThreadManager::flushRenderThreadQueue()
    {
        std::function<void()> action;
        while (mRenderThreadQueue.tryDequeue(action))
            action();
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
