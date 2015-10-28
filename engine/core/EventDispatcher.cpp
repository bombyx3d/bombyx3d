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
#include "EventDispatcher.h"

namespace B3D
{
    void EventDispatcher::addObserver(TypeID event, IEventObserver* observer)
    {
        mObservers[event].add(observer);
    }

    void EventDispatcher::removeObserver(TypeID event, IEventObserver* observer)
    {
        auto it = mObservers.find(event);
        if (it != mObservers.end())
            it->second.remove(observer);
    }

    void EventDispatcher::removeObserver(IEventObserver* observer)
    {
        for (auto& it : mObservers)
            it.second.remove(observer);
    }

    void EventDispatcher::sendEvent(const IEvent* event)
    {
        auto it = mObservers.find(event->typeID());
        if (it != mObservers.end())
            it->second.forEach([event](IEventObserver* observer){ observer->onEvent(event); });
    }
}
