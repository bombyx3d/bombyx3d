/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
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
#include <functional>

namespace Engine
{
    /**
     * Unique identifier representing a data type.
     * @see @ref typeOf.
     */
    struct TypeID
    {
    public:
        /** @cond */
        const char* const opaque;
        /** @endcond */

        /**
         * Comparison operator.
         * @param other Value to compare with.
         * @return Comparison result.
         */
        bool operator==(const TypeID& other) const
        {
            return opaque == other.opaque;
        }

        /**
         * Comparison operator.
         * @param other Value to compare with.
         * @return Comparison result.
         */
        bool operator<(const TypeID& other) const
        {
            return opaque < other.opaque;
        }
    };

    /**
     * Retrieves an unique identifier representing the specified data type.
     * @tparam TYPE Data type.
     * @return Unique identifier representing the specified data type.
     */
    template <class TYPE> static TypeID typeOf()
    {
        static char id;
        return TypeID{ &id };
    }
}

/** @cond */
namespace std
{
    template <> struct hash<Engine::TypeID>
    {
        size_t operator()(TypeID value) const
        {
            return hash<void*>(value.opaque);
        }
    }
}
/** @endcond */
