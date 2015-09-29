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
#include "XmlUtils.h"
#include "engine/core/Services.h"
#include "engine/core/Log.h"
#include <sstream>
#include <exception>
#include <cassert>

namespace Engine
{
    XmlUtils::ElementIterator& XmlUtils::ElementIterator::operator++()
    {
        mElement = mElement ? mElement->NextSiblingElement() : nullptr;
        return *this;
    }

    TiXmlDocument XmlUtils::parseFile(IFile* file)
    {
        auto bytes = FileUtils::loadFile(file);
        bytes.push_back(0);

        TiXmlDocument doc(file->name());
        if (!doc.LoadBuffer(bytes.data(), bytes.size() - 1)) {
            Z_LOGE("Unable to parse XML document \"" << doc.ValueStr() <<
                "\": at line " << doc.ErrorRow() << ", column " << doc.ErrorCol() << ": " << doc.ErrorDesc());
            throw ParseError();
        }

        return doc;
    }

    TiXmlDocument XmlUtils::parseFile(const FilePtr& file)
    {
        return parseFile(file.get());
    }

    TiXmlDocument XmlUtils::parseFile(const std::string& fileName)
    {
        return parseFile(Services::fileSystem()->openFile(fileName));
    }

    std::string XmlUtils::locationOf(const TiXmlBase* node)
    {
        if (!node)
            return std::string();

        std::stringstream ss;
        ss << "Unable to parse XML document \"" << node->GetDocument()->ValueStr()
            << "\": at line " << node->Row() << ", column " << node->Column() << ": ";
        return ss.str();
    }

    void XmlUtils::assertNotNull(const void* pointer)
    {
        if (!pointer) {
            Z_LOGE("Unable to parse XML document: NULL pointer.");
            throw ParseError();
        }
    }

    void XmlUtils::assertTagNameEquals(const TiXmlElement* element, const std::string& name)
    {
        assertNotNull(element);
        if (element->ValueStr() != name) {
            Z_LOGE(locationOf(element) << "Invalid tag name (expected \"" << name
                << "\" but found \"" << element->ValueStr() << "\").");
            throw ParseError();
        }
    }

    const TiXmlAttribute* XmlUtils::getAttribute(const TiXmlElement* element, const char* name)
    {
        assertNotNull(element);

        TiXmlAttribute* attribute = element->GetAttribute(name);
        if (attribute == nullptr) {
            Z_LOGE(locationOf(element) << "Attribute \"" << name << "\" is missing in tag \""
                << element->ValueStr() << "\".");
            throw ParseError();
        }

        return attribute;
    }

    std::string XmlUtils::getStringAttribute(const TiXmlElement* element, const char* name)
    {
        return getAttribute(element, name)->ValueStr();
    }

    std::string XmlUtils::getStringAttribute(const TiXmlElement* element, const char* name, const std::string& def)
    {
        assertNotNull(element);
        TiXmlAttribute* attribute = element->GetAttribute(name);
        return (attribute ? attribute->ValueStr() : def);
    }

    int XmlUtils::getIntAttribute(const TiXmlElement* element, const char* name)
    {
        TiXmlAttribute* attribute = element->GetAttribute(name);
        if (attribute == nullptr) {
            Z_LOGE(locationOf(element) << "Attribute \"" << name << "\" is missing in tag \""
                << element->ValueStr() << "\".");
            throw ParseError();
        }

        const std::string& string = attribute->ValueStr();
        try {
            size_t pos = 0;
            int value = std::stoi(string, &pos);
            if (pos == string.length())
                return value;
        } catch (const std::exception& e) {
            Z_LOGE(locationOf(element) << "Value of attribute \"" << name
                << "\" is not a valid integer (" << e.what() << ").");
            throw ParseError();
        }

        Z_LOGE(locationOf(element) << "Value of attribute \"" << name << "\" is not a valid integer.");
        throw ParseError();
    }

    int XmlUtils::getIntAttribute(const TiXmlElement* element, const char* name, int def)
    {
        TiXmlAttribute* attribute = element->GetAttribute(name);
        if (attribute == nullptr)
            return def;

        const std::string& string = attribute->ValueStr();
        try {
            size_t pos = 0;
            int value = std::stoi(string, &pos);
            if (pos == string.length())
                return value;
        } catch (const std::exception& e) {
            Z_LOGE(locationOf(element) << "Value of attribute \"" << name
                << "\" is not a valid integer (" << e.what() << ").");
            throw ParseError();
        }

        Z_LOGE(locationOf(element) << "Value of attribute \"" << name << "\" is not a valid integer.");
        throw ParseError();
    }

    float XmlUtils::getFloatAttribute(const TiXmlElement* element, const char* name)
    {
        TiXmlAttribute* attribute = element->GetAttribute(name);
        if (attribute == nullptr) {
            Z_LOGE(locationOf(element) << "Attribute \"" << name << "\" is missing in tag \""
                << element->ValueStr() << "\".");
            throw ParseError();
        }

        const std::string& string = attribute->ValueStr();
        try {
            size_t pos = 0;
            float value = std::stof(string, &pos);
            if (pos == string.length())
                return value;
        } catch (const std::exception& e) {
            Z_LOGE(locationOf(element) << "Value of attribute \"" << name
                << "\" is not a valid floating-point number (" << e.what() << ").");
            throw ParseError();
        }

        Z_LOGE(locationOf(element) << "Value of attribute \"" << name << "\" is not a valid floating-point number.");
        throw ParseError();
    }

    float XmlUtils::getFloatAttribute(const TiXmlElement* element, const char* name, float def)
    {
        TiXmlAttribute* attribute = element->GetAttribute(name);
        if (attribute == nullptr)
            return def;

        const std::string& string = attribute->ValueStr();
        try {
            size_t pos = 0;
            float value = std::stof(string, &pos);
            if (pos == string.length())
                return value;
        } catch (const std::exception& e) {
            Z_LOGE(locationOf(element) << "Value of attribute \"" << name
                << "\" is not a valid floating-point number (" << e.what() << ").");
            throw ParseError();
        }

         Z_LOGE(locationOf(element) << "Value of attribute \"" << name << "\" is not a valid floating-point number.");
         throw ParseError();
    }
}

Engine::XmlUtils::ElementIterator begin(const TiXmlElement* element)
{
    return Engine::XmlUtils::ElementIterator(element ? element->FirstChildElement() : nullptr);
}

Engine::XmlUtils::ElementIterator end(const TiXmlElement*)
{
    return Engine::XmlUtils::ElementIterator();
}
