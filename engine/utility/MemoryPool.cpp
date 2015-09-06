#include "MemoryPool.h"
#include <cstdlib>
#include <algorithm>
#include <utility>

namespace Engine
{
    MemoryPool::MemoryPool(size_t arenaSize)
        : mArenas(nullptr)
        , mArenaSize(arenaSize)
    {
    }

    MemoryPool::~MemoryPool()
    {
        Arena* arenas = mArenas;
        while (arenas) {
            Arena* arena = arenas;
            arenas = arena->next;
            free(arena);
        }
    }

    void* MemoryPool::alloc(size_t size)
    {
        Arena* arena = nullptr;

        if (size < mArenaSize) {
            arena = mArenas;
            while (arena && arena->bytesLeft < size)
                arena = arena->next;
        }

        size_t arenaSize = mArenaSize;
        if (!arena) {
            arenaSize = std::max(size, arenaSize);
            arena = newArena(arenaSize);
            if (!arena)
                return nullptr;
        }

        void* ptr = reinterpret_cast<uint8_t*>(arena + 1) + (arenaSize - arena->bytesLeft);
        arena->bytesLeft -= size;

        return ptr;
    }

    MemoryPool::Arena* MemoryPool::newArena(size_t size)
    {
        Arena* arena = reinterpret_cast<Arena*>(malloc(sizeof(Arena) + size));
        if (!arena)
            return nullptr;
        arena->next = mArenas;
        arena->bytesLeft = size;
        mArenas = arena;
        return arena;
    }
}
