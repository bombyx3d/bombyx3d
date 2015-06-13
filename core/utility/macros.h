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

/**
 * Deletes copy and move constructors and assignment operators in the class or struct.
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

/**
 * Helper macro for interfaces.
 *
 * Use this macro to add dummy constructor and destructor to the interface.
 *
 * @param NAME Name of the interface.
 *
 * Example usage:
 * @code{.cpp}
 * struct IInterface : public IUnknown
 * {
 *     Z_INTERFACE(IInterface)
 *
 *     // ...
 * };
 * @endcode
 */
#define Z_INTERFACE(NAME) \
    Z_DISABLE_COPY(NAME) \
    /** @cond */ \
    public: \
        NAME() = default; \
        virtual ~NAME() = default; \
    /** @endcond */

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
