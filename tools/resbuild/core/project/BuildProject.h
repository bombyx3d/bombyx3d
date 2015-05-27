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
#include "BuildRule.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>

class BuildProject
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void onProjectModified(BuildProject*) {}
    };

    static const std::string ROOT_ELEMENT;
    static const std::string RULE_ELEMENT;
    static const std::string CLASS_ATTRIBUTE;

    explicit BuildProject(Listener* listener = nullptr);
    ~BuildProject();

    const std::vector<BuildRulePtr>& rules() const { return m_Rules; }

    void addRule(const BuildRulePtr& rule);

    void load(const std::string& file, BuildRule::Listener* ruleListener = nullptr);
    void save(const std::string& file, const std::function<std::string(const std::string&)>& remapFileName);

private:
    Listener* m_Listener;
    std::vector<BuildRulePtr> m_Rules;

    BuildProject(const BuildProject&) = delete;
    BuildProject& operator=(const BuildProject&) = delete;
};

using BuildProjectPtr = std::shared_ptr<BuildProject>;
