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
#include "engine/interfaces/input/IInputObserver.h"
#include "engine/interfaces/scene/IScene.h"
#include <functional>
#include <unordered_set>

namespace Engine
{
    class Application : public IApplication, public IInputObserver
    {
    public:
        static Application* instance() { return mInstance; }

        glm::ivec2 preferredScreenSize() const override;
        int preferredDepthBits() const override;
        int preferredStencilBits() const override;

        const glm::ivec2& screenSize() { return mScreenSize; }
        float screenAspect() const { return mScreenAspect; }

        const ScenePtr& currentScene() const { return mCurrentScene; }
        void setCurrentScene(const ScenePtr& scene);
        void setCurrentScene(ScenePtr&& scene);

    protected:
        Application();
        ~Application();

        virtual ScenePtr createInitialScene() = 0;

    private:
        static Application* mInstance;
        std::unordered_set<int> mActiveTouches;
        glm::ivec2 mScreenSize;
        float mScreenAspect;
        ScenePtr mPreviousScene;
        ScenePtr mCurrentScene;

        void initialize(const glm::ivec2& screenSize) final override;
        void shutdown() final override;

        void resize(const glm::ivec2& screenSize) final override;

        void runFrame(double time) final override;

        void onTouchBegan(int fingerIndex, const glm::vec2& position) final override;
        void onTouchMoved(int fingerIndex, const glm::vec2& position) final override;
        void onTouchEnded(int fingerIndex) final override;
        void onTouchCancelled(int fingerIndex) final override;

        Z_DISABLE_COPY(Application);
    };

    template <typename TYPE, typename... ARGS> IApplication* createApplicationWithInitialScene(ARGS... args)
    {
        class ApplicationWithScene : public Application
        {
        public:
            ApplicationWithScene(const std::function<ScenePtr()>& factory) : mFactory(factory) {}

        protected:
            std::function<ScenePtr()> mFactory;
            ScenePtr createInitialScene() final override { return mFactory(); }
        };

        auto factory = [=]() -> ScenePtr { return std::make_shared<TYPE>(args...); };
        return new ApplicationWithScene(factory);
    }
}
