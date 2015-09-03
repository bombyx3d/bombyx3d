
#pragma once
#include "engine/core/macros.h"
#include <utility>
#include <memory>

namespace Engine
{
    template <class INTERFACE> class Singleton
    {
    public:
        static const std::shared_ptr<INTERFACE>& instance()
        {
            return instanceVariable();
        }

        static void setInstance(const std::shared_ptr<INTERFACE>& newInstance)
        {
            instanceVariable() = newInstance;
        }

        static void setInstance(std::shared_ptr<INTERFACE>&& newInstance)
        {
            instanceVariable() = std::move(newInstance);
        }

        template <class IMPLEMENTATION, typename... ARGS> static void createInstance(ARGS&&... args)
        {
            instanceVariable() = std::make_shared<IMPLEMENTATION>(std::forward<ARGS>(args)...);
        }

        static void destroyInstance()
        {
            instanceVariable().reset();
        }

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    private:
        static std::shared_ptr<INTERFACE>& instanceVariable()
        {
            static std::shared_ptr<INTERFACE> instance;
            return instance;
        }

        Z_DISABLE_COPY(Singleton);
    };
}
