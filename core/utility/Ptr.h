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

#pragma once
#include "core/utility/debug.h"
#include <functional>

namespace Engine
{
    class IUnknown;

    /** @cond */
    namespace Internal
    {
        class IUnknownPtr
        {
        protected:
            IUnknownPtr() = default;
            ~IUnknownPtr() = default;
            static void acquire(IUnknown* object);
            static void release(IUnknown* object);
        };
    }
    /** @endcond */

    /** Smart pointer for @ref Engine::IUnknown descendants. */
    template <class TYPE> class Ptr
      #ifndef DOXYGEN
        : public Internal::IUnknownPtr
      #endif
    {
    public:
        /** Constructs a `null` pointer. */
        Ptr()
            : m_Object(nullptr)
        {
        }

        /** Constructs a `null` pointer. */
        Ptr(const std::nullptr_t&)
            : m_Object(nullptr)
        {
        }

        /**
         * Constructor.
         * @param object Pointer to the object.
         */
        Ptr(TYPE* object)
            : m_Object(object)
        {
            IUnknownPtr::acquire(m_Object);
        }

        /**
         * Copy constructor.
         * @param other Source pointer.
         */
        Ptr(const Ptr& other)
            : m_Object(other.m_Object)
        {
            IUnknownPtr::acquire(m_Object);
        }

        /**
         * Copy constructor.
         * @param other Source pointer.
         */
        template <class OTHER> Ptr(const Ptr<OTHER>& other)
            : m_Object(other.m_Object)
        {
            IUnknownPtr::acquire(m_Object);
        }

        /**
         * Move constructor.
         * @param other Source pointer.
         */
        Ptr(Ptr&& other)
            : m_Object(other.m_Object)
        {
            other.m_Object = nullptr;
        }

        /**
         * Move constructor.
         * @param other Source pointer.
         */
        template <class OTHER> Ptr(Ptr<OTHER>&& other)
            : m_Object(other.m_Object)
        {
            other.m_Object = nullptr;
        }

        /** Destructor. */
        ~Ptr()
        {
            IUnknownPtr::release(m_Object);
        }

        /**
         * Assignment operator.
         * @return This pointer.
         */
        Ptr& operator=(const std::nullptr_t&)
        {
            IUnknownPtr::release(m_Object);
            return *this;
        }

        /**
         * Assignment operator.
         * @param other Source pointer.
         * @return This pointer.
         */
        Ptr& operator=(TYPE* other)
        {
            IUnknownPtr::acquire(other);
            IUnknownPtr::release(m_Object);
            m_Object = other;
            return *this;
        }

        /**
         * Assignment operator.
         * @param other Source pointer.
         * @return This pointer.
         */
        Ptr& operator=(const Ptr& other)
        {
            IUnknownPtr::acquire(other.m_Object);
            IUnknownPtr::release(m_Object);
            m_Object = other.m_Object;
            return *this;
        }

        /**
         * Assignment operator.
         * @param other Source pointer.
         * @return This pointer.
         */
        template <class OTHER> Ptr& operator=(const Ptr<OTHER>& other)
        {
            IUnknownPtr::acquire(other.m_Object);
            IUnknownPtr::release(m_Object);
            m_Object = other.m_Object;
            return *this;
        }

        /**
         * Assignment operator.
         * @param other Source pointer.
         * @return This pointer.
         */
        Ptr& operator=(Ptr&& other)
        {
            Z_ASSERT(&other != this);
            IUnknownPtr::release(m_Object);
            m_Object = other.m_Object;
            other.m_Object = nullptr;
            return *this;
        }

        /**
         * Assignment operator.
         * @param other Source pointer.
         * @return This pointer.
         */
        template <class OTHER> Ptr& operator=(Ptr<OTHER>&& other)
        {
            Z_ASSERT(&other != this);
            IUnknownPtr::release(m_Object);
            m_Object = other.m_Object;
            other.m_Object = nullptr;
            return *this;
        }

        /** Releases the associated object and resets this pointer to `nullptr`. */
        void release()
        {
            IUnknownPtr::release(m_Object);
        }

        /**
         * Checks whether this pointer is `null`.
         * @return `true` if this pointer represents a valid object or `false` if this pointer is `nullptr`.
         */
        explicit operator bool() const
        {
            return m_Object != nullptr;
        }

        /**
         * Checks whether this pointer is `null`.
         * @return `true` if this pointer is `nullptr` or `false` if this pointer represents a valid object.
         */
        bool operator!() const
        {
            return m_Object == nullptr;
        }

        /**
         * Retrieves an associated raw pointer.
         * @return Raw pointer.
         */
        operator TYPE*() const
        {
            return m_Object;
        }

        /**
         * Overloaded operator.
         * @return Pointer to the object.
         */
        TYPE* operator->() const
        {
            return m_Object;
        }

        /**
         * Overloaded operator.
         * @return Reference to the object.
         */
        TYPE& operator*() const
        {
            Z_ASSERT(m_Object != nullptr);
            return *m_Object;
        }

        /**
         * Compares this pointer with `nullptr`.
         * @return Result of comparison.
         */
        bool operator==(const std::nullptr_t&) const
        {
            return m_Object == nullptr;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        bool operator==(const TYPE* other) const
        {
            return m_Object == other;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        bool operator==(const Ptr& other) const
        {
            return m_Object == other.m_Object;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        template <class OTHER> bool operator==(const Ptr<OTHER>& other) const
        {
            return m_Object == other.m_Object;
        }

        /**
         * Compares this pointer with `nullptr`.
         * @return Result of comparison.
         */
        bool operator!=(const std::nullptr_t&) const
        {
            return m_Object != nullptr;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        bool operator!=(const TYPE* other) const
        {
            return m_Object != other;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        bool operator!=(const Ptr& other) const
        {
            return m_Object != other.m_Object;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        template <class OTHER> bool operator!=(const Ptr<OTHER>& other) const
        {
            return m_Object != other.m_Object;
        }

        /**
         * Compares this pointer with `nullptr`.
         * @return Result of comparison.
         */
        bool operator<(const std::nullptr_t&) const
        {
            return reinterpret_cast<const char*>(m_Object) < nullptr;
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        bool operator<(const TYPE* other) const
        {
            return reinterpret_cast<const char*>(m_Object) < reinterpret_cast<const char*>(other);
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        bool operator<(const Ptr& other) const
        {
            return reinterpret_cast<const char*>(m_Object) < reinterpret_cast<const char*>(other.m_Object);
        }

        /**
         * Compares this pointer with another one.
         * @param other Pointer to compare with.
         * @return Result of comparison.
         */
        template <class OTHER> bool operator<(const Ptr<OTHER>& other) const
        {
            return reinterpret_cast<const char*>(m_Object) < reinterpret_cast<const char*>(other.m_Object);
        }

    private:
        TYPE* m_Object;

        template <class OTHER> friend class Ptr;
    };
}

/** @cond */
namespace std
{
    template <class TYPE> struct hash<Engine::Ptr<TYPE>>
    {
        size_t operator()(const Engine::Ptr<TYPE>& value) const
        {
            return hash<const void*>()(static_cast<TYPE*>(value));
        }
    };
}
/** @endcond */
