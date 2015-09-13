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

struct Option
{
    enum Name
    {
        CullFace,
        Blend,
    };

    virtual ~Option() = default;
    virtual void applyToPass(MaterialPass& pass) const = 0;
};

template <Option::Name NAME, class TYPE> struct OptionValue : Option
{
    const TYPE value;
    OptionValue(TYPE v) : value(v) {}
    void applyToPass(MaterialPass& pass) const override;
};

using CullFaceOption = OptionValue<Option::CullFace, CullFace>;
template<> void CullFaceOption::applyToPass(MaterialPass& pass) const
{
    pass.setCullFace(value);
}

struct BlendOptionValue
{
    bool enabled;
    BlendFunc src;
    BlendFunc dst;
};
using BlendOption = OptionValue<Option::Blend, BlendOptionValue>;
template<> void BlendOption::applyToPass(MaterialPass& pass) const
{
    pass.setBlendingEnabled(value.enabled);
    pass.setBlendingSourceFactor(value.src);
    pass.setBlendingDestinationFactor(value.dst);
}

struct OptionList
{
    std::vector<std::shared_ptr<Option>> options;
};

struct Pass : public OptionList
{
    std::unique_ptr<std::string> name;
};

struct Technique : public OptionList
{
    std::unique_ptr<std::string> name;
    std::vector<std::shared_ptr<Pass>> passes;
};

struct File : public OptionList
{
    std::vector<std::shared_ptr<Technique>> techniques;
};
