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
#include "Match3Field.h"
#include "utility/debug.h"

namespace Z
{
    Match3Field::Match3Field(int width, int height, int8_t numUniqueElements)
        : m_Width(width)
        , m_Height(height)
        , m_NumUniqueElements(numUniqueElements)
    {
        Z_CHECK(numUniqueElements > 0);
        Z_CHECK(width > 0);
        Z_CHECK(height > 0);

        if (m_NumUniqueElements < 1)
            m_NumUniqueElements = 1;
        if (m_Width < 1)
            m_Width = 1;
        if (m_Height < 1)
            m_Height = 1;

        m_Elements = new int8_t[m_Width * m_Height];
        for (int i = 0; i < m_Width * m_Height; i++) {
            m_Elements[i] = i % m_NumUniqueElements; // FIXME
        }
    }

    Match3Field::~Match3Field()
    {
        delete[] m_Elements;
    }

    int8_t& Match3Field::elementAt(int x, int y)
    {
        Z_CHECK(x >= 0);
        Z_CHECK(y >= 0);
        Z_CHECK(x < m_Width);
        Z_CHECK(y < m_Height);

        if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            return m_Elements[x * m_Width + y];

        static int8_t dummy = -1;
        return dummy;
    }

    int8_t Match3Field::elementAt(int x, int y) const
    {
        Z_CHECK(x >= 0);
        Z_CHECK(y >= 0);
        Z_CHECK(x < m_Width);
        Z_CHECK(y < m_Height);

        if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            return m_Elements[x * m_Width + y];

        return -1;
    }

    bool Match3Field::cellWillMatch(int x, int y, int8_t id)
    {
        Z_CHECK(id >= 0);
        if (id < 0)
            return false;

        int l2 = elementAt(x - 2, y);
        int l1 = elementAt(x - 1, y);
        int r1 = elementAt(x + 1, y);
        int r2 = elementAt(x + 2, y);

        int u2 = elementAt(x, y - 2);
        int u1 = elementAt(x, y - 1);
        int d1 = elementAt(x, y + 1);
        int d2 = elementAt(x, y + 2);

        if (l1 == id && l2 == id)
            return true;
        if (l1 == id && r1 == id)
            return true;
        if (r1 == id && r2 == id)
            return true;

        if (u1 == id && u2 == id)
            return true;
        if (u1 == id && d1 == id)
            return true;
        if (d1 == id && d2 == id)
            return true;

        return false;
    }

    void Match3Field::addListener(Match3Listener* listener)
    {
        m_Listeners.insert(listener);
    }

    void Match3Field::removeListener(Match3Listener* listener)
    {
        m_Listeners.erase(listener);
    }
}
