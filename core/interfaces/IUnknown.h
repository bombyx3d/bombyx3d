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

#pragma once
#include "core/utility/macros.h"
#include "core/utility/debug.h"
#include "core/utility/TypeID.h"
#include <atomic>
#include <string>
#include <cstdint>

namespace Engine
{
    /** Base interface for all engine interfaces. */
    class IUnknown
    {
    public:
        /** @cond */
        IUnknown();
        virtual ~IUnknown();
        /** @endcond */

        /** Increments reference counter for this object. */
        void addRef() const;

        /**
         * Decrements reference counter for this object.
         * When reference counter reaches zero, the object is deleted.
         */
        void releaseRef() const;

        /**
         * Queries a pointer to the specified interface.
         * @param typeID Type ID for the interface.
         * @return Pointer to the specified interface
         * or `nullptr` if this object does not implement the requested interface.
         */
        virtual void* queryInterface(TypeID typeID);

        /**
         * Queries a pointer to the specified interface.
         * @tparam TYPE Data type of the interface.
         * @return Pointer to the specified interface
         * or `nullptr` if this object does not implement the requested interface.
         */
        template <class TYPE> TYPE* queryInterface()
        {
            return reinterpret_cast<TYPE*>(queryInterface(typeOf<TYPE>()));
        }

        /** @cond */
        void* operator new(size_t size);
        void operator delete(void* ptr, size_t size);
        /** @endcond */

    private:
        mutable volatile std::atomic<int> m_ReferenceCount;
      #if Z_ASSERTIONS_ENABLED
        int m_InitialReferenceCount;
      #endif

        Z_DISABLE_COPY(IUnknown)
    };
}