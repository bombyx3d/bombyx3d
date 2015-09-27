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
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <algorithm>

using namespace Engine;

namespace Game
{
    LoadingScene::LoadingScene()
        : mProjectionMatrix(1.0f)
        , mCurrentProgress(0.0f)
    {
        auto spriteSheet = Services::resourceManager()->getSpriteSheet("loading/ProgressBar.xml", false);
        mBorder = spriteSheet->getSprite("border");
        mFiller = spriteSheet->getSprite("gray");
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
        mProgressBarPosition = glm::vec2(w * 0.5f, h - 2.0f * mBorder->originalSize().y);
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

    void LoadingScene::draw(ICanvas* canvas) const
    {
        canvas->setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        canvas->clear();

        canvas->setProjectionMatrix(mProjectionMatrix);
        canvas->setModelViewMatrix(glm::mat4(1.0f));

        canvas->setBlend(true);

        #define interpolate(from, to) ((to).x = (from).x + ((to).x - (from).x) * mCurrentProgress)
        Quad filler = mFiller->trimmedQuad() + mProgressBarPosition;
        interpolate(filler.topLeft, filler.topRight);
        interpolate(filler.bottomLeft, filler.bottomRight);
        Quad fillerTexCoord = mFiller->textureCoordinates();
        interpolate(fillerTexCoord.topLeft, fillerTexCoord.topRight);
        interpolate(fillerTexCoord.bottomLeft, fillerTexCoord.bottomRight);
        canvas->drawTexturedQuad(filler, fillerTexCoord, mFiller->texture());

        canvas->drawSprite(mProgressBarPosition, mBorder);
    }
}
