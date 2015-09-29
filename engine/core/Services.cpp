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
#include "Services.h"

namespace B3D
{
    Services Services::mInstance;

    Services::Services()
    {
    }

    Services::~Services()
    {
    }

    void Services::setLogger(const LoggerPtr& instance)
    {
        mInstance.mLogger = instance;
    }

    void Services::setFileSystem(const FileSystemPtr& instance)
    {
        mInstance.mFileSystem = instance;
    }

    void Services::setThreadManager(const ThreadManagerPtr& instance)
    {
        mInstance.mThreadManager = instance;
    }

    void Services::setRendererResourceFactory(const RendererResourceFactoryPtr& instance)
    {
        mInstance.mRendererResFactory = instance;
    }

    void Services::setResourceManager(const ResourceManagerPtr& instance)
    {
        mInstance.mResourceManager = instance;
    }

    void Services::setInputManager(const InputManagerPtr& instance)
    {
        mInstance.mInputManager = instance;
    }

    void Services::setSceneManager(const SceneManagerPtr& instance)
    {
        mInstance.mSceneManager = instance;
    }
}
