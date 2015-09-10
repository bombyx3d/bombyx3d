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
#include "engine/core/Services.h"
#include "engine/core/Log.h"
#include "engine/input/InputManager.h"
#include "engine/platform/shared/StdIoFileSystem.h"
#include "engine/platform/shared/CxxThreadManager.h"
#include "engine/platform/shared/GlfwWrapper.h"
#include "engine/platform/shared/PosixLogger.h"
#include "engine/render/opengl.h"
#include <clocale>

using namespace Engine;

int main()
{
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "C");

    Services::setLogger(std::make_shared<PosixLogger>());

    auto threadManager = std::make_shared<CxxThreadManager>();
    Services::setThreadManager(threadManager);
    Services::setFileSystem(std::make_shared<StdIoFileSystem>("."));

    auto inputManager = std::make_shared<InputManager>();
    inputManager->setHasKeyboard(true);
    inputManager->setHasMouse(true);
    Services::setInputManager(inputManager);

    int exitCode = EXIT_SUCCESS;
    {
        GlfwWrapper glfwWrapper;
        if (!glfwWrapper.createWindow())
            exitCode = EXIT_FAILURE;
        else {
            glewExperimental = GL_TRUE;
            if (glewInit() == GLEW_OK) {
                glfwWrapper.run([threadManager](){ threadManager->flushRenderThreadQueue(); });
            } else {
                Z_LOGE("Unable to initialize GLEW.");
                glfwWrapper.destroyWindow();
                exitCode = EXIT_FAILURE;
            }
        }
    }

    threadManager.reset();
    inputManager.reset();
    Services::setInputManager(nullptr);
    Services::setFileSystem(nullptr);
    Services::setThreadManager(nullptr);
    Services::setLogger(nullptr);

    return exitCode;
}
