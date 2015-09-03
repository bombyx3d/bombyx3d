
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/core/IApplication.h"
#include <glm/glm.hpp>
#include <memory>

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

        void run();

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
