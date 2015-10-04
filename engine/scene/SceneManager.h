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
#include "engine/interfaces/render/lowlevel/IRenderer.h"
#include "engine/interfaces/scene/ISceneManager.h"
#include "engine/interfaces/input/IInputObserver.h"
#include "engine/render/Canvas.h"
#include <memory>
#include <unordered_set>
#include <glm/glm.hpp>

namespace B3D
{
    class SceneManager : public ISceneManager, public IInputObserver
    {
    public:
        SceneManager(const RendererPtr& renderer, const glm::vec2& screenSize);
        ~SceneManager();

        const glm::vec4& defaultClearColor() const override { return mDefaultClearColor; }
        void setDefaultClearColor(const glm::vec4& color) override { mDefaultClearColor = color; }

        const glm::vec2& screenSize() const override { return mScreenSize; }
        float screenAspect() const override { return mScreenAspect; }

        const ScenePtr& currentScene() const override { return mCurrentScene; }
        void setCurrentScene(const ScenePtr& scene) override;
        void setCurrentScene(ScenePtr&& scene) override;

        void resize(const glm::vec2& screenSize);

        void runFrame(double time);

    private:
        std::unordered_set<int> mActiveTouches;
        RendererPtr mRenderer;
        glm::vec4 mDefaultClearColor;
        std::unique_ptr<Canvas> mCanvas;
        glm::vec2 mScreenSize;
        float mScreenAspect;
        ScenePtr mCurrentScene;

        void onTouchBegan(int fingerIndex, const glm::vec2& position) final override;
        void onTouchMoved(int fingerIndex, const glm::vec2& position) final override;
        void onTouchEnded(int fingerIndex, const glm::vec2& position) final override;
        void onTouchCancelled(int fingerIndex, const glm::vec2& position) final override;

        glm::vec2 adjustTouchPosition(const glm::vec2& position) const;

        B3D_DISABLE_COPY(SceneManager);
    };
}
