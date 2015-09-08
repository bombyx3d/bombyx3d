
#pragma once
#include "engine/core/macros.h"
#include "engine/interfaces/core/IApplication.h"
#include "engine/interfaces/scene/IScene.h"
#include <functional>

namespace Engine
{
    class Application : public IApplication
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
        glm::ivec2 mScreenSize;
        float mScreenAspect;
        ScenePtr mPreviousScene;
        ScenePtr mCurrentScene;

        void initialize(const glm::ivec2& screenSize) final override;
        void shutdown() final override;

        void resize(const glm::ivec2& screenSize) final override;

        void runFrame(double time) final override;

        void onTouchBegan(int fingerIndex, const glm::ivec2& position) final override;
        void onTouchMoved(int fingerIndex, const glm::ivec2& position) final override;
        void onTouchEnded(int fingerIndex) final override;
        void onTouchCancelled(int fingerIndex) final override;

        void cancelAllTouches();

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
