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
#include "MainScene.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"

using namespace Engine;

namespace Game
{
    MainScene::MainScene()
    {
        mCamera = std::make_shared<OrbitCamera>();
        mCamera->setVerticalAngle(glm::radians(45.0f));
    }

    MainScene::~MainScene()
    {
    }

    void MainScene::resize(const glm::ivec2& newSize)
    {
        mCamera->setAspectRatioFromSize(newSize);
    }

    void MainScene::update(double)
    {
        /*
        mCamera->setDistance(glm::length(mMesh->boundingBox().size()));
        mCamera->setTarget(mMesh->boundingBox().center());
        */
    }

    void MainScene::draw(IRenderer* renderer) const
    {
        renderer->setProjectionMatrix(mCamera->projectionMatrix());
        renderer->setModelViewMatrix(mCamera->viewMatrix());

        /*
        for (const auto& element : mMesh->elements()) {
            element->material()->technique(0)->pass(0)->apply(renderer);
            renderer->bindVertexSource(element->vertexSource());
            renderer->drawPrimitive(element->primitiveType(), element->indexBufferOffset(), element->indexCount());
        }
        */
    }

    bool MainScene::onTouchBegan(int fingerIndex, const glm::ivec2& position)
    {
        mPrevTouchPosition = position;
        return true;
    }

    void MainScene::onTouchMoved(int fingerIndex, const glm::ivec2& position)
    {
        glm::ivec2 delta = position - mPrevTouchPosition;
        mPrevTouchPosition = position;
        mCamera->setHorizontalAngle(mCamera->horizontalAngle() + float(delta.x) * 0.01f);
        mCamera->setVerticalAngle(glm::clamp(mCamera->verticalAngle() + float(delta.y) * 0.01f,
            -glm::radians(89.0f), glm::radians(89.0f)));
    }

    void MainScene::onTouchEnded(int fingerIndex)
    {
    }

    void MainScene::onTouchCancelled(int fingerIndex)
    {
    }
}
