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
#include "Win32ConsoleLogger.h"

namespace B3D
{
    static const WORD DARK_GRAY = FOREGROUND_INTENSITY;
    static const WORD LIGHT_GRAY = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
    static const WORD WHITE = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    static const WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    static const WORD RED = FOREGROUND_RED | FOREGROUND_INTENSITY;

    Win32ConsoleLogger::Win32ConsoleLogger()
    {
        mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(mConsoleHandle, &mConsoleInitialSettings);
        SetConsoleOutputCP(CP_UTF8);
        InitializeCriticalSection(&mConsoleCriticalSection);
    }

    Win32ConsoleLogger::~Win32ConsoleLogger()
    {
        DeleteCriticalSection(&mConsoleCriticalSection);
        SetConsoleTextAttribute(mConsoleHandle, mConsoleInitialSettings.wAttributes);
    }

    void Win32ConsoleLogger::write(LogLevel level, const std::string& message)
    {
        WORD wColor = mConsoleInitialSettings.wAttributes;
        switch (level)
        {
        case LogLevel::Trace: wColor = DARK_GRAY; break;
        case LogLevel::Debug: wColor = LIGHT_GRAY; break;
        case LogLevel::Info: wColor = WHITE; break;
        case LogLevel::Warning: wColor = YELLOW; break;
        case LogLevel::Error: wColor = RED; break;
        }

        EnterCriticalSection(&mConsoleCriticalSection);
        SetConsoleTextAttribute(mConsoleHandle, wColor);

        DWORD dwCharactersWritten = 0;
        std::vector<WCHAR> unicode = Win32::multiByteToWideChar(message, 2);
        unicode[unicode.size() - 2] = WCHAR('\n');
        WriteConsoleW(mConsoleHandle, unicode.data(), DWORD(unicode.size() - 1), &dwCharactersWritten, nullptr);

        SetConsoleTextAttribute(mConsoleHandle, mConsoleInitialSettings.wAttributes);
        LeaveCriticalSection(&mConsoleCriticalSection);

        OutputDebugStringW(unicode.data());
    }
}
