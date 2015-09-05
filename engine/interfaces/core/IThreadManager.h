
#pragma once
#include "engine/utility/Singleton.h"
#include <functional>

namespace Engine
{
    class IThreadManager : public Singleton<IThreadManager>
    {
    public:
        virtual ~IThreadManager() = default;

        virtual void performInRenderThread(const std::function<void()>& action) = 0;
        virtual void performInRenderThread(std::function<void()>&& action) = 0;

        virtual void performInBackgroundThread(const std::function<void()>& action) = 0;
        virtual void performInBackgroundThread(std::function<void()>&& action) = 0;
    };
}
