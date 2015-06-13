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
#include "core/interfaces/IUnknown.h"
#include "core/interfaces/IFileSystem.h"
#include "core/utility/debug.h"

namespace Engine
{
     /** Interface to the engine core. */
    class ICore : public IUnknown
    {
    public:
        /**
         * Retrieves reference to the instance of the engine core.
         * @return Reference to the instance of the engine code.
         */
        static ICore& instance()
        {
            Z_ASSERT(m_Instance != nullptr);
            return *m_Instance;
        }

        /**
         * Retrieves a reference to the file system.
         * @return Reference to the file system.
         */
        virtual IFileSystem& fileSystem() = 0;

        /** @cond */
        ICore();
        ~ICore();
        _Z_DECLARE_QUERY_INTERFACE_METHODS()
        /** @endcond */

    private:
        static ICore* m_Instance;           /**< Instance of the engine core. */

        Z_DISABLE_COPY(ICore)
    };
}
