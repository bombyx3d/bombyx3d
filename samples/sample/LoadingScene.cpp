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
#include "LoadingScene.h"
#include "engine/core/Application.h"
#include "engine/core/Services.h"
#include "engine/render/ImmediateModeRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <algorithm>

using namespace Engine;

namespace Game
{
    static const glm::vec2 BAR_SIZE(120.0f, 12.0f);

    LoadingScene::LoadingScene()
        : mProjectionMatrix(1.0f)
        , mCurrentProgress(0.0f)
    {
        mProgressBarTexture = Services::resourceManager()->getTexture("loading/ProgressBar.png", false);
        setAutoSwitchScene(false);
    }

    LoadingScene::LoadingScene(const std::function<ScenePtr()>& sceneFactory)
        : LoadingScene()
    {
        beginLoading(sceneFactory());
    }

    void LoadingScene::resize(const glm::vec2& newSize)
    {
        float w = newSize.x;
        float h = newSize.y;
        mProjectionMatrix = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
        mProgressBarQuad = Quad::fromCenterAndSize(glm::vec2(w, h) * 0.5f, BAR_SIZE);
    }

    void LoadingScene::update(double time)
    {
        AbstractLoadingScene::update(time);
        mTargetProgress = std::max(currentProgress(), mTargetProgress);

        if (loadingComplete() && fabsf(mTargetProgress - mCurrentProgress) < 0.01f) {
            switchToNextScene();
            return;
        }

        if (mCurrentProgress < mTargetProgress) {
            mCurrentProgress += std::min(2.0f * float(time), mTargetProgress - mCurrentProgress);
            if (mCurrentProgress > mTargetProgress)
                mCurrentProgress = mTargetProgress;
        }
    }

    void LoadingScene::draw(IRenderer* renderer) const
    {
        renderer->setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        renderer->clear();

        renderer->setProjectionMatrix(mProjectionMatrix);
        renderer->setModelViewMatrix(glm::mat4(1.0f));

        const glm::vec2 texSize = mProgressBarTexture->size();
        float barY = BAR_SIZE.y * 4.0f /* 1 - 5 */;

        const glm::vec2 borderTC0 = glm::vec2(0.0f);
        const glm::vec2 borderTC1 = BAR_SIZE / texSize;
        const glm::vec2 barTC0 = glm::vec2(0.0f, barY) / texSize;
        const glm::vec2 barTC1 = glm::vec2(BAR_SIZE.x * mCurrentProgress, barY + BAR_SIZE.y) / texSize;

        Quad q = mProgressBarQuad;
        q.topRight.x = q.topLeft.x + (q.topRight.x - q.topLeft.x) * mCurrentProgress;
        q.bottomRight.x = q.bottomLeft.x + (q.bottomRight.x - q.bottomLeft.x) * mCurrentProgress;

        ImmediateModeRenderer r;
        r.setBlend(true);
        r.setTexture(mProgressBarTexture);
        r.drawSolidQuad(q, barTC0, barTC1, 0.0f);
        r.drawSolidQuad(mProgressBarQuad, borderTC0, borderTC1, 1.0f);
    }
}
