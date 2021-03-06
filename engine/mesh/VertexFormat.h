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

#pragma once
#include "engine/interfaces/render/lowlevel/IVertexSource.h"
#include <exception>
#include <cstddef>
#include <glm/glm.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/stringize.hpp>

namespace B3D
{
    // Descriptor for a single attribute in a vertex format

    template <typename TYPE = void> struct VertexFormatAttribute;
    template <> struct VertexFormatAttribute<>
    {
        const char* const name;
        const VertexAttributeType type;
        const size_t offset;
        const bool normalize;

        VertexFormatAttribute(const char* n, VertexAttributeType t, size_t o, bool norm)
            : name(n)
            , type(t)
            , offset(o)
            , normalize(norm)
        {}
    };

    #define B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(CXX_TYPE, ATTRIBUTE_TYPE) \
        template <> struct VertexFormatAttribute<CXX_TYPE> : VertexFormatAttribute<> { \
            explicit VertexFormatAttribute(const char* n, size_t o) \
                : VertexFormatAttribute<>(n, VertexAttributeType::ATTRIBUTE_TYPE, o, false) \
            {} \
        }

    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(float, Float);
    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(float[2], Float2);
    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(float[3], Float3);
    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(float[4], Float4);
    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(glm::vec2, Float2);
    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(glm::vec3, Float3);
    B3D_DECLARE_VERTEX_ATTRIBUTE_TYPE(glm::vec4, Float4);


    // List of attributes

    class IVertexFormatAttributeList
    {
    public:
        virtual ~IVertexFormatAttributeList() = default;
        virtual size_t stride() const = 0;
        virtual size_t attributeCount() const = 0;
        virtual const VertexFormatAttribute<>& attribute(size_t index) const = 0;
    };

    template <typename TYPE> class VertexFormatAttributeList : public IVertexFormatAttributeList
    {
    public:
        size_t stride() const override { return sizeof(TYPE); }
        size_t attributeCount() const override { return TYPE::ATTRIBUTE_COUNT; }
        const VertexFormatAttribute<>& attribute(size_t index) const override { return TYPE::attribute(index); }
    };


    // The code below is based on code from:
    // http://stackoverflow.com/questions/11031062/c-preprocessor-avoid-code-repetition-of-member-variable-list/11744832#11744832
    // ---------------------------------------------------------------------------------------------------------------
    //
    // Vertex format should be declared as:
    //
    // B3D_VERTEX_FORMAT(MyVertexFormat,
    //    (float[3]) position,
    //    (float[2]) texCoord
    // );
    //
    // We use preprocessor magic to split strings "(TYPE) NAME" into type and name.
    //

    // This macro is used to remove brackets from string "(TYPE) NAME" effectively converting it into "TYPE NAME"
    #define B3D_VERTEX_FORMAT_ELEMENT_STRIP_BRACKETS(...) __VA_ARGS__
    // This macro is used to remove first element from string "(TYPE) NAME" effectively converting it into "NAME"
    #define B3D_VERTEX_FORMAT_ELEMENT_CONSUME_1ST(...)

    // This macro extracts "NAME" from string "(TYPE) NAME"
    #define B3D_VERTEX_FORMAT_ELEMENT_NAME(x) B3D_VERTEX_FORMAT_ELEMENT_CONSUME_1ST x

    // This macro and it's helper friends extracts "TYPE" from string "(TYPE) NAME"
    #define B3D_VERTEX_FORMAT_ELEMENT_TYPE(x) B3D_VERTEX_FORMAT_ELEMENT_TYPE_DETAIL(B3D_VERTEX_FORMAT_ELEMENT_TYPE_PROBE x,)
    #define B3D_VERTEX_FORMAT_ELEMENT_TYPE_DETAIL(...) B3D_VERTEX_FORMAT_ELEMENT_TYPE_HEAD(__VA_ARGS__)
    #ifndef _MSC_VER
     #define B3D_VERTEX_FORMAT_ELEMENT_TYPE_HEAD(x, ...) B3D_VERTEX_FORMAT_ELEMENT_STRIP_BRACKETS x
     #define B3D_VERTEX_FORMAT_ELEMENT_TYPE_PROBE(...) (__VA_ARGS__),
    #else
     #define B3D_VERTEX_FORMAT_ELEMENT_TYPE_HEAD(x, ...) x
     #define B3D_VERTEX_FORMAT_ELEMENT_TYPE_PROBE(...) (__VA_ARGS__) B3D_VERTEX_FORMAT_ELEMENT_NONE
     #define B3D_VERTEX_FORMAT_ELEMENT_NONE(x)
    #endif

    // This template is used to translate declarations like `float[2]` into valid C++ type.
    template <typename TYPE> struct VertexAttributeTypeResolver { using type = TYPE; };

    // This macro declares a struct field
    #define B3D_VERTEX_FORMAT_ELEMENT_DECLARATION(_, __, INDEX, E) \
        ::B3D::VertexAttributeTypeResolver<B3D_VERTEX_FORMAT_ELEMENT_TYPE(E)>::type \
            B3D_VERTEX_FORMAT_ELEMENT_NAME(E);

    // This macro defines a descriptor for a single attribute
    #define B3D_VERTEX_FORMAT_ELEMENT_DESCRIPTOR(_, __, INDEX, E) \
        case (INDEX): { \
            using Type = ::B3D::VertexAttributeTypeResolver<B3D_VERTEX_FORMAT_ELEMENT_TYPE(E)>::type; \
            const char* const name = BOOST_PP_STRINGIZE(B3D_VERTEX_FORMAT_ELEMENT_NAME(E)); \
            const size_t offset = offsetof(Self, B3D_VERTEX_FORMAT_ELEMENT_NAME(E)); \
            static const ::B3D::VertexFormatAttribute<Type> attribute(name, offset); \
            return attribute; \
        }

    #define B3D_VERTEX_FORMAT(NAME, ...) \
        struct NAME { \
            static const size_t ATTRIBUTE_COUNT = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
            \
            /* Declare struct fields for attributes. */ \
            BOOST_PP_SEQ_FOR_EACH_I(B3D_VERTEX_FORMAT_ELEMENT_DECLARATION, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
            \
            static const ::B3D::VertexFormatAttribute<>& attribute(size_t index) { \
                using Self = NAME; \
                switch (index) { \
                BOOST_PP_SEQ_FOR_EACH_I(B3D_VERTEX_FORMAT_ELEMENT_DESCRIPTOR, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
                } \
                /* We get here if attribute index is out of range. */ \
                std::terminate(); \
            } \
            \
            static const ::B3D::IVertexFormatAttributeList& attributes() { \
                static B3D::VertexFormatAttributeList<NAME> list; \
                return list; \
            } \
        };
}
