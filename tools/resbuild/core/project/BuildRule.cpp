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
#include "BuildRule.h"
#include "../rules/BinaryFileRule.h"
#include <sstream>
#include <unordered_map>
#include <functional>

const std::string BuildRule::NAME_ATTRIBUTE = "name";
const std::string BuildRule::ID_ATTRIBUTE = "id";
const std::string BuildRule::INPUT_FILE_ELEMENT = "InputFile";
const std::string BuildRule::OUTPUT_FILE_ELEMENT = "OutputFile";

BuildRule::BuildRule(uint32_t id, Listener* listener)
    : m_ID(id)
    , m_Listener(listener)
{
    if (m_Listener)
        m_Listener->onRuleCreated(this);
}

BuildRule::~BuildRule()
{
    if (m_Listener)
        m_Listener->onRuleDestroyed(this);
}

void BuildRule::setName(const std::string& name)
{
    if (m_Name != name) {
        m_Name = name;
        notifyListener();
    }
}

std::string BuildRule::settingsHash() const
{
    MD4_CTX context;
    MD4_Init(&context);

    hashString(&context, m_Name);
    hashStringVector(&context, m_InputFiles);
    hashStringVector(&context, m_OutputFiles);

    calcSettingsHash(&context);

    unsigned char buf[16];
    MD4_Final(buf, &context);

    char result[32];
    for (size_t i = 0; i < 16; i++) {
        static const char hex[] = "0123456789abcdef";
        result[i * 2 + 0] = hex[buf[i] >> 4];
        result[i * 2 + 1] = hex[buf[i] & 15];
    }

    return std::string(result, sizeof(result));
}

void BuildRule::load(TiXmlElement* element)
{
    m_Name.clear();
    m_InputFiles.clear();
    m_OutputFiles.clear();

    TiXmlAttribute* idAttribute = element->GetAttribute(ID_ATTRIBUTE);
    if (idAttribute)
        m_ID = uint32_t(std::stoul(idAttribute->ValueStr()));

    TiXmlAttribute* nameAttribute = element->GetAttribute(NAME_ATTRIBUTE);
    if (nameAttribute)
        m_Name = nameAttribute->ValueStr();

    for (TiXmlElement* e = element->FirstChildElement(INPUT_FILE_ELEMENT); e;
        e = e->NextSiblingElement(INPUT_FILE_ELEMENT))
    {
        const char* text = e->GetText();
        if (!text)
            text = "";
        m_InputFiles.push_back(element->GetDocument()->ValueStr() + text);
    }

    for (TiXmlElement* e = element->FirstChildElement(OUTPUT_FILE_ELEMENT); e;
        e = e->NextSiblingElement(OUTPUT_FILE_ELEMENT))
    {
        const char* text = e->GetText();
        if (!text)
            text = "";
        m_OutputFiles.push_back(element->GetDocument()->ValueStr() + text);
    }

    notifyListener();
}

void BuildRule::save(TiXmlElement* element, const std::function<std::string(const std::string&)>& remapFileName)
{
    std::stringstream ss;
    ss << m_ID;
    element->SetAttribute(ID_ATTRIBUTE, ss.str());

    element->SetAttribute(NAME_ATTRIBUTE, m_Name);

    for (const auto& inputFile : m_InputFiles) {
        TiXmlElement* fileElement = new TiXmlElement(INPUT_FILE_ELEMENT);
        element->LinkEndChild(fileElement);

        TiXmlText* text = new TiXmlText(remapFileName(inputFile));
        fileElement->LinkEndChild(text);
    }

    for (const auto& outputFile : m_OutputFiles) {
        TiXmlElement* fileElement = new TiXmlElement(OUTPUT_FILE_ELEMENT);
        element->LinkEndChild(fileElement);

        TiXmlText* text = new TiXmlText(outputFile);
        fileElement->LinkEndChild(text);
    }
}

BuildRulePtr BuildRule::createBuildRule(const std::string& className, uint32_t id, Listener* listener)
{
    #define RULE(X) { X::CLASS_NAME, [](uint32_t i, Listener* l) -> BuildRulePtr { return std::make_shared<X>(i, l); }}

    static const std::unordered_map<std::string, std::function<BuildRulePtr(uint32_t, Listener*)>> rules = {
        RULE(BinaryFileRule)
    };

    auto it = rules.find(className);
    return (it != rules.end() ? it->second(id, listener) : nullptr);
}

void BuildRule::notifyListener()
{
    if (m_Listener)
        m_Listener->onRuleModified(this);
}

void BuildRule::clearInputFiles()
{
    if (!m_InputFiles.empty()) {
        m_InputFiles.clear();
        notifyListener();
    }
}

void BuildRule::addInputFile(const std::string& file)
{
    m_InputFiles.emplace_back(file);
    notifyListener();
}

void BuildRule::clearOutputFiles()
{
    if (!m_OutputFiles.empty()) {
        m_OutputFiles.clear();
        notifyListener();
    }
}

void BuildRule::addOutputFile(const std::string& file)
{
    m_OutputFiles.emplace_back(file);
    notifyListener();
}

void BuildRule::hashSizeT(MD4_CTX* context, size_t value)
{
    hashUInt64T(context, uint64_t(value));
}

void BuildRule::hashUInt64T(MD4_CTX* context, uint64_t value)
{
    MD4_Update(context, &value, sizeof(value));
}

void BuildRule::hashString(MD4_CTX* context, const std::string& string)
{
    hashSizeT(context, string.length());
    MD4_Update(context, string.data(), static_cast<unsigned long>(string.length()));
}

void BuildRule::hashStringVector(MD4_CTX* context, const std::vector<std::string>& vec)
{
    hashSizeT(context, vec.size());
    for (const auto& string : vec)
        hashString(context, string);
}
