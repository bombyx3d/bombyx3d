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
#include <functional>
#include <sstream>

namespace Z
{
    // Assertions

    #if !defined(Z_ENABLE_ASSERTIONS) && defined(NDEBUG)
     #define Z_ASSERTIONS_ENABLED 0
     #define Z_CHECK(X) ((void)0)
     #define Z_CHECK_MSG(X, MSG) ((void)0)
     #define Z_ASSERT(X) ((void)0)
     #define Z_ASSERT_MSG(X, MSG) ((void)0)
     #define Z_DEBUG_FAIL(MSG) ((void)0)
    #else
     #define Z_ASSERTIONS_ENABLED 1
     #define Z_CHECK(X) ((X) ? (void)0 : ::Z::assertionFailed(__FILE__, __LINE__, "Check failed: " #X, false))
     #define Z_CHECK_MSG(X, MSG) ((X) ? (void)0 : ::Z::assertionFailed(__FILE__, __LINE__, (MSG), false))
     #define Z_ASSERT(X) ((X) ? (void)0 : ::Z::assertionFailed(__FILE__, __LINE__, "Assertion failed: " #X, true))
     #define Z_ASSERT_MSG(X, MSG) ((X) ? (void)0 : ::Z::assertionFailed(__FILE__, __LINE__, (MSG), true))
     #define Z_DEBUG_FAIL(MSG) (::Z::assertionFailed(__FILE__, __LINE__, (MSG), true))
     void assertionFailed(const char* file, int line, const char* message, bool fatal);
    #endif

    // Logging

    #if !defined(Z_ENABLE_LOGGING) && defined(NDEBUG)
     #define Z_LOGGING_ENABLED 0
     #define Z_LOG(X) ((void)0)
    #else
     #define Z_LOGGING_ENABLED 1
     #define Z_LOG(X) (::Z::debugPrint(static_cast<const std::stringstream&>(std::stringstream() << X)))
    #endif

    void debugPrint(const std::stringstream& message);
    void debugPrint(const std::string& message);
    void setDebugPrinter(const std::function<void(const std::string&)>& printer);
    void setDebugPrinter(std::function<void(const std::string&)>&& printer);
}
