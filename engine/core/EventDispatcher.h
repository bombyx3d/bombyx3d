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
#include "engine/utility/ObserverList.h"
#include "engine/utility/TypeID.h"
#include "engine/interfaces/core/IEventDispatcher.h"
#include <unordered_map>

namespace B3D
{
    class EventDispatcher : public IEventDispatcher
    {
    public:
        EventDispatcher() = default;

        template <class TYPE> void addObserver(IEventObserver* observer) { addObserver(typeOf<TYPE>(), observer); }
        void addObserver(TypeID event, IEventObserver* observer) override;
        void removeObserver(TypeID event, IEventObserver* observer) override;
        void removeObserver(IEventObserver* observer) override;

        void sendEvent(const IEvent* event) override;

    private:
        std::unordered_map<TypeID, ObserverList<IEventObserver>> mObservers;

        B3D_DISABLE_COPY(EventDispatcher);
    };
}
