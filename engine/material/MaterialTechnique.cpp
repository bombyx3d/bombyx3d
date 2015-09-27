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
#include "MaterialTechnique.h"
#include <cassert>

namespace Engine
{
    static const MaterialPassPtr gNullPass;

    MaterialTechnique::MaterialTechnique(const std::string& techniqueName)
        : mName(techniqueName)
    {
    }

    MaterialTechnique::~MaterialTechnique()
    {
    }

    const std::string& MaterialTechnique::name() const
    {
        return mName;
    }

    size_t MaterialTechnique::numPasses() const
    {
        return mPasses.size();
    }

    const MaterialPassPtr& MaterialTechnique::pass(const std::string& name) const
    {
        for (const auto& pass : mPasses) {
            if (pass->name() == name)
                return pass;
        }
        return gNullPass;
    }

    const MaterialPassPtr& MaterialTechnique::pass(size_t index) const
    {
        assert(index < mPasses.size());
        return (index < mPasses.size() ? mPasses[index] : gNullPass);
    }

    void MaterialTechnique::addPass(const MaterialPassPtr& pass)
    {
        mPasses.emplace_back(pass);
    }

    void MaterialTechnique::addPass(MaterialPassPtr&& pass)
    {
        mPasses.emplace_back(std::move(pass));
    }

    void MaterialTechnique::loadPendingResources(bool async)
    {
        for (const auto& pass : mPasses)
            pass->loadPendingResources(async);
    }
}
