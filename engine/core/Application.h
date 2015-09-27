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
#include "engine/render/Canvas.h"
#include <functional>
#include <unordered_set>
#include <memory>
#include <cassert>

namespace Engine
{
    class Application : public IApplication, public IInputObserver
    {
    public:
        glm::ivec2 preferredScreenSize() const override;
        int preferredDepthBits() const override;
        int preferredStencilBits() const override;

        static const glm::vec2& screenSize() { assert(mInstance != nullptr); return mInstance->mScreenSize; }
        static float screenAspect() { assert(mInstance != nullptr); return mInstance->mScreenAspect; }

        static const ScenePtr& currentScene() { assert(mInstance != nullptr); return mInstance->mCurrentScene; }
        static void setCurrentScene(const ScenePtr& scene);
        static void setCurrentScene(ScenePtr&& scene);

    protected:
        Application();
        ~Application();

        virtual ScenePtr createInitialScene() = 0;

    private:
        static Application* mInstance;
        std::unordered_set<int> mActiveTouches;
        glm::vec2 mScreenSize;
        float mScreenAspect;
        ScenePtr mPreviousScene;
        ScenePtr mCurrentScene;
        RendererPtr mRenderer;
        std::unique_ptr<Canvas> mCanvas;

        void initialize(const RendererPtr& renderer, const glm::vec2& screenSize) final override;
        void shutdown() final override;

        void resize(const glm::vec2& screenSize) final override;

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
