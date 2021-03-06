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
#include "engine/scene/camera/OrbitCamera.h"
#include "engine/interfaces/mesh/IMesh.h"

namespace Game
{
    class MainScene : public B3D::AbstractScene
    {
    public:
        MainScene();
        ~MainScene();

        void update(double time) override;
        void draw(B3D::ICanvas* canvas) const override;

        bool onTouchBegan(int fingerIndex, const glm::vec2& position) override;
        void onTouchMoved(int fingerIndex, const glm::vec2& position) override;
        void onTouchEnded(int fingerIndex, const glm::vec2& position) override;
        void onTouchCancelled(int fingerIndex, const glm::vec2& position) override;

    private:
        B3D::OrbitCameraPtr mCamera;
        B3D::MeshPtr mMesh;
        glm::vec2 mPrevTouchPosition;
    };
}
