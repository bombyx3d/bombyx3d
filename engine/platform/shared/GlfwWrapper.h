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
#include "engine/core/macros.h"
#include "engine/interfaces/core/IApplication.h"
#include <glm/glm.hpp>
#include <memory>
#include <functional>

struct GLFWwindow;

namespace Engine
{
    class GlfwWrapper
    {
    public:
        GlfwWrapper();
        ~GlfwWrapper();

        bool createWindow();
        void destroyWindow();

        void run(const std::function<void()>& frameCallback);

    private:
        bool mInitialized = false;
        GLFWwindow* mWindow = nullptr;
        double mPrevTime = 0.0;
        double mFrameTime = 0.0;
        std::unique_ptr<IApplication> mApplication;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void mouseScrollCallback(GLFWwindow* window, double x, double y);
        static void mouseMoveCallback(GLFWwindow* window, double x, double y);
        static void errorCallback(int error, const char* description);

        Z_DISABLE_COPY(GlfwWrapper);
    };
}
