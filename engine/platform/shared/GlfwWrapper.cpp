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
#include "GlfwWrapper.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"
#include "engine/render/gles2/GLES2Renderer.h"
#include <cstdlib>
#include <cassert>
#include <GLFW/glfw3.h>

namespace Engine
{
    GlfwWrapper::GlfwWrapper()
    {
        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) {
            Z_LOGE("Unable to initialize GLFW.");
            return;
        }

        mInitialized = true;
        mApplication.reset(IApplication::create());
    }

    GlfwWrapper::~GlfwWrapper()
    {
        mApplication.reset();
        if (mInitialized) {
            destroyWindow();
            glfwTerminate();
        }
    }

    bool GlfwWrapper::createWindow()
    {
        if (!mInitialized)
            return false;

        destroyWindow();

        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);

        glfwWindowHint(GLFW_DEPTH_BITS, mApplication->preferredDepthBits());
        glfwWindowHint(GLFW_STENCIL_BITS, mApplication->preferredStencilBits());

        glm::ivec2 windowSize = mApplication->preferredScreenSize();
        mWindow = glfwCreateWindow(windowSize.x, windowSize.y, "", nullptr, nullptr);
        if (!mWindow) {
            Z_LOGE("Unable to create GLFW window with size (" << windowSize.x << ", " << windowSize.y << ").");
            return false;
        }

        Z_TRACE("Created GLFW window with size (" << windowSize.x << ", " << windowSize.y << ").");

        glfwSetWindowUserPointer(mWindow, this);
        glfwSetKeyCallback(mWindow, keyCallback);
        glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
        glfwSetScrollCallback(mWindow, mouseScrollCallback);
        glfwSetCursorPosCallback(mWindow, mouseMoveCallback);

        glfwMakeContextCurrent(mWindow);
        glfwSwapInterval(1);

        mPrevTime = glfwGetTime();
        mFrameTime = 0.0;

        return true;
    }

    void GlfwWrapper::destroyWindow()
    {
        if (mWindow) {
            glfwDestroyWindow(mWindow);
            mWindow = nullptr;
            mFrameTime = 0.0;

            Z_TRACE("Destroyed GLFW window.");
        }
    }

    void GlfwWrapper::run(const std::function<void()>& frameCallback)
    {
        assert(mWindow != nullptr);

        glm::ivec2 screenSize(0);
        glfwGetFramebufferSize(mWindow, &screenSize.x, &screenSize.y);

        Z_LOGI("Initializing application with window size (" << screenSize.x << ", " << screenSize.y << ").");
        auto renderer = std::make_shared<Renderer>();
        Services::setRendererResourceFactory(renderer);
        mApplication->initialize(renderer, glm::vec2(screenSize));

        while (!glfwWindowShouldClose(mWindow)) {
            double time = glfwGetTime();
            mFrameTime = (time > mPrevTime ? time - mPrevTime : 0.0);
            mPrevTime = time;

            glm::ivec2 newScreenSize(0);
            glfwGetFramebufferSize(mWindow, &newScreenSize.x, &newScreenSize.y);
            if (screenSize != newScreenSize) {
                screenSize = newScreenSize;
                Z_LOGI("Application window has been resized to (" << screenSize.x << ", " << screenSize.y << ").");
                mApplication->resize(glm::vec2(screenSize));
            }

            mApplication->runFrame(mFrameTime);

            glfwSwapBuffers(mWindow);
            glfwPollEvents();

            if (frameCallback)
                frameCallback();
        }

        Z_LOGI("Application is shutting down.");
        mApplication->shutdown();
        Services::setRendererResourceFactory(nullptr);
        renderer.reset();

        destroyWindow();
    }

    void GlfwWrapper::keyCallback(GLFWwindow*, int, int, int, int)
    {
    }

    void GlfwWrapper::mouseButtonCallback(GLFWwindow*, int button, int action, int)
    {
        if (action == GLFW_PRESS) {
            const auto& inputManager = Services::inputManager();
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT: inputManager->injectMouseButtonPress(MouseButton::Left); break;
            case GLFW_MOUSE_BUTTON_RIGHT: inputManager->injectMouseButtonPress(MouseButton::Right); break;
            case GLFW_MOUSE_BUTTON_MIDDLE: inputManager->injectMouseButtonPress(MouseButton::Middle); break;
            }
        }

        if (action == GLFW_RELEASE) {
            const auto& inputManager = Services::inputManager();
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT: inputManager->injectMouseButtonRelease(MouseButton::Left); break;
            case GLFW_MOUSE_BUTTON_RIGHT: inputManager->injectMouseButtonRelease(MouseButton::Right); break;
            case GLFW_MOUSE_BUTTON_MIDDLE: inputManager->injectMouseButtonRelease(MouseButton::Middle); break;
            }
        }
    }

    void GlfwWrapper::mouseScrollCallback(GLFWwindow*, double, double)
    {
    }

    void GlfwWrapper::mouseMoveCallback(GLFWwindow*, double mouseX, double mouseY)
    {
        Services::inputManager()->injectMouseMove(glm::vec2(float(mouseX), float(mouseY)));
    }

    void GlfwWrapper::errorCallback(int, const char* description)
    {
        Z_LOGE("GLFW error: " << description);
    }
}
