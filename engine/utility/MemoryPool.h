
#pragma once
#include "engine/core/macros.h"
#include <memory>
#include <vector>

namespace Engine
{
    class MemoryPool
    {
    public:
        static const size_t DEFAULT_ARENA_SIZE = 65536;

        explicit MemoryPool(size_t arenaSize = DEFAULT_ARENA_SIZE);
        ~MemoryPool();

        void* alloc(size_t size);

    private:
        struct Arena
        {
            Arena* next;
            size_t bytesLeft;
        };

        Arena* m_Arenas;
        size_t m_ArenaSize;

        Arena* newArena(size_t size);

        Z_DISABLE_COPY(MemoryPool);
    };
}
