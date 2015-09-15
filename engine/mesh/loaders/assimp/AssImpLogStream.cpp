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
#include "AssImpLogStream.h"
#include "engine/core/Log.h"
#include <mutex>
#include <cstring>

namespace Engine
{
    void AssImpLogStream::write(const char* message)
    {
        size_t length = strlen(message);
        if (length > 0 && message[length - 1] == '\n')
            --length;
        Z_LOGI(" - AssImp: " << std::string(message, length));
    }

    static std::once_flag gInitOnce;
    void AssImpLogStream::init()
    {
        std::call_once(gInitOnce, []() {
            auto severity = Assimp::Logger::NORMAL;
            //auto severity = Assimp::Logger::VERBOSE;
            Assimp::DefaultLogger::create(nullptr, severity, 0, nullptr);

            const unsigned int levels =
                //Assimp::Logger::Debugging |
                //Assimp::Logger::Info |
                Assimp::Logger::Err |
                Assimp::Logger::Warn;
            Assimp::DefaultLogger::get()->attachStream(new AssImpLogStream, levels);
        });
    }
}
