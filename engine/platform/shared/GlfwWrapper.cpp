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

namespace B3D
{
    GlfwWrapper::GlfwWrapper()
    {
        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) {
            B3D_LOGE("Unable to initialize GLFW.");
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
      #if (defined(B3D_PLATFORM_WIN32) && defined(B3D_USE_ANGLE)) || defined(B3D_PLATFORM_WINRT)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
      #endif

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);

        glfwWindowHint(GLFW_DEPTH_BITS, mApplication->preferredDepthBits());
        glfwWindowHint(GLFW_STENCIL_BITS, mApplication->preferredStencilBits());

        glm::ivec2 windowSize = mApplication->preferredScreenSize();
        mWindow = glfwCreateWindow(windowSize.x, windowSize.y, "", nullptr, nullptr);
        if (!mWindow) {
            B3D_LOGE("Unable to create GLFW window with size (" << windowSize.x << ", " << windowSize.y << ").");
            return false;
        }

        B3D_TRACE("Created GLFW window with size (" << windowSize.x << ", " << windowSize.y << ").");

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

            B3D_TRACE("Destroyed GLFW window.");
        }
    }

    void GlfwWrapper::run(const std::function<void()>& frameCallback)
    {
        assert(mWindow != nullptr);

        glm::ivec2 screenSize(0);
        glfwGetFramebufferSize(mWindow, &screenSize.x, &screenSize.y);

        B3D_LOGI("Initializing application with window size (" << screenSize.x << ", " << screenSize.y << ").");
        mApplication->initialize(std::make_shared<Renderer>(), glm::vec2(screenSize));

        while (!glfwWindowShouldClose(mWindow)) {
            double time = glfwGetTime();
            mFrameTime = (time > mPrevTime ? time - mPrevTime : 0.0);
            mPrevTime = time;

            glm::ivec2 newScreenSize(0);
            glfwGetFramebufferSize(mWindow, &newScreenSize.x, &newScreenSize.y);
            if (screenSize != newScreenSize) {
                screenSize = newScreenSize;
                B3D_LOGI("Application window has been resized to (" << screenSize.x << ", " << screenSize.y << ").");
                mApplication->resize(glm::vec2(screenSize));
            }

            mApplication->runFrame(mFrameTime);

            glfwSwapBuffers(mWindow);
            glfwPollEvents();

            if (frameCallback)
                frameCallback();
        }

        B3D_LOGI("Application is shutting down.");
        mApplication->shutdown();

        destroyWindow();
    }

    void GlfwWrapper::keyCallback(GLFWwindow*, int key, int, int action, int)
    {
        auto k = keyFromGlfw(key);
        if (k == Key::Unknown)
            return;

        const auto& inputManager = Services::inputManager();
        switch (action)
        {
        case GLFW_PRESS: inputManager->injectKeyPress(k, false); break;
        case GLFW_REPEAT: inputManager->injectKeyPress(k, true); break;
        case GLFW_RELEASE: inputManager->injectKeyRelease(k); break;
        }
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
        B3D_LOGE("GLFW error: " << description);
    }

    Key GlfwWrapper::keyFromGlfw(int key)
    {
        switch (key)
        {
        case GLFW_KEY_SPACE: return Key::Space;
        case GLFW_KEY_APOSTROPHE: return Key::Apostrophe;
        case GLFW_KEY_COMMA: return Key::Comma;
        case GLFW_KEY_MINUS: return Key::Minus;
        case GLFW_KEY_PERIOD: return Key::Period;
        case GLFW_KEY_SLASH: return Key::Slash;
        case GLFW_KEY_0: return Key::_0;
        case GLFW_KEY_1: return Key::_1;
        case GLFW_KEY_2: return Key::_2;
        case GLFW_KEY_3: return Key::_3;
        case GLFW_KEY_4: return Key::_4;
        case GLFW_KEY_5: return Key::_5;
        case GLFW_KEY_6: return Key::_6;
        case GLFW_KEY_7: return Key::_7;
        case GLFW_KEY_8: return Key::_8;
        case GLFW_KEY_9: return Key::_9;
        case GLFW_KEY_SEMICOLON: return Key::Semicolon;
        case GLFW_KEY_EQUAL: return Key::Equal;
        case GLFW_KEY_A: return Key::A;
        case GLFW_KEY_B: return Key::B;
        case GLFW_KEY_C: return Key::C;
        case GLFW_KEY_D: return Key::D;
        case GLFW_KEY_E: return Key::E;
        case GLFW_KEY_F: return Key::F;
        case GLFW_KEY_G: return Key::G;
        case GLFW_KEY_H: return Key::H;
        case GLFW_KEY_I: return Key::I;
        case GLFW_KEY_J: return Key::J;
        case GLFW_KEY_K: return Key::K;
        case GLFW_KEY_L: return Key::L;
        case GLFW_KEY_M: return Key::M;
        case GLFW_KEY_N: return Key::N;
        case GLFW_KEY_O: return Key::O;
        case GLFW_KEY_P: return Key::P;
        case GLFW_KEY_Q: return Key::Q;
        case GLFW_KEY_R: return Key::R;
        case GLFW_KEY_S: return Key::S;
        case GLFW_KEY_T: return Key::T;
        case GLFW_KEY_U: return Key::U;
        case GLFW_KEY_V: return Key::V;
        case GLFW_KEY_W: return Key::W;
        case GLFW_KEY_X: return Key::X;
        case GLFW_KEY_Y: return Key::Y;
        case GLFW_KEY_Z: return Key::Z;
        case GLFW_KEY_LEFT_BRACKET: return Key::LeftBracket;
        case GLFW_KEY_BACKSLASH: return Key::Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return Key::RightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return Key::GraveAccent;
        case GLFW_KEY_WORLD_1: return Key::World1;
        case GLFW_KEY_WORLD_2: return Key::World2;
        case GLFW_KEY_ESCAPE: return Key::Escape;
        case GLFW_KEY_ENTER: return Key::Enter;
        case GLFW_KEY_TAB: return Key::Tab;
        case GLFW_KEY_BACKSPACE: return Key::Backspace;
        case GLFW_KEY_INSERT: return Key::Insert;
        case GLFW_KEY_DELETE: return Key::Delete;
        case GLFW_KEY_RIGHT: return Key::Right;
        case GLFW_KEY_LEFT: return Key::Left;
        case GLFW_KEY_DOWN: return Key::Down;
        case GLFW_KEY_UP: return Key::Up;
        case GLFW_KEY_PAGE_UP: return Key::PageUp;
        case GLFW_KEY_PAGE_DOWN: return Key::PageDown;
        case GLFW_KEY_HOME: return Key::Home;
        case GLFW_KEY_END: return Key::End;
        case GLFW_KEY_CAPS_LOCK: return Key::CapsLock;
        case GLFW_KEY_SCROLL_LOCK: return Key::ScrollLock;
        case GLFW_KEY_NUM_LOCK: return Key::NumLock;
        case GLFW_KEY_PRINT_SCREEN: return Key::PrintScreen;
        case GLFW_KEY_PAUSE: return Key::Pause;
        case GLFW_KEY_F1: return Key::F1;
        case GLFW_KEY_F2: return Key::F2;
        case GLFW_KEY_F3: return Key::F3;
        case GLFW_KEY_F4: return Key::F4;
        case GLFW_KEY_F5: return Key::F5;
        case GLFW_KEY_F6: return Key::F6;
        case GLFW_KEY_F7: return Key::F7;
        case GLFW_KEY_F8: return Key::F8;
        case GLFW_KEY_F9: return Key::F9;
        case GLFW_KEY_F10: return Key::F10;
        case GLFW_KEY_F11: return Key::F11;
        case GLFW_KEY_F12: return Key::F12;
        case GLFW_KEY_F13: return Key::F13;
        case GLFW_KEY_F14: return Key::F14;
        case GLFW_KEY_F15: return Key::F15;
        case GLFW_KEY_F16: return Key::F16;
        case GLFW_KEY_F17: return Key::F17;
        case GLFW_KEY_F18: return Key::F18;
        case GLFW_KEY_F19: return Key::F19;
        case GLFW_KEY_F20: return Key::F20;
        case GLFW_KEY_F21: return Key::F21;
        case GLFW_KEY_F22: return Key::F22;
        case GLFW_KEY_F23: return Key::F23;
        case GLFW_KEY_F24: return Key::F24;
        case GLFW_KEY_F25: return Key::F25;
        case GLFW_KEY_KP_0: return Key::KeyPad0;
        case GLFW_KEY_KP_1: return Key::KeyPad1;
        case GLFW_KEY_KP_2: return Key::KeyPad2;
        case GLFW_KEY_KP_3: return Key::KeyPad3;
        case GLFW_KEY_KP_4: return Key::KeyPad4;
        case GLFW_KEY_KP_5: return Key::KeyPad5;
        case GLFW_KEY_KP_6: return Key::KeyPad6;
        case GLFW_KEY_KP_7: return Key::KeyPad7;
        case GLFW_KEY_KP_8: return Key::KeyPad8;
        case GLFW_KEY_KP_9: return Key::KeyPad9;
        case GLFW_KEY_KP_DECIMAL: return Key::KeyPadDecimal;
        case GLFW_KEY_KP_DIVIDE: return Key::KeyPadDivide;
        case GLFW_KEY_KP_MULTIPLY: return Key::KeyPadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return Key::KeyPadSubtract;
        case GLFW_KEY_KP_ADD: return Key::KeyPadAdd;
        case GLFW_KEY_KP_ENTER: return Key::KeyPadEnter;
        case GLFW_KEY_KP_EQUAL: return Key::KeyPadEqual;
        case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
        case GLFW_KEY_LEFT_CONTROL: return Key::LeftControl;
        case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
        case GLFW_KEY_LEFT_SUPER: return Key::LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
        case GLFW_KEY_RIGHT_CONTROL: return Key::RightControl;
        case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;
        case GLFW_KEY_RIGHT_SUPER: return Key::RightSuper;
        case GLFW_KEY_MENU: return Key::Menu;
        }

        return Key::Unknown;
    }
}
