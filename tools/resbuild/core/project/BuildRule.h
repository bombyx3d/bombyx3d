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
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <cstdint>
#include <md4.h>
#include <tinyxml.h>

class BuildManager;

class BuildRule;
using BuildRulePtr = std::shared_ptr<BuildRule>;

class BuildRule
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void onRuleCreated(BuildRule*) {}
        virtual void onRuleModified(BuildRule*) {}
        virtual void onRuleDestroyed(BuildRule*) {}
    };

    static const std::string NAME_ATTRIBUTE;
    static const std::string INPUT_FILE_ELEMENT;
    static const std::string OUTPUT_FILE_ELEMENT;

    explicit BuildRule(Listener* listener = nullptr);
    virtual ~BuildRule();

    virtual const std::string& className() const = 0;

    const std::string& name() const { return m_Name; }
    void setName(const std::string& name);

    const std::vector<std::string>& inputFiles() const { return m_InputFiles; }
    const std::vector<std::string>& outputFiles() const { return m_OutputFiles; }

    std::string settingsHash() const;

    virtual void load(TiXmlElement* element);
    virtual void save(TiXmlElement* element, const std::function<std::string(const std::string&)>& remapFileName);
    
    virtual bool build(BuildManager* buildManager) = 0;

    static BuildRulePtr createBuildRule(const std::string& name, Listener* listener = nullptr);

protected:
    void notifyListener();

    virtual void clearInputFiles();
    virtual void addInputFile(const std::string& file);

    virtual void clearOutputFiles();
    virtual void addOutputFile(const std::string& file);

    virtual void calcSettingsHash(MD4_CTX* context) const = 0;
    static void hashSizeT(MD4_CTX* context, size_t value);
    static void hashUInt64T(MD4_CTX* context, uint64_t value);
    static void hashString(MD4_CTX* context, const std::string& string);
    static void hashStringVector(MD4_CTX* context, const std::vector<std::string>& vec);

private:
    Listener* m_Listener;
    std::string m_Name;
    std::vector<std::string> m_InputFiles;
    std::vector<std::string> m_OutputFiles;

    BuildRule(const BuildRule&) = delete;
    BuildRule& operator=(const BuildRule&) = delete;
};
