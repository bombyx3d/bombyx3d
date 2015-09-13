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

template <class RULE> struct Action : pegtl::nothing<RULE> {};

#undef ACTION
#define ACTION(RULE, CODE) \
    template <> struct Action<Grammar::RULE> \
    { \
        static void apply(const pegtl::input& input, Context& context) \
        { \
            (void)input; \
            CODE; \
        } \
    }

//////////////////////////////////////////////////////////////////////////////
// Literals

ACTION(BoolFalse, context.boolValues.push_back(false));
ACTION(BoolTrue, context.boolValues.push_back(true));

ACTION(IdentifierText, context.stringValues.emplace_back(input.string()));

ACTION(FloatingPointNumber, context.floatValues.emplace_back(std::stof(input.string())));

ACTION(StringLiteral, {
    std::stringstream result;
    const auto& literal = input.string();
    size_t literalLength = literal.size();
    assert(literalLength >= 2);
    assert(literal[0] == '"');
    assert(literal[literalLength - 1] == '"');
    for (size_t i = 1; i < literalLength - 1; i++) {
        if (literal[i] != '\\')
            result << literal[i];
        else
            result << literal[++i];
    }
    context.stringValues.emplace_back(result.str());
});

//////////////////////////////////////////////////////////////////////////////
// Options

ACTION(CullFaceNone, context.cullFaceValue = CullFace::None);
ACTION(CullFaceFront, context.cullFaceValue = CullFace::Front);
ACTION(CullFaceBack, context.cullFaceValue = CullFace::Back);
ACTION(CullFaceOption, context.emitOption<Tree::CullFaceOption>(context.cullFaceValue));

ACTION(BlendDisabled, context.blendingEnabled = false);
ACTION(BlendFuncValue, context.blendingEnabled = true);
ACTION(BlendFuncZero, context.blendFuncValues.emplace_back(BlendFunc::Zero));
ACTION(BlendFuncOne, context.blendFuncValues.emplace_back(BlendFunc::One));
ACTION(BlendFuncSrcColor, context.blendFuncValues.emplace_back(BlendFunc::SrcColor));
ACTION(BlendFuncSrcAlpha, context.blendFuncValues.emplace_back(BlendFunc::SrcAlpha));
ACTION(BlendFuncSrcAlphaSaturate, context.blendFuncValues.emplace_back(BlendFunc::SrcAlphaSaturate));
ACTION(BlendFuncDstColor, context.blendFuncValues.emplace_back(BlendFunc::DstColor));
ACTION(BlendFuncDstAlpha, context.blendFuncValues.emplace_back(BlendFunc::DstAlpha));
ACTION(BlendFuncOneMinusSrcColor, context.blendFuncValues.emplace_back(BlendFunc::OneMinusSrcColor));
ACTION(BlendFuncOneMinusSrcAlpha, context.blendFuncValues.emplace_back(BlendFunc::OneMinusSrcAlpha));
ACTION(BlendFuncOneMinusDstColor, context.blendFuncValues.emplace_back(BlendFunc::OneMinusDstColor));
ACTION(BlendFuncOneMinusDstAlpha, context.blendFuncValues.emplace_back(BlendFunc::OneMinusDstAlpha));
ACTION(BlendOption, {
    Tree::BlendOptionValue value;
    value.enabled = context.blendingEnabled;
    if (value.enabled) {
        assert(context.blendFuncValues.size() == 2);
        value.dst = pop(context.blendFuncValues);
        value.src = pop(context.blendFuncValues);
        context.blendFuncValues.clear();
    }
    context.emitOption<Tree::BlendOption>(value);
});

ACTION(DepthTestOption, {
    context.emitOption<Tree::DepthTestOption>(pop(context.boolValues));
});

ACTION(DepthWriteOption, {
    context.emitOption<Tree::DepthWriteOption>(pop(context.boolValues));
});

ACTION(ShaderOption, {
    std::string shaderName = pop(context.stringValues);
    std::string fileName = FileUtils::makeFullPath(shaderName, context.materialFileName);
    ShaderPtr shader = Services::resourceManager()->getShader(fileName);
    context.emitOption<Tree::ShaderOption>(shader);
});

//////////////////////////////////////////////////////////////////////////////
// Uniforms

ACTION(UniformFloatValue, {
    float x = pop(context.floatValues);
    context.uniformValue.reset(new Tree::UniformFloat(std::move(x)));
});

ACTION(UniformVec2Value, {
    float y = pop(context.floatValues);
    float x = pop(context.floatValues);
    context.uniformValue.reset(new Tree::UniformVec2(glm::vec2(x, y)));
});

ACTION(UniformVec3Value, {
    float z = pop(context.floatValues);
    float y = pop(context.floatValues);
    float x = pop(context.floatValues);
    context.uniformValue.reset(new Tree::UniformVec3(glm::vec3(x, y, z)));
});

ACTION(UniformVec4Value, {
    float w = pop(context.floatValues);
    float z = pop(context.floatValues);
    float y = pop(context.floatValues);
    float x = pop(context.floatValues);
    context.uniformValue.reset(new Tree::UniformVec4(glm::vec4(x, y, z, w)));
});

ACTION(UniformTextureValue, {
    std::string textureName = pop(context.stringValues);
    std::string fileName = FileUtils::makeFullPath(textureName, context.materialFileName);
    TexturePtr texture = Services::resourceManager()->getTexture(fileName);
    context.uniformValue.reset(new Tree::UniformTexture(std::move(texture)));
});

ACTION(Uniform, {
    std::string name = pop(context.stringValues);
    assert(context.uniformValue != nullptr);
    context.currentOptionList().uniforms.emplace_back(name, std::move(context.uniformValue));
    context.uniformValue.reset();
});

//////////////////////////////////////////////////////////////////////////////
// Passes

ACTION(PassName, context.passName.reset(new std::string(input.string())));
ACTION(NoPassName, context.passName.reset());

ACTION(PassBegin, {
    assert(context.currentPass == nullptr);
    assert(context.currentTechnique != nullptr);
    context.currentPass = std::make_shared<Tree::Pass>();
    context.currentPass->name = std::move(context.passName);
    context.currentTechnique->passes.emplace_back(context.currentPass);
    context.pushOptionList(context.currentPass);
    context.passName.reset();
});

ACTION(PassEnd, {
    assert(context.currentPass != nullptr);
    context.popOptionList(*context.currentPass);
    context.currentPass.reset();
});

//////////////////////////////////////////////////////////////////////////////
// Techniques

ACTION(TechniqueName, context.techniqueName.reset(new std::string(input.string())));
ACTION(NoTechniqueName, context.techniqueName.reset());

ACTION(TechniqueBegin, {
    assert(context.currentTechnique == nullptr);
    context.currentTechnique = std::make_shared<Tree::Technique>();
    context.currentTechnique->name = std::move(context.techniqueName);
    context.currentFile->techniques.emplace_back(context.currentTechnique);
    context.pushOptionList(context.currentTechnique);
    context.techniqueName.reset();
});

ACTION(TechniqueEnd, {
    assert(context.currentTechnique != nullptr);
    context.popOptionList(*context.currentTechnique);
    context.currentTechnique.reset();
});
