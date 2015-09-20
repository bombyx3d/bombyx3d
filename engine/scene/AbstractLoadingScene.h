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
#include "engine/scene/AbstractScene.h"

namespace Engine
{
    class AbstractLoadingScene : public AbstractScene
    {
    public:
        AbstractLoadingScene();

        void beginLoading(const ScenePtr& scene);
        void beginLoading(ScenePtr&& scene);
        template <class SCENE, class... ARGS> void beginLoading(ARGS&&... args)
            { beginLoading(std::make_shared<SCENE>(std::forward<ARGS>(args)...)); }

        const ScenePtr& nextScene() const { return mNextScene; }
        void switchToNextScene();

        bool autoSwitchScene() const { return mAutoSwitchScene; }
        void setAutoSwitchScene(bool flag) { mAutoSwitchScene = flag; }

        bool loadingComplete() const { return mLoadingComplete; }
        float currentProgress() const { return mCurrentProgress; }

        void update(double time) override;

    private:
        ScenePtr mNextScene;
        float mCurrentProgress;
        bool mAutoSwitchScene;
        bool mLoadingComplete;
    };
}
