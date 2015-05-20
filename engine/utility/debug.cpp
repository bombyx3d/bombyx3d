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
#include "debug.h"
#include "macros.h"
#include <sstream>
#include <iostream>
#include <cstdlib>

namespace Z
{
    static std::function<void(const std::string&)> g_DebugPrinter;

    #if defined(Z_ENABLE_ASSERTIONS) || !defined(NDEBUG)
    static Z_THREADLOCAL bool recursive = false;
    void assertionFailed(const char* file, int line, const char* message, bool fatal)
    {
        if (recursive)
            abort();
        recursive = true;

        std::stringstream ss;
        ss << "In file \"" << file << "\" at line " << line << ": " << message;
        debugPrint(ss);

        recursive = false;

        if (fatal)
            abort();
    }
    #endif

    void debugPrint(const std::stringstream& message)
    {
        debugPrint(message.str());
    }

    void debugPrint(const std::string& message)
    {
        if (g_DebugPrinter)
            g_DebugPrinter(message);
        else
            std::cerr << message << std::endl;
    }

    void setDebugPrinter(const std::function<void(const std::string&)>& printer)
    {
        g_DebugPrinter = printer;
    }

    void setDebugPrinter(std::function<void(const std::string&)>&& printer)
    {
        g_DebugPrinter = std::move(printer);
    }
}
