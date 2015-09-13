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
#include "engine/core/AtomTable.h"
#include <functional>

namespace Engine
{
    class Atom
    {
    public:
        Atom() : mAtomTableEntry(&AtomTable::mNullEntry) {}
        Atom(const Atom& src) : mAtomTableEntry(src.mAtomTableEntry) {}
        Atom(Atom&& src) : mAtomTableEntry(std::move(src.mAtomTableEntry)) {}
        ~Atom() = default;

        Atom& operator=(const Atom& src) { mAtomTableEntry = src.mAtomTableEntry; return *this; }
        Atom& operator=(Atom&& src) { mAtomTableEntry = std::move(src.mAtomTableEntry); return *this; }

        size_t uniqueID() const { return mAtomTableEntry->uniqueID; }
        const std::string& text() const { return mAtomTableEntry->text; }

        bool operator==(const Atom& other) const { return mAtomTableEntry == other.mAtomTableEntry; }
        bool operator!=(const Atom& other) const { return mAtomTableEntry != other.mAtomTableEntry; }
        bool operator<(const Atom& other) const { return mAtomTableEntry->uniqueID < other.mAtomTableEntry->uniqueID; }

    private:
        const AtomTable::Entry* mAtomTableEntry;

        explicit Atom(const AtomTable::Entry* entry) : mAtomTableEntry(entry) {}

        friend class AtomTable;
    };
}

namespace std {
    template <> struct hash<Engine::Atom> {
        size_t operator()(const Engine::Atom& atom) const {
            return std::hash<size_t>()(atom.uniqueID());
        }
    };
}
