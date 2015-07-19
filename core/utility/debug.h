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

namespace Engine
{
    // Assertions

    /**
     * @def Z_ASSERTIONS_ENABLED
     * Boolean flag indicating whether assertion checks are enabled.
     * This symbol is defined to `1` if assertions are enabled or to `0` if assertions are disabled.
     */

    /**
     * @def Z_CHECK
     * If assertion checks are enabled, this macro checks whether given condition is `true`.
     * If condition `X` evaluates to `false`, this macro reports failure into the engine log.
     * @note This macro does nothing in release builds.
     * @param X condition to check.
     */

    /**
     * @def Z_CHECK_MSG
     * If assertion checks are enabled, this macro checks whether given condition is `true`.
     * If condition `X` evaluates to `false`, this macro reports failure into the engine log.
     * @note This macro does nothing in release builds.
     * @param X condition to check.
     * @param MSG Message to write into the engine log.
     */

    /**
     * @def Z_ASSERT
     * If assertion checks are enabled, this macro aborts program if given condition is `false`.
     * If condition `X` evaluates to `false`, this macro reports failure into the engine log and aborts execution
     * of the program.
     * @note This macro does nothing in release builds.
     * @param X condition to check.
     */

    /**
     * @def Z_CHECK_MSG
     * If assertion checks are enabled, this macro aborts program if given condition if `false`.
     * If condition `X` evaluates to `false`, this macro reports failure into the engine log and aborts execution
     * of the program.
     * @note This macro does nothing in release builds.
     * @param X condition to check.
     * @param MSG Message to write into the engine log.
     */

    /**
     * @def Z_DEBUG_FAIL
     * If assertion checks are enabled, this macro aborts execution of the program.
     * @note This macro does nothing in release builds.
     * @param MSG Message to write into the engine log.
     */

    #if !defined(Z_ENABLE_ASSERTIONS) && defined(NDEBUG)
     #define Z_ASSERTIONS_ENABLED 0
     #define Z_CHECK(X) ((void)0)
     #define Z_CHECK_MSG(X, MSG) ((void)0)
     #define Z_ASSERT(X) ((void)0)
     #define Z_ASSERT_MSG(X, MSG) ((void)0)
     #define Z_DEBUG_FAIL(MSG) ((void)0)
    #else
     #define Z_ASSERTIONS_ENABLED 1
     #define Z_CHECK(X) ((X) ? (void)0 : ::Engine::assertionFailed(__FILE__, __LINE__, "Check failed: " #X, false))
     #define Z_CHECK_MSG(X, MSG) ((X) ? (void)0 : ::Engine::assertionFailed(__FILE__, __LINE__, (MSG), false))
     #define Z_ASSERT(X) ((X) ? (void)0 : ::Engine::assertionFailed(__FILE__, __LINE__, "Assertion failed: " #X, true))
     #define Z_ASSERT_MSG(X, MSG) ((X) ? (void)0 : ::Engine::assertionFailed(__FILE__, __LINE__, (MSG), true))
     #define Z_DEBUG_FAIL(MSG) (::Engine::assertionFailed(__FILE__, __LINE__, (MSG), true))
     /** @cond */
     void assertionFailed(const char* file, int line, const char* message, bool fatal);
     /** @endcond */
    #endif

    // Logging

    /**
     * @def Z_LOGGING_ENABLED
     * Boolean flag indicating whether logging is enabled.
     * This symbol is defined to `1` if logging is enabled or to `0` if logging is disabled.
     * @see @ref _logging, @ref Engine::setDebugPrinter.
     */

    /**
     * @def Z_LOG
     * If logging is enabled, this macro writes message into the engine log.
     *
     * Given message can be formatted according to the `std::stringstream` rules, for example:
     *
     * @code{.cpp}
     * int counter = 10;
     * Z_LOG("Counter is " << 10);
     * @endcode
     *
     * @param X Message to write into the engine log.
     * @see @ref _logging, @ref Engine::setDebugPrinter.
     */

    #if !defined(Z_ENABLE_LOGGING) && defined(NDEBUG)
     #define Z_LOGGING_ENABLED 0
     #define Z_LOG(X) ((void)0)
    #else
     #define Z_LOGGING_ENABLED 1
     #define Z_LOG(X) (::Engine::debugPrint(static_cast<const std::stringstream&>(std::stringstream() << X)))
    #endif

    /** @cond */
    void debugPrint(const std::stringstream& message);
    void debugPrint(const std::string& message);
    /** @endcond */

    /**
     * Sets a custom printer for debug messages.
     * @param printer Function that will write debug messages. Passing `nullptr` in this argument will
     * restore the default behavior.
     * @see @ref _logging.
     */
    void setDebugPrinter(const std::function<void(const std::string&)>& printer);

    /**
     * Sets a custom printer for debug messages.
     * @param printer Function that will write debug messages. Passing `nullptr` in this argument will
     * restore the default behavior.
     * @see @ref _logging.
     */
    void setDebugPrinter(std::function<void(const std::string&)>&& printer);
}
