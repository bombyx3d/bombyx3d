/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include "platform/PlatformInitOptions.h"

#ifdef NDEBUG
 #undef Z_DEBUG_ASSETS_PATH
#endif
#ifndef Z_DEBUG_ASSETS_PATH
 #define Z_DEBUG_ASSETS_PATH nullptr
#endif

#define Z_GAME_CLASS(CLASS) \
    ::Z::Game* ::Z::Game::create() { \
        ::Z::Game* game = new ::CLASS; \
        game->setAssetsLocationHint(Z_DEBUG_ASSETS_PATH); \
        return game; \
    }

namespace Z
{
    class Game : public PlatformInitOptions
    {
    public:
        Game() = default;
        virtual ~Game() = default;

        static Game* create();

        virtual int preferredDisplayWidth() const override;
        virtual int preferredDisplayHeight() const override;
        virtual int preferredDepthBufferBits() const override;
        virtual int preferredStencilBufferBits() const override;

        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void runFrame(double time) = 0;

        virtual void onPointerPressed(int id, float x, float y);
        virtual void onPointerMoved(int id, float x, float y);
        virtual void onPointerReleased(int id, float x, float y);
        virtual void onPointerCancelled(int id, float x, float y);

    private:
        const char* m_AssetsLocationHint = nullptr;

        void setAssetsLocationHint(const char* hint);
        virtual const char* assetsLocationHint() const final override;
    };
}
