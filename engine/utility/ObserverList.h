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
#include "engine/utility/ScopedCounter.h"
#include <vector>
#include <cassert>

namespace B3D
{
    template <class TYPE> class ObserverList
    {
    public:
        ObserverList() = default;
        ~ObserverList() = default;

        void add(TYPE* observer)
        {
            assert(observer != nullptr);
            mObservers.push_back(observer);
        }

        void remove(TYPE* observer)
        {
            for (auto& it : mObservers) {
                if (it == observer) {
                    it = nullptr;
                    return;
                }
            }
        }

        template <class CALLBACK> void forEach(CALLBACK&& callback)
        {
            ScopedCounter counter(&mIterating);
            for (auto it = mObservers.begin(); it != mObservers.end(); ) {
                if (*it)
                    callback(*it++);
                else if (mIterating > 1)
                    ++it;
                else
                    it = mObservers.erase(it);
            }
        }

    private:
        std::vector<TYPE*> mObservers;
        int mIterating = 0;

        B3D_DISABLE_COPY(ObserverList);
    };
}
