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

struct Context
{
    CullFace cullFaceValue;
    bool blendingEnabled;
    std::vector<bool> boolValues;
    std::vector<float> floatValues;
    std::vector<BlendFunc> blendFuncValues;
    std::unique_ptr<std::string> techniqueName;
    std::unique_ptr<std::string> passName;
    std::vector<std::shared_ptr<Tree::OptionList>> optionLists;
    std::shared_ptr<Tree::Pass> currentPass;
    std::shared_ptr<Tree::Technique> currentTechnique;
    std::shared_ptr<Tree::File> currentFile;

    Context()
    {
        currentFile = std::make_shared<Tree::File>();
        optionLists.emplace_back(currentFile);
    }

    Tree::OptionList& currentOptionList()
    {
        assert(!optionLists.empty());
        return *optionLists.back();
    }

    template <class TYPE, class... ARGS> void emitOption(ARGS&&... args)
    {
        currentOptionList().options.emplace_back(std::make_shared<TYPE>(std::forward<ARGS>(args)...));
    }

    void pushOptionList(std::shared_ptr<Tree::OptionList>&& list)
    {
        optionLists.emplace_back(list);
    }

    void popOptionList(Tree::OptionList& current)
    {
        assert(!optionLists.empty());
        assert(optionLists.back().get() == &current);
        optionLists.pop_back();
    }
};

template <class TYPE> TYPE pop(std::vector<TYPE>& array)
{
    assert(!array.empty());
    TYPE value(std::move(array.back()));
    array.pop_back();
    return value;
}
