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
 * Deletes copy constructor and assignment operator for the class.
 *
 * This macro should be used inside the class declaration. For example:
 * @code
 * class MyClass
 * {
 *     // ...
 *
 *     Z_DISABLE_COPY(MyClass);
 * };
 * @endcode
 */
#define Z_DISABLE_COPY(CLASSNAME) \
    CLASSNAME(const CLASSNAME&) = delete; \
    CLASSNAME& operator=(const CLASSNAME&) = delete

/**
 * Deletes move constructor and assignment operator for the class.
 *
 * This macro should be used inside the class declaration. For example:
 * @code
 * class MyClass
 * {
 *     // ...
 *
 *     Z_DISABLE_MOVE(MyClass);
 * };
 * @endcode
 */
#define Z_DISABLE_MOVE(CLASSNAME) \
    CLASSNAME(CLASSNAME&&) = delete; \
    CLASSNAME& operator=(CLASSNAME&&) = delete

/**
 * Deletes copy and move constructors and assignment operators for the class.
 *
 * This macro should be used inside the class declaration. For example:
 * @code
 * class MyClass
 * {
 *     // ...
 *
 *     Z_DISABLE_COPY_AND_MOVE(MyClass);
 * };
 * @endcode
 */
#define Z_DISABLE_COPY_AND_MOVE(CLASSNAME) \
    Z_DISABLE_COPY(CLASSNAME); \
    Z_DISABLE_MOVE(CLASSNAME)

/**
 * Deletes constructor, destructor, copy and move constructors and assignment operators for the class.
 *
 * This macro is useful for classes that contain only static methods.
 *
 * This macro should be used inside the class declaration. For example:
 * @code
 * class MyClass
 * {
 *     // ...
 *
 *     Z_STATIC_CLASS(MyClass);
 * };
 * @endcode
 */
#define Z_STATIC_CLASS(CLASSNAME) \
    CLASSNAME() = delete; \
    ~CLASSNAME() = delete; \
    Z_DISABLE_COPY_AND_MOVE(CLASSNAME)
