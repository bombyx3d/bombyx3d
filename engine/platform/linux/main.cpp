#include "engine/core/Log.h"
#include "engine/interfaces/core/IApplication.h"
#include "engine/interfaces/io/IFileSystem.h"
#include "engine/platform/shared/StdIoFileSystem.h"
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

    IFileSystem::createInstance<StdIoFileSystem>(".");

    int exitCode = EXIT_SUCCESS;
    GlfwWrapper glfwWrapper;
    if (!glfwWrapper.run())
        exitCode = EXIT_FAILURE;

    IFileSystem::destroyInstance();

    Log::setLogger(nullptr);

    return exitCode;
}
