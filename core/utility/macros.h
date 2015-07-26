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

namespace Engine
{

/**
 * Deletes copy and move constructors and assignment operators in a class or struct.
 *
 * @param NAME Name of the class or struct.
 *
 * @note This macro resets class' current protection level to `private`.
 *
 * Example usage:
 * @code{.cpp}
 * class MyClass
 * {
 *     // ...
 *
 *     Z_DISABLE_COPY(MyClass)
 * };
 * @endcode
 */
#define Z_DISABLE_COPY(NAME) \
    /** @cond */ \
    private: \
        NAME(const NAME&) = delete; \
        NAME(NAME&&) = delete; \
        NAME& operator=(const NAME&) = delete; \
        NAME& operator=(NAME&&) = delete; \
    /** @endcond */

/** @cond */
#define _Z_DECLARE_QUERY_INTERFACE_METHODS() \
    /** @cond */ \
    virtual void* queryInterface(TypeID typeID) override; \
    template <class TYPE> TYPE* queryInterface() \
        { return reinterpret_cast<TYPE*>(queryInterface(typeOf<TYPE>())); } \
    /** @endcond */
/** @endcond */

/**
 * Helper macro for interface implementations.
 *
 * @param NAME Name of the implementation class.
 *
 * @note This macro resets class' current protection level to `public`.
 *
 * Example usage:
 * @code{.cpp}
 * class UnknownImpl : public IUnknown
 * {
 *     Z_IMPLEMENTATION(UnknownImpl)
 *
 *     // ...
 * };
 * @endcode
 *
 * @see @ref Z_SINGLETON_IMPLEMENTATION, @ref Z_CUSTOM_IMPLEMENTATION, @ref Z_INTERFACE.
 */
#define Z_IMPLEMENTATION(NAME) \
    /** @cond */ \
    Z_DISABLE_COPY(NAME) \
    public: \
        _Z_DECLARE_QUERY_INTERFACE_METHODS() \
    /** @endcond */ \
    public:

/**
 * Helper macro for singleton interface implementations.
 *
 * Instances of singletons are automatically constructed on the program startup and can be later queried
 * from the engine using the @ref Core::querySingleton method.
 *
 * Singleton implementations should have a public constructor that accepts no arguments.
 *
 * If multiple singletons implement the same interface, all of them will be constructed but only one of them
 * will be accessible through the @ref Core::querySingleton method (it is indeterminate which one of them).
 *
 * Singletons are constructed very early at the startup process and should not access any other parts of
 * the engine in their constructors except the @ref Core class.
 *
 * @param NAME Name of the implementation class.
 *
 * @note This macro resets class' current protection level to `public`.
 *
 * Example usage:
 * @code{.cpp}
 * class UnknownImpl : public IUnknown
 * {
 *     Z_SINGLETON_IMPLEMENTATION(UnknownImpl)
 *
 *     // ...
 * };
 * @endcode
 *
 * @see @ref Z_IMPLEMENTATION, @ref Z_CUSTOM_IMPLEMENTATION, @ref Z_INTERFACE.
 */
#define Z_SINGLETON_IMPLEMENTATION(NAME) \
    /** @cond */ \
    Z_DISABLE_COPY(NAME) \
    public: \
        _Z_DECLARE_QUERY_INTERFACE_METHODS() \
    /** @endcond */ \
    public:

/**
 * Helper macro for interface implementations with complex logic behind the @ref IUnknown::queryInterface method.
 *
 * Classes using this macro instead of @ref Z_IMPLEMENTATION should additionally implement method named
 * `_queryCustomInterface` (with underscore at the beginning) that should perform the complex logic.
 *
 * Signature of the `_queryCustomInterface` method should be the same as of the
 * @ref IUnknown::queryInterface method.
 *
 * @param NAME Name of the implementation class.
 *
 * @note This macro resets class' current protection level to `public`.
 *
 * Example usage:
 * @code{.cpp}
 * class UnknownImpl : public IUnknown
 * {
 *     Z_CUSTOM_IMPLEMENTATION(UnknownImpl)
 *
 *     // ...
 *
 * private:
 *     IStream* m_Stream;
 *
 *     void* _queryCustomInterface(TypeID typeID)
 *     {
 *         if (m_Stream != null)
 *             return m_Stream->queryInterface(typeID);
 *         return nullptr;
 *     }
 * };
 * @endcode
 *
 * @see @ref Z_IMPLEMENTATION, @ref Z_SINGLETON_IMPLEMENTATION, @ref Z_INTERFACE.
 */
#define Z_CUSTOM_IMPLEMENTATION(NAME) \
    /** @cond */ \
    Z_DISABLE_COPY(NAME) \
    public: \
        _Z_DECLARE_QUERY_INTERFACE_METHODS() \
    /** @endcond */ \
    public:

/**
 * Helper macro for interfaces.
 * Use this macro to add dummy constructor and destructor to the interface.
 *
 * @param NAME Name of the interface.
 *
 * @note This macro resets class' current protection level to `public`.
 *
 * Example usage:
 * @code{.cpp}
 * class IInterface : public IUnknown
 * {
 *     Z_INTERFACE(IInterface)
 *
 *     // ...
 * };
 * @endcode
 *
 * @see @ref Z_IMPLEMENTATION, @ref Z_SINGLETON_IMPLEMENTATION, @ref Z_CUSTOM_IMPLEMENTATION.
 */
#define Z_INTERFACE(NAME) \
    /** @cond */ \
    Z_DISABLE_COPY(NAME) \
    protected: \
        NAME() = default; \
        virtual ~NAME() = default; \
    public: \
        _Z_DECLARE_QUERY_INTERFACE_METHODS() \
    /** @endcond */ \
    public:

/**
 * @def Z_THREADLOCAL
 * A _thread local_ storage class.
 * Use this macro instead of `thread_local`, `__thread` or `__declspec(thread)`.
 */
#ifdef _MSC_VER
 #define Z_THREADLOCAL __declspec(thread)
#else
 #define Z_THREADLOCAL __thread
#endif

}
