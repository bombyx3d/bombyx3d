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
#include <glm/gtc/matrix_transform.hpp>

using namespace B3D;

namespace Game
{
    MainScene::MainScene()
    {
        mCamera = std::make_shared<OrbitCamera>();
        mCamera->setHorizontalAngle(glm::radians(0.0f));
        mCamera->setVerticalAngle(glm::radians(0.0f));
        mCamera->setDepthRange(0.1f, 10.0f);
        addComponent(mCamera);

        mMesh = Services::resourceManager()->getStaticMesh("girl/girl.obj");
    }

    MainScene::~MainScene()
    {
    }

    void MainScene::update(double)
    {
        mCamera->setDistance(0.7f * glm::length(mMesh->boundingBox().size()));
        mCamera->setTarget(mMesh->boundingBox().center());
    }

    void MainScene::draw(ICanvas* canvas) const
    {
        glm::mat4 matrix = canvas->modelViewMatrix() * glm::translate(glm::mat4(1.0f), mCamera->target());
        matrix = matrix * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = matrix * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = matrix * glm::translate(glm::mat4(1.0f), -mCamera->target());
        canvas->setModelViewMatrix(matrix);

        canvas->setDepthTest(true);
        mMesh->render(canvas);
        canvas->drawWireframeBoundingBox(mMesh->boundingBox());
    }

    bool MainScene::beginTouch(int fingerIndex, const glm::vec2& position)
    {
        if (fingerIndex != 0)
            return false;

        mPrevTouchPosition = position;
        return true;
    }

    void MainScene::moveTouch(int fingerIndex, const glm::vec2& position)
    {
        if (fingerIndex != 0)
            return;

        glm::vec2 delta = position - mPrevTouchPosition;
        mPrevTouchPosition = position;
        mCamera->setHorizontalAngle(mCamera->horizontalAngle() + delta.x * 0.01f);
        mCamera->setVerticalAngle(glm::clamp(mCamera->verticalAngle() + delta.y * 0.01f,
            -glm::radians(89.0f), glm::radians(89.0f)));
    }

    void MainScene::endTouch(int, const glm::vec2&)
    {
    }

    void MainScene::cancelTouch(int, const glm::vec2&)
    {
    }
}
