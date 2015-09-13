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
#include <mutex>
#include <unordered_map>
#include <memory>
#include <string>

namespace Engine
{
    class Atom;

    class AtomTable
    {
    public:
        static Atom getAtom(std::string&& text);
        static Atom getAtom(const std::string& text);

    private:
        struct HashFunction
        {
            size_t operator()(const std::string* item) const;
        };

        struct Comparator
        {
            bool operator()(const std::string* item1, const std::string* item2) const;
        };

        struct Entry
        {
            size_t uniqueID;
            std::string text;

            Entry();
            Entry(size_t i, const std::string& s);
            Entry(size_t i, std::string&& s);
        };

        static const Entry mNullEntry;
        static AtomTable mInstance;

        std::unordered_map<const std::string*, std::unique_ptr<Entry>, HashFunction, Comparator> mMap;
        std::mutex mMutex;

        AtomTable();
        ~AtomTable();

        Z_DISABLE_COPY(AtomTable);
        friend class Atom;
    };
}
