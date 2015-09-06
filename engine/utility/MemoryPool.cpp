/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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
