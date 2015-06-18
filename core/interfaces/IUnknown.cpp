/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include "IUnknown.h"
#include <cstring>

namespace Engine
{
    namespace
    {
        //
        // Maintain a little per-thread stack of IUnknown instances constructed with operator new.
        //
        // In IUnknown constructor we initialize reference counter to 0 if object was created with
        // operator new or to 1 if object was created any other way.
        //
        // This allows user code to create objects both on stack and on heap and pass them
        // through the code that increments/decrements reference counter.
        //

        struct AllocatedObject
        {
            const void* startAddress;
            size_t size;
        };

        struct AllocatedObjects
        {
            static const size_t SIZE = 8;

            AllocatedObject objects[SIZE];
            size_t count;

            void push(void* address, size_t size)
            {
                size_t n = count;

                Z_ASSERT(n < SIZE);
                if (n >= SIZE)
                    abort();

                objects[n].startAddress = address;
                objects[n].size = size;
                count = n + 1;
            }

            bool tryPop(const void* address)
            {
                size_t n = count;
                if (n == 0)
                    return false;

                size_t last = n - 1;
                const uint8_t* addr = reinterpret_cast<const uint8_t*>(address);
                const uint8_t* start = reinterpret_cast<const uint8_t*>(objects[last].startAddress);
                const uint8_t* end = start + objects[last].size;

                if (addr >= start && addr < end) {
                    count = n - 1;
                    return true;
                }

                return false;
            }
        };
    }

    static Z_THREADLOCAL AllocatedObjects g_AllocationStack;


    IUnknown::IUnknown()
    {
        int initialRefCount = 0;

        if (!g_AllocationStack.tryPop(this))
            initialRefCount = 1;

      #if Z_ASSERTIONS_ENABLED
        m_InitialReferenceCount = initialRefCount;
      #endif

        std::atomic_store_explicit(&m_ReferenceCount, initialRefCount, std::memory_order_relaxed);
    }

    IUnknown::~IUnknown()
    {
        Z_ASSERT(m_ReferenceCount == m_InitialReferenceCount);
    }

    void IUnknown::addRef() const
    {
        //
        // From: http://www.boost.org/doc/libs/1_55_0/doc/html/atomic/usage_examples.html
        //
        // Increasing the reference counter can always be done with memory_order_relaxed:
        // New references to an object can only be formed from an existing reference, and
        // passing an existing reference from one thread to another must already provide
        // any required synchronization.
        //

        std::atomic_fetch_add_explicit(&m_ReferenceCount, 1, std::memory_order_relaxed);
    }

    void IUnknown::releaseRef() const
    {
        //
        // From: http://www.boost.org/doc/libs/1_55_0/doc/html/atomic/usage_examples.html
        //
        // It is important to enforce any possible access to the object in one thread
        // (through an existing reference) to happen before deleting the object in a
        // different thread. This is achieved by a "release" operation after dropping a
        // reference (any access to the object through this reference must obviously
        // happened before), and an "acquire" operation before deleting the object.
        //
        // It would be possible to use memory_order_acq_rel for the fetch_sub operation,
        // but this results in unneeded "acquire" operations when the reference counter
        // does not yet reach zero and may impose a performance penalty.
        //

        auto wasRefCount = std::atomic_fetch_sub_explicit(&m_ReferenceCount, 1, std::memory_order_release);
        Z_ASSERT(wasRefCount > m_InitialReferenceCount);
        if (wasRefCount <= 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete this;
        }
    }

    void* IUnknown::operator new(size_t size)
    {
        void* ptr = new void*[(size + sizeof(void*) - 1) / sizeof(void*)];
        if (!ptr)
            return nullptr;

        g_AllocationStack.push(ptr, size);

      #if Z_ASSERTIONS_ENABLED
        memset(ptr, 0xCC, size);
      #endif

        return ptr;
    }

    void IUnknown::operator delete(void* ptr, size_t size)
    {
        g_AllocationStack.tryPop(ptr);

      #if Z_ASSERTIONS_ENABLED
        memset(ptr, 0xCC, size);
      #endif

        delete[] reinterpret_cast<void**>(ptr);
    }
}
