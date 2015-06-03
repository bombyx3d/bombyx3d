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
#include <unordered_set>
#include <cstdint>

namespace Z
{
    class Match3Listener;

    class Match3Field
    {
    public:
        Match3Field(int width, int height, int8_t numUniqueElements);
        ~Match3Field();

        int8_t& elementAt(int x, int y);
        int8_t elementAt(int x, int y) const;

        bool cellWillMatch(int x, int y, int8_t with);

        void addListener(Match3Listener* listener);
        void removeListener(Match3Listener* listener);

    private:
        int m_Width;
        int m_Height;
        int8_t m_NumUniqueElements;
        int8_t* m_Elements;
        std::unordered_set<Match3Listener*> m_Listeners;

        Match3Field(const Match3Field&) = delete;
        Match3Field& operator=(const Match3Field&) = delete;
    };
}
