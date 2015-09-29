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
#include "engine/utility/FileUtils.h"
#include <iterator>
#include <string>
#include <tinyxml.h>

namespace B3D
{
    namespace XmlUtils
    {
        class ParseError {};

        class ElementIterator
        {
        public:
            ElementIterator() : mElement(nullptr) {}
            explicit ElementIterator(const TiXmlElement* e) : mElement(e) {}

            const TiXmlElement* operator*() const { assert(mElement != nullptr); return mElement; }

            ElementIterator& operator++();

            bool operator==(const ElementIterator& other) const { return mElement == other.mElement; }
            bool operator!=(const ElementIterator& other) const { return mElement != other.mElement; }

        private:
            const TiXmlElement* mElement;
        };

        TiXmlDocument parseFile(IFile* file);
        TiXmlDocument parseFile(const FilePtr& file);
        TiXmlDocument parseFile(const std::string& fileName);

        std::string locationOf(const TiXmlBase* node);

        void assertNotNull(const void* pointer);
        void assertTagNameEquals(const TiXmlElement* element, const std::string& name);

        const TiXmlAttribute* getAttribute(const TiXmlElement* element, const char* name);
        std::string getStringAttribute(const TiXmlElement* element, const char* name);
        std::string getStringAttribute(const TiXmlElement* element, const char* name, const std::string& def);
        int getIntAttribute(const TiXmlElement* element, const char* name);
        int getIntAttribute(const TiXmlElement* element, const char* name, int def);
        float getFloatAttribute(const TiXmlElement* element, const char* name);
        float getFloatAttribute(const TiXmlElement* element, const char* name, float def);
    }
}

B3D::XmlUtils::ElementIterator begin(const TiXmlElement* element);
B3D::XmlUtils::ElementIterator end(const TiXmlElement*);
