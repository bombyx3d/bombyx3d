#include "WorkerThread.h"
#include <cassert>

namespace Engine
{
    WorkerThread::WorkerThread()
        : mThread(std::bind(&WorkerThread::thread, this))
        , mShouldExit(false)
    {
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
    }
}
