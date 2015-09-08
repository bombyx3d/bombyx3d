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
#include "engine/render/Renderer.h"
#include "engine/core/Log.h"
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
        IRenderer::createInstance<Renderer>();
        mApplication->initialize(screenSize);

        while (!glfwWindowShouldClose(mWindow)) {
            double time = glfwGetTime();
            mFrameTime = (time > mPrevTime ? time - mPrevTime : 0.0);
            mPrevTime = time;

            glm::ivec2 newScreenSize(0);
            glfwGetFramebufferSize(mWindow, &newScreenSize.x, &newScreenSize.y);
            if (screenSize != newScreenSize) {
                screenSize = newScreenSize;
                Z_LOGI("Application window has been resized to (" << screenSize.x << ", " << screenSize.y << ").");
                mApplication->resize(screenSize);
            }

            mApplication->runFrame(mFrameTime);

            glfwSwapBuffers(mWindow);
            glfwPollEvents();

            if (frameCallback)
                frameCallback();
        }

        Z_LOGI("Application is shutting down.");
        mApplication->shutdown();
        IRenderer::destroyInstance();

        destroyWindow();
    }

    void GlfwWrapper::keyCallback(GLFWwindow*, int, int, int, int)
    {
    }

    void GlfwWrapper::mouseButtonCallback(GLFWwindow* window, int button, int action, int)
    {
        GlfwWrapper* self = reinterpret_cast<GlfwWrapper*>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_LEFT && self->mApplication) {
            if (action == GLFW_PRESS) {
                double mouseX = 0.0, mouseY = 0.0;
                glfwGetCursorPos(window, &mouseX, &mouseY);
                self->mApplication->onTouchBegan(0, glm::ivec2(int(mouseX), int(mouseY)));
                self->mLeftMouseButtonDown = true;
            } else if (action == GLFW_RELEASE) {
                self->mLeftMouseButtonDown = false;
                self->mApplication->onTouchEnded(0);
            }
        }
    }

    void GlfwWrapper::mouseScrollCallback(GLFWwindow*, double, double)
    {
    }

    void GlfwWrapper::mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY)
    {
        GlfwWrapper* self = reinterpret_cast<GlfwWrapper*>(glfwGetWindowUserPointer(window));
        if (self->mLeftMouseButtonDown && self->mApplication)
            self->mApplication->onTouchMoved(0, glm::ivec2(int(mouseX), int(mouseY)));
    }

    void GlfwWrapper::errorCallback(int, const char* description)
    {
        Z_LOGE("GLFW error: " << description);
    }
}
