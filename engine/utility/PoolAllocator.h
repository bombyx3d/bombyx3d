
#pragma once
#include "engine/core/macros.h"
#include "engine/utility/MemoryPool.h"
#include <vector>

namespace Engine
{
    template <class TYPE> class PoolAllocator
    {
    public:
        explicit PoolAllocator(size_t arenaSize = MemoryPool::DEFAULT_ARENA_SIZE)
            : m_MemoryPool(arenaSize)
        {
        }

        ~PoolAllocator()
        {
            for (TYPE* item : m_FreeList)
                item->~TYPE();
        }

        TYPE* alloc()
        {
            if (!m_FreeList.empty()) {
                TYPE* value = m_FreeList.back();
                m_FreeList.pop_back();
                return value;
            }
            return reinterpret_cast<TYPE*>(m_MemoryPool.alloc(sizeof(TYPE)));
        }

        void free(TYPE* value)
        {
            m_FreeList.push_back(value);
        }

    private:
        MemoryPool m_MemoryPool;
        std::vector<TYPE*> m_FreeList;

        Z_DISABLE_COPY(PoolAllocator);
    };
}
