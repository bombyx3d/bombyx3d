
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
            : mMemoryPool(arenaSize)
        {
        }

        ~PoolAllocator()
        {
            for (TYPE* item : mFreeList)
                item->~TYPE();
        }

        TYPE* alloc()
        {
            if (!mFreeList.empty()) {
                TYPE* value = mFreeList.back();
                mFreeList.pop_back();
                return value;
            }
            return reinterpret_cast<TYPE*>(mMemoryPool.alloc(sizeof(TYPE)));
        }

        void free(TYPE* value)
        {
            mFreeList.push_back(value);
        }

    private:
        MemoryPool mMemoryPool;
        std::vector<TYPE*> mFreeList;

        Z_DISABLE_COPY(PoolAllocator);
    };
}
