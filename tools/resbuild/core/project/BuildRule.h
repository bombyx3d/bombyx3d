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
#include <string>
#include <vector>
#include <cstdint>
#include <md4.h>

class BuildManager;

class BuildRule
{
public:
    BuildRule();
    virtual ~BuildRule();

    const std::string& name() const { return m_Name; }
    void setName(const std::string& name) { m_Name = name; }

    const std::vector<std::string>& inputFiles() const { return m_InputFiles; }
    const std::vector<std::string>& outputFiles() const { return m_OutputFiles; }

    std::string settingsHash() const;

    virtual bool build(BuildManager* buildManager) = 0;

protected:
    virtual void addInputFile(const std::string& file);
    virtual void addOutputFile(const std::string& file);

    virtual void calcSettingsHash(MD4_CTX* context) const = 0;
    static void hashSizeT(MD4_CTX* context, size_t value);
    static void hashUInt64T(MD4_CTX* context, uint64_t value);
    static void hashString(MD4_CTX* context, const std::string& string);
    static void hashStringVector(MD4_CTX* context, const std::vector<std::string>& vec);

private:
    std::string m_Name;
    std::vector<std::string> m_InputFiles;
    std::vector<std::string> m_OutputFiles;
};

typedef std::shared_ptr<BuildRule> BuildRulePtr;
