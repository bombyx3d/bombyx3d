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
#include "core/interfaces/IFileReader.h"
#include <string>
#include <vector>

namespace Engine
{
    /** An `std::vector`-based implementation of @ref Engine::IFileReader. */
    class MemoryFile : public IFileReader
    {
    public:
        /**
         * Constructor.
         * @param fileName File name.
         */
        explicit MemoryFile(const std::string& fileName = "<memory>");

        /**
         * Constructor.
         * @param fileData File data.
         * @param fileName File name.
         */
        explicit MemoryFile(std::vector<char>&& fileData, const std::string& fileName = "<memory>");

        /** Destructor. */
        ~MemoryFile();

        /**
         * Retrieves reference to data.
         * @return Reference to data.
         */
        std::vector<char>& data()
        {
            return m_Data;
        }

        /**
         * Retrieves reference to data.
         * @return Reference to data.
         */
        const std::vector<char>& data() const
        {
            return m_Data;
        }

        /**
         * Retrieves reference to data.
         * @return Reference to data.
         */
        const std::vector<char>& constData() const
        {
            return m_Data;
        }

        /** @cond */
        const std::string& name() const override;
        uint64_t size() const override;
        bool read(uint64_t offset, void* buffer, size_t bytesToRead) override;
        void* queryInterface(TypeID typeID) override;
        /** @endcond */

    private:
        std::string m_Name;             /**< File name. */
        std::vector<char> m_Data;       /**< File data. */
    };
}
