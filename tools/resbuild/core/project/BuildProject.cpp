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
#include "BuildProject.h"
#include <tinyxml.h>
#include <sys/stat.h>
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cerrno>

const std::string BuildProject::ROOT_ELEMENT = "ResBuildProject";
const std::string BuildProject::RULE_ELEMENT = "Rule";
const std::string BuildProject::CLASS_ATTRIBUTE = "class";

BuildProject::BuildProject(Listener* listener)
    : m_Listener(listener)
{
}

BuildProject::~BuildProject()
{
}

void BuildProject::addRule(const BuildRulePtr& rule)
{
    m_Rules.emplace_back(rule);
    if (m_Listener)
        m_Listener->onProjectModified(this);
}

void BuildProject::load(const std::string& file, BuildRule::Listener* ruleListener)
{
    std::string path = file;
    size_t index = path.rfind('/');
  #ifdef _WIN32
    size_t index2 = path.rfind('\\');
    if (index2 != std::string::npos && (index == std::string::npos || index2 > index))
        index = index2;
  #endif
    if (index != std::string::npos)
        path.resize(index + 1);

    TiXmlDocument doc(path);
    if (!doc.LoadFile(file)) {
        std::stringstream ss;
        ss << "Unable to load XML file \"" << file << "\": at line " << doc.ErrorRow() << ", column "
            << doc.ErrorCol() << ": " << doc.ErrorDesc();
        throw new std::runtime_error(ss.str());
    }

    TiXmlElement* rootElement = doc.RootElement();
    if (!rootElement) {
        std::stringstream ss;
        ss << "Unable to load XML file \"" << file << "\": missing root element.";
        throw new std::runtime_error(ss.str());
    }

    if (rootElement->ValueStr() != ROOT_ELEMENT) {
        std::stringstream ss;
        ss << "Unable to load XML file \"" << file << "\": at line " << rootElement->Row() << ", column "
            << rootElement->Column() << ": invalid root element.";
        throw new std::runtime_error(ss.str());
    }

    m_Rules.clear();
    for (TiXmlElement* element = rootElement->FirstChildElement(RULE_ELEMENT);
        element != nullptr; element = element->NextSiblingElement(RULE_ELEMENT))
    {
        TiXmlAttribute* classAttribute = element->GetAttribute(CLASS_ATTRIBUTE);
        if (!classAttribute) {
            std::stringstream ss;
            ss << "Unable to load XML file \"" << file << "\": at line " << rootElement->Row() << ", column "
                << rootElement->Column() << ": missing attribute \"" << CLASS_ATTRIBUTE << "\".";
            throw new std::runtime_error(ss.str());
        }

        BuildRulePtr rule = BuildRule::createBuildRule(classAttribute->ValueStr(), ruleListener);
        if (!rule) {
            std::stringstream ss;
            ss << "Unable to load XML file \"" << file << "\": at line " << rootElement->Row() << ", column "
                << rootElement->Column() << ": unknown class \"" << classAttribute->ValueStr() << "\".";
            throw new std::runtime_error(ss.str());
        }

        rule->load(element);
        m_Rules.emplace_back(std::move(rule));
    }
}

void BuildProject::save(const std::string& file, const std::function<std::string(const std::string&)>& remapFileName)
{
    TiXmlDocument doc;

    doc.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));

    TiXmlElement* rootElement = new TiXmlElement(ROOT_ELEMENT);
    doc.LinkEndChild(rootElement);

    for (const auto& rule : m_Rules) {
        TiXmlElement* ruleElement = new TiXmlElement(RULE_ELEMENT);
        rootElement->LinkEndChild(ruleElement);

        ruleElement->SetAttribute(CLASS_ATTRIBUTE, rule->className());
        rule->save(ruleElement, remapFileName);
    }

    struct stat st;
    bool oldFileExists = stat(file.c_str(), &st) == 0;

    if (!oldFileExists) {
        if (!doc.SaveFile(file)) {
            std::stringstream ss;
            ss << "Unable to write file \"" << file << "\".";
            throw std::runtime_error(ss.str());
        }
    } else {
        std::string backupFile = file + "~";
        remove(backupFile.c_str());

        if (rename(file.c_str(), backupFile.c_str()) < 0) {
            int err = errno;
            std::stringstream ss;
            ss << "Unable to rename file \"" << file << "\" to \"" << backupFile << "\".";
            throw std::runtime_error(ss.str());
        }   

        if (!doc.SaveFile(file)) {
            std::stringstream ss;
            ss << "Unable to write file \"" << file << "\" (old file was renamed to \"" << backupFile << "\").";
            throw std::runtime_error(ss.str());
        }
    }
}
