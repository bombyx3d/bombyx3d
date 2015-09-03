﻿#include "WinAPI.h"
#include "engine/interfaces/core/IApplication.h"
#include "engine/core/Log.h"
#include "engine/platform/shared/GlfwWrapper.h"
#include <vector>
#include <cstdlib>

using namespace Engine;

static bool gIsConsoleApplication;
static CRITICAL_SECTION gConsoleCriticalSection;
static HANDLE gConsoleHandle;
static CONSOLE_SCREEN_BUFFER_INFO gConsoleInitialSettings;

// Console colors
static const WORD DARK_GRAY = FOREGROUND_INTENSITY;
static const WORD LIGHT_GRAY = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
static const WORD WHITE = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const WORD RED = FOREGROUND_RED | FOREGROUND_INTENSITY;

static int win32Main()
{
    if (gIsConsoleApplication) {
        gConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(gConsoleHandle, &gConsoleInitialSettings);
        SetConsoleOutputCP(CP_UTF8);
        InitializeCriticalSection(&gConsoleCriticalSection);
    }

    Log::setLogger([](LogLevel level, const std::string& message) {
        int unicodeLength = MultiByteToWideChar(CP_UTF8, 0, message.data(), int(message.length()), nullptr, 0);
        if (unicodeLength < 0)
            return;
        std::vector<WCHAR> unicode(size_t(unicodeLength + 2));
        MultiByteToWideChar(CP_UTF8, 0, message.data(), int(message.length()), unicode.data(), unicodeLength);
        unicode[size_t(unicodeLength)] = '\n';

        if (gIsConsoleApplication) {
            WORD wColor = gConsoleInitialSettings.wAttributes;
            switch (level)
            {
            case LogLevel::Trace: wColor = DARK_GRAY; break;
            case LogLevel::Debug: wColor = LIGHT_GRAY; break;
            case LogLevel::Info: wColor = WHITE; break;
            case LogLevel::Warning: wColor = YELLOW; break;
            case LogLevel::Error: wColor = RED; break;
            }

            EnterCriticalSection(&gConsoleCriticalSection);
            SetConsoleTextAttribute(gConsoleHandle, wColor);

            DWORD dwCharactersWritten;
            WriteConsoleW(gConsoleHandle, unicode.data(), unicode.size() - 1, &dwCharactersWritten, nullptr);
        }

        OutputDebugStringW(unicode.data());

        if (gIsConsoleApplication) {
            SetConsoleTextAttribute(gConsoleHandle, gConsoleInitialSettings.wAttributes);
            LeaveCriticalSection(&gConsoleCriticalSection);
        }
    });

    int exitCode = EXIT_SUCCESS;
    GlfwWrapper glfwWrapper;
    if (!glfwWrapper.run()) {
        MessageBoxW(nullptr, L"Game exited with an error.", L"Error", MB_ICONERROR | MB_OK);
        exitCode = EXIT_FAILURE;
    }

    Log::setLogger(nullptr);
    if (gIsConsoleApplication) {
        DeleteCriticalSection(&gConsoleCriticalSection);
        SetConsoleTextAttribute(gConsoleHandle, gConsoleInitialSettings.wAttributes);
    }

    return exitCode;
}

int main()
{
    gIsConsoleApplication = true;
    return win32Main();
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    gIsConsoleApplication = false;
    return win32Main();
}