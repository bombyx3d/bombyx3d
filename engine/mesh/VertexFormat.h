
#pragma once
#include "engine/interfaces/render/IVertexSource.h"
#include <exception>
#include <cstddef>
#include <glm/glm.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/stringize.hpp>

namespace Engine
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

    #define Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(CXX_TYPE, ATTRIBUTE_TYPE) \
        template <> struct VertexFormatAttribute<CXX_TYPE> : VertexFormatAttribute<> { \
            explicit VertexFormatAttribute(const char* n, size_t o) \
                : VertexFormatAttribute<>(n, VertexAttributeType::ATTRIBUTE_TYPE, o, false) \
            {} \
        }

    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(float, Float);
    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(float[2], Float2);
    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(float[3], Float3);
    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(float[4], Float4);
    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(glm::vec2, Float2);
    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(glm::vec3, Float3);
    Z_DECLARE_VERTEX_ATTRIBUTE_TYPE(glm::vec4, Float4);


    // The code below is based on code from:
    // http://stackoverflow.com/questions/11031062/c-preprocessor-avoid-code-repetition-of-member-variable-list/11744832#11744832
    // ---------------------------------------------------------------------------------------------------------------
    //
    // Vertex format should be declared as:
    //
    // Z_VERTEX_FORMAT(MyVertexFormat,
    //    (float[3]) position,
    //    (float[2]) texCoord
    // );
    //
    // We use preprocessor magic to split strings "(TYPE) NAME" into type and name.
    //

    // This macro is used to remove brackets from string "(TYPE) NAME" effectively converting it into "TYPE NAME"
    #define Z_VERTEX_FORMAT_ELEMENT_STRIP_BRACKETS(...) __VA_ARGS__
    // This macro is used to remove first element from string "(TYPE) NAME" effectively converting it into "NAME"
    #define Z_VERTEX_FORMAT_ELEMENT_CONSUME_1ST(...)

    // This macro extracts "NAME" from string "(TYPE) NAME"
    #define Z_VERTEX_FORMAT_ELEMENT_NAME(x) Z_VERTEX_FORMAT_ELEMENT_CONSUME_1ST x

    // This macro and it's helper friends extracts "TYPE" from string "(TYPE) NAME"
    #define Z_VERTEX_FORMAT_ELEMENT_TYPE(x) Z_VERTEX_FORMAT_ELEMENT_TYPE_DETAIL(Z_VERTEX_FORMAT_ELEMENT_TYPE_PROBE x,)
    #define Z_VERTEX_FORMAT_ELEMENT_TYPE_DETAIL(...) Z_VERTEX_FORMAT_ELEMENT_TYPE_HEAD(__VA_ARGS__)
    #ifndef _MSC_VER
     #define Z_VERTEX_FORMAT_ELEMENT_TYPE_HEAD(x, ...) Z_VERTEX_FORMAT_ELEMENT_STRIP_BRACKETS x
     #define Z_VERTEX_FORMAT_ELEMENT_TYPE_PROBE(...) (__VA_ARGS__),
    #else
     #define Z_VERTEX_FORMAT_ELEMENT_TYPE_HEAD(x, ...) x
     #define Z_VERTEX_FORMAT_ELEMENT_TYPE_PROBE(...) (__VA_ARGS__) Z_VERTEX_FORMAT_ELEMENT_NONE
     #define Z_VERTEX_FORMAT_ELEMENT_NONE(x)
    #endif

    // This template is used to translate declarations like `float[2]` into valid C++ type.
    template <typename TYPE> struct VertexAttributeTypeResolver { using type = TYPE; };

    // This macro declares a struct field
    #define Z_VERTEX_FORMAT_ELEMENT_DECLARATION(_, __, INDEX, E) \
        ::Engine::VertexAttributeTypeResolver<Z_VERTEX_FORMAT_ELEMENT_TYPE(E)>::type \
            Z_VERTEX_FORMAT_ELEMENT_NAME(E);

    // This macro defines a descriptor for a single attribute
    #define Z_VERTEX_FORMAT_ELEMENT_DESCRIPTOR(_, __, INDEX, E) \
        case (INDEX): { \
            using Type = ::Engine::VertexAttributeTypeResolver<Z_VERTEX_FORMAT_ELEMENT_TYPE(E)>::type; \
            const char* const name = BOOST_PP_STRINGIZE(Z_VERTEX_FORMAT_ELEMENT_NAME(E)); \
            const size_t offset = offsetof(Self, Z_VERTEX_FORMAT_ELEMENT_NAME(E)); \
            static const ::Engine::VertexFormatAttribute<Type> attribute(name, offset); \
            return attribute; \
        }

    #define Z_VERTEX_FORMAT(NAME, ...) \
        struct NAME { \
            static const size_t ATTRIBUTE_COUNT = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
            \
            /* Declare struct fields for attributes. */ \
            BOOST_PP_SEQ_FOR_EACH_I(Z_VERTEX_FORMAT_ELEMENT_DECLARATION, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
            \
            static const ::Engine::VertexFormatAttribute<>& attribute(size_t index) { \
                using Self = NAME; \
                switch (index) { \
                BOOST_PP_SEQ_FOR_EACH_I(Z_VERTEX_FORMAT_ELEMENT_DESCRIPTOR, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
                } \
                /* We get here if attribute index is out of range. */ \
                std::terminate(); \
            }; \
        };
}
