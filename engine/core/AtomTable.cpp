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
#include "AtomTable.h"
#include "engine/core/Atom.h"

namespace Engine
{
    size_t AtomTable::HashFunction::operator()(const std::string* item) const
    {
        return std::hash<std::string>()(*item);
    }

    bool AtomTable::Comparator::operator()(const std::string* item1, const std::string* item2) const
    {
        return *item1 == *item2;
    }


    AtomTable::Entry::Entry()
        : id(0)
    {
    }

    AtomTable::Entry::Entry(size_t i, const std::string& s)
        : id(i)
        , text(s)
    {
    }

    AtomTable::Entry::Entry(size_t i, std::string&& s)
        : id(i)
        , text(std::move(s))
    {
    }


    const AtomTable::Entry AtomTable::mNullEntry;
    AtomTable AtomTable::mInstance;

    AtomTable::AtomTable()
    {
    }

    AtomTable::~AtomTable()
    {
    }

    Atom AtomTable::getAtom(std::string&& text)
    {
        std::lock_guard<decltype(mMutex)> lock(mMutex);

        auto it = mMap.find(&text);
        if (it != mMap.end())
            return Atom(it->second.get());

        Entry* entry = new Entry(mMap.size() + 1, std::move(text));
        mMap.emplace(&entry->text, std::unique_ptr<Entry>(entry));

        return Atom(entry);
    }

    Atom AtomTable::getAtom(const std::string& text)
    {
        std::lock_guard<decltype(mMutex)> lock(mMutex);

        auto it = mMap.find(&text);
        if (it != mMap.end())
            return Atom(it->second.get());

        Entry* entry = new Entry(mMap.size() + 1, text);
        mMap.emplace(&entry->text, std::unique_ptr<Entry>(entry));

        return Atom(entry);
    }
}
