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
#include "engine/interfaces/render/ICanvas.h"
#include "engine/interfaces/core/IEvent.h"
#include "engine/interfaces/scene/ISceneComponent.h"
#include <memory>
#include <glm/glm.hpp>

namespace B3D
{
    class IScene
    {
    public:
        virtual ~IScene() = default;

        virtual void addComponent(const SceneComponentPtr& component) = 0;
        virtual void addComponent(SceneComponentPtr&& component) = 0;
        virtual void removeComponent(const SceneComponentPtr& component) = 0;

        virtual void setSize(const glm::vec2& newSize) = 0;

        virtual void performUpdate(double time) = 0;
        virtual void performDraw(ICanvas* canvas) const = 0;

        virtual bool beginTouch(int fingerIndex, const glm::vec2& position) = 0;
        virtual void moveTouch(int fingerIndex, const glm::vec2& position) = 0;
        virtual void endTouch(int fingerIndex, const glm::vec2& position) = 0;
        virtual void cancelTouch(int fingerIndex, const glm::vec2& position) = 0;

        virtual void sendEvent(const IEvent* event, bool recursive = true) = 0;
    };

    using ScenePtr = std::shared_ptr<IScene>;
}
