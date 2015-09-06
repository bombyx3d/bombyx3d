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
#include "engine/core/Log.h"
#include "engine/interfaces/core/IApplication.h"
#include "engine/interfaces/io/IFileSystem.h"
#include "engine/platform/shared/StdIoFileSystem.h"
#include "engine/platform/shared/CxxThreadManager.h"
#include "engine/platform/shared/GlfwWrapper.h"
#include <unistd.h>

using namespace Engine;

int main()
{
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "C");

    Log::setLogger([](LogLevel level, const std::string& message) {
        const char* ansi = "\033[0m";
        switch (level)
        {
        case LogLevel::Trace: ansi = "\033[30;1m"; break;
        case LogLevel::Debug: ansi = "\033[37m"; break;
        case LogLevel::Info: ansi = "\033[37;1m"; break;
        case LogLevel::Warning: ansi = "\033[33;1m"; break;
        case LogLevel::Error: ansi = "\033[31;1m"; break;
        }

        if (!isatty(STDOUT_FILENO))
            printf("%s\n", message.c_str());
        else
            printf("%s%s\033[0m\n", ansi, message.c_str());
    });

    IThreadManager::createInstance<CxxThreadManager>();
    IFileSystem::createInstance<StdIoFileSystem>(".");

    int exitCode = EXIT_SUCCESS;
    GlfwWrapper glfwWrapper;
    if (!glfwWrapper.createWindow())
        exitCode = EXIT_FAILURE;
    else {
        glfwWrapper.run([](){
            static_cast<CxxThreadManager&>(*IThreadManager::instance()).flushRenderThreadQueue();
        });
    }

    IFileSystem::destroyInstance();
    IThreadManager::destroyInstance();

    Log::setLogger(nullptr);

    return exitCode;
}
