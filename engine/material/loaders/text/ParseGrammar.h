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


// Comments are introduced by a '//' and proceed to the end-of-line/file.
struct Comment : if_must<string<'/','/'>, until<eolf>> {};


// The parser ignores all spaces and comments; space is a pegtl rule
// that matches the usual ascii characters ' ', '\t', '\n' etc. In other
// words, everything that is space or a comment is ignored.
struct WhitespaceElement : sor<space, Comment> {};
struct Whitespace : plus<WhitespaceElement> {};
struct OptionalWhitespace : star<WhitespaceElement> {};


struct BACK : string<'B','a','c','k'> {};
struct BLEND_FUNC : string<'B','l','e','n','d','F','u','n','c'> {};
struct CULL_FACE : string<'C','u','l','l','F','a','c','e'> {};
struct DISABLED : string<'D','i','s','a','b','l','e','d'> {};
struct DST_ALPHA : string<'D','s','t','A','l','p','h','a'> {};
struct DST_COLOR : string<'D','s','t','C','o','l','o','r'> {};
struct FRONT : string<'F','r','o','n','t'> {};
struct NONE : string<'N','o','n','e'> {};
struct OFF : string<'O','f','f'> {};
struct ON : string<'O','n'> {};
struct ONE : string<'O','n','e'> {};
struct ONE_MINUS_DST_ALPHA : string<'O','n','e','M','i','n','u','s','D','s','t','A','l','p','h','a'> {};
struct ONE_MINUS_DST_COLOR : string<'O','n','e','M','i','n','u','s','D','s','t','C','o','l','o','r'> {};
struct ONE_MINUS_SRC_ALPHA : string<'O','n','e','M','i','n','u','s','S','r','c','A','l','p','h','a'> {};
struct ONE_MINUS_SRC_COLOR : string<'O','n','e','M','i','n','u','s','S','r','c','C','o','l','o','r'> {};
struct PASS : string<'p','a','s','s'> {};
struct SRC_ALPHA : string<'S','r','c','A','l','p','h','a'> {};
struct SRC_ALPHA_SATURATE : string<'S','r','c','A','l','p','h','a','S','a','t','u','r','a','t','e'> {};
struct SRC_COLOR : string<'S','r','c','C','o','l','o','r'> {};
struct TECHNIQUE : string<'t','e','c','h','n','i','q','u','e'> {};
struct ZERO : string<'Z','e','r','o'> {};


struct BoolFalse : OFF {};
struct BoolTrue : ON {};
struct BoolValue : seq<sor<BoolFalse, BoolTrue>, OptionalWhitespace> {};


struct FloatValue : seq<
    opt<one<'-'>>,                      // '-'?
    plus<digit>,                        // [0-9]+
    opt<                                // (
        one<'.'>,                       //   '.'
        plus<digit>                     //   [0-9]+
    >,                                  // )?
    opt<                                // (
        one<'e', 'E'>,                  //   [eE]
        opt<one<'+', '-'>>,             //   [+-]?
        plus<digit>                     //   [0-9]+
    >                                   // )?
> {};


///////////////////////////////
// Options

struct NameValueSeparator : seq<one<':'>, OptionalWhitespace> {};
struct ValueSeparator : seq<one<','>, OptionalWhitespace> {};

struct CullFaceNone : NONE {};
struct CullFaceFront : FRONT {};
struct CullFaceBack : BACK {};
struct CullFaceValue : seq<sor<CullFaceNone, CullFaceFront, CullFaceBack>, OptionalWhitespace> {};
struct CullFaceOption : seq<CULL_FACE, NameValueSeparator, CullFaceValue> {};

struct BlendDisabled : seq<DISABLED, OptionalWhitespace> {};
struct BlendFuncZero : ZERO {};
struct BlendFuncSrcColor : SRC_COLOR {};
struct BlendFuncSrcAlphaSaturate : SRC_ALPHA_SATURATE {};
struct BlendFuncSrcAlpha : SRC_ALPHA {};
struct BlendFuncDstColor : DST_COLOR {};
struct BlendFuncDstAlpha : DST_ALPHA {};
struct BlendFuncOneMinusSrcColor : ONE_MINUS_SRC_COLOR {};
struct BlendFuncOneMinusSrcAlpha : ONE_MINUS_SRC_ALPHA {};
struct BlendFuncOneMinusDstColor : ONE_MINUS_DST_COLOR {};
struct BlendFuncOneMinusDstAlpha : ONE_MINUS_DST_ALPHA {};
struct BlendFuncOne : ONE {};
struct BlendFuncValue : seq<sor<
    BlendFuncZero,
    BlendFuncSrcColor,
    BlendFuncSrcAlphaSaturate,
    BlendFuncSrcAlpha,
    BlendFuncDstColor,
    BlendFuncDstAlpha,
    BlendFuncOneMinusSrcColor,
    BlendFuncOneMinusSrcAlpha,
    BlendFuncOneMinusDstColor,
    BlendFuncOneMinusDstAlpha,
    BlendFuncOne
>, OptionalWhitespace> {};
struct BlendFuncValuePair : seq<BlendFuncValue, ValueSeparator, BlendFuncValue> {};
struct BlendFuncOptionValue : sor<BlendDisabled, BlendFuncValuePair> {};
struct BlendOption : seq<BLEND_FUNC, NameValueSeparator, BlendFuncOptionValue> {};

struct Option : seq<sor<
    CullFaceOption,
    BlendOption
>, OptionalWhitespace> {};


///////////////////////////////
// Passes

struct PassName : identifier {};
struct NoPassName : success {};
struct OptionalPassName : seq<sor<seq<Whitespace, PassName>, NoPassName>, OptionalWhitespace> {};

struct PassBegin : seq<PASS, OptionalPassName, one<'{'>, OptionalWhitespace> {};
struct PassEnd : seq<one<'}'>, OptionalWhitespace> {};

struct Pass : seq<PassBegin, star<Option>, PassEnd> {};


///////////////////////////////
// Techniques

struct TechniqueName : identifier {};
struct NoTechniqueName : success {};
struct OptionalTechniqueName : seq<sor<seq<Whitespace, TechniqueName>, NoTechniqueName>, OptionalWhitespace> {};

struct TechniqueBegin : seq<TECHNIQUE, OptionalTechniqueName, one<'{'>, OptionalWhitespace> {};
struct TechniqueEnd : seq<one<'}'>, OptionalWhitespace> {};

struct Technique : seq<TechniqueBegin, star<sor<Option, Pass>>, TechniqueEnd> {};


///////////////////////////////

// Primary production of the grammar
struct File : must<OptionalWhitespace, star<sor<Option, Technique>>, eof> {};
