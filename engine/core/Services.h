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
#include "engine/interfaces/core/ILogger.h"
#include "engine/interfaces/core/IResourceManager.h"
#include "engine/interfaces/core/IThreadManager.h"
#include "engine/interfaces/input/IInputManager.h"
#include "engine/interfaces/io/IFileSystem.h"
#include "engine/interfaces/render/lowlevel/IRenderer.h"

namespace Engine
{
    class Services
    {
    public:
        static const LoggerPtr& logger() { return mInstance.mLogger; }
        static const FileSystemPtr& fileSystem() { return mInstance.mFileSystem; }
        static const ThreadManagerPtr& threadManager() { return mInstance.mThreadManager; }
        static const RendererPtr& renderer() { return mInstance.mRenderer; }
        static const ResourceManagerPtr& resourceManager() { return mInstance.mResourceManager; }
        static const InputManagerPtr& inputManager() { return mInstance.mInputManager; }

        static void setLogger(const LoggerPtr& instance);
        static void setFileSystem(const FileSystemPtr& instance);
        static void setThreadManager(const ThreadManagerPtr& instance);
        static void setRenderer(const RendererPtr& instance);
        static void setResourceManager(const ResourceManagerPtr& instance);
        static void setInputManager(const InputManagerPtr& instance);

    private:
        static Services mInstance;

        LoggerPtr mLogger;
        FileSystemPtr mFileSystem;
        ThreadManagerPtr mThreadManager;
        RendererPtr mRenderer;
        ResourceManagerPtr mResourceManager;
        InputManagerPtr mInputManager;

        Services();
        ~Services();

        Z_DISABLE_COPY(Services);
    };
}
