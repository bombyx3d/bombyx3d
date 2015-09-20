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
#include "InitialLoadingScene.h"
#include "MainScene.h"
#include "engine/utility/RenderUtils.h"
#include "engine/core/Application.h"
#include "engine/core/Services.h"
#include "engine/mesh/VertexFormat.h"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <algorithm>

using namespace Engine;

namespace Game
{
    namespace
    {
        Z_VERTEX_FORMAT(Vertex,
            (glm::vec3) position,
            (glm::vec2) texCoord
        )

        const float BAR_WIDTH = 120.0f;
        const float BAR_HEIGHT = 12.0f;

        const float TEXTURE_WIDTH = 120.0f;
        const float TEXTURE_HEIGHT = 72.0f;
    }

    InitialLoadingScene::InitialLoadingScene()
        : mProjectionMatrix(1.0f)
        , mPoint1(0.0f)
        , mPoint2(0.0f)
        , mCurrentProgress(0.0f)
    {
        mTextureUniform = AtomTable::getAtom("uTexture");

        mProgressBarShader = Services::resourceManager()->getShader("shaders/BasicTextured.glsl", false);
        mProgressBarTexture = Services::resourceManager()->getTexture("loading/ProgressBar.png", false);

        static const uint16_t indices[] = { 0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7 };
        mIndexBuffer = Services::renderer()->createIndexBuffer();
        mIndexBuffer->setData(indices, sizeof(indices), BufferUsage::Static);

        mVertexBuffer = Services::renderer()->createVertexBuffer();
        mVertexSource = RenderUtils::createVertexSource<Vertex>(mVertexBuffer, mIndexBuffer);

        setAutoSwitchScene(false);
        beginLoading<MainScene>();
    }

    void InitialLoadingScene::resize(const glm::ivec2& newSize)
    {
        float w = float(newSize.x);
        float h = float(newSize.y);
        mProjectionMatrix = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

        mPoint1 = glm::vec2(w - BAR_WIDTH, h - BAR_HEIGHT) * 0.5f;
        mPoint2 = mPoint1 + glm::vec2(BAR_WIDTH, BAR_HEIGHT);
    }

    void InitialLoadingScene::update(double time)
    {
        LoadingScene::update(time);
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

    void InitialLoadingScene::draw(IRenderer* renderer) const
    {
        renderer->setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        renderer->clear();

        const float aTX1 = 0.0f / TEXTURE_WIDTH;
        const float aTY1 = 0.0f / TEXTURE_HEIGHT;
        const float aTX2 = BAR_WIDTH / TEXTURE_WIDTH;
        const float aTY2 = BAR_HEIGHT / TEXTURE_HEIGHT;

        const float color = 4.0f;
        const float bTX1 = 0.0f / TEXTURE_WIDTH;
        const float bTY1 = (BAR_HEIGHT * color) / TEXTURE_HEIGHT;
        const float bTX2 = (BAR_WIDTH * mCurrentProgress) / TEXTURE_WIDTH;
        const float bTY2 = (BAR_HEIGHT * (color + 1.0f)) / TEXTURE_HEIGHT;

        glm::vec2 point1 = mPoint1 + glm::vec2(1.0f, 0.0f);
        glm::vec2 point2 = glm::vec2(mPoint1.x + (mPoint2.x - mPoint1.x) * mCurrentProgress, mPoint2.y);

        const Vertex vertices[] = {
            { {  point1.x,  point1.y, 0.0f }, { bTX1, bTY1 } },
            { {  point2.x,  point1.y, 0.0f }, { bTX2, bTY1 } },
            { {  point1.x,  point2.y, 0.0f }, { bTX1, bTY2 } },
            { {  point2.x,  point2.y, 0.0f }, { bTX2, bTY2 } },
            { { mPoint1.x, mPoint1.y, 0.0f }, { aTX1, aTY1 } },
            { { mPoint2.x, mPoint1.y, 0.0f }, { aTX2, aTY1 } },
            { { mPoint1.x, mPoint2.y, 0.0f }, { aTX1, aTY2 } },
            { { mPoint2.x, mPoint2.y, 0.0f }, { aTX2, aTY2 } },
        };
        mVertexBuffer->setData(vertices, sizeof(vertices), BufferUsage::Stream);

        renderer->setBlendingEnabled(true);
        renderer->setBlendFunc(BlendFunc::SrcAlpha, BlendFunc::OneMinusSrcAlpha);
        renderer->setDepthTestingEnabled(false);
        renderer->setDepthWritingEnabled(false);
        renderer->setCullFace(CullFace::None);

        renderer->setProjectionMatrix(mProjectionMatrix);
        renderer->setModelViewMatrix(glm::mat4(1.0f));

        renderer->useShader(mProgressBarShader);
        renderer->setUniform(mTextureUniform, mProgressBarTexture);

        renderer->bindVertexSource(mVertexSource);
        renderer->drawPrimitive(PrimitiveType::Triangles, 0, 12);
    }
}
