//
// Copyright (c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// ProgramD3D.h: Defines the rx::ProgramD3D class which implements rx::ProgramImpl.

#ifndef LIBANGLE_RENDERER_D3D_PROGRAMD3D_H_
#define LIBANGLE_RENDERER_D3D_PROGRAMD3D_H_

#include <string>
#include <vector>

#include "common/Optional.h"
#include "compiler/translator/blocklayoutHLSL.h"
#include "libANGLE/Constants.h"
#include "libANGLE/formatutils.h"
#include "libANGLE/renderer/ProgramImpl.h"
#include "libANGLE/renderer/d3d/DynamicHLSL.h"
#include "libANGLE/renderer/d3d/WorkaroundsD3D.h"

namespace gl
{
struct LinkedUniform;
struct VariableLocation;
struct VertexFormat;
}

namespace rx
{
class RendererD3D;
class UniformStorageD3D;
class ShaderExecutableD3D;

#if !defined(ANGLE_COMPILE_OPTIMIZATION_LEVEL)
// WARNING: D3DCOMPILE_OPTIMIZATION_LEVEL3 may lead to a DX9 shader compiler hang.
// It should only be used selectively to work around specific bugs.
#define ANGLE_COMPILE_OPTIMIZATION_LEVEL D3DCOMPILE_OPTIMIZATION_LEVEL1
#endif

class ProgramD3D : public ProgramImpl
{
  public:
    typedef int SemanticIndexArray[gl::MAX_VERTEX_ATTRIBS];

    ProgramD3D(const gl::Program::Data &data, RendererD3D *renderer);
    virtual ~ProgramD3D();

    const std::vector<PixelShaderOutputVariable> &getPixelShaderKey() { return mPixelShaderKey; }
    int getShaderVersion() const { return mShaderVersion; }

    GLint getSamplerMapping(gl::SamplerType type, unsigned int samplerIndex, const gl::Caps &caps) const;
    GLenum getSamplerTextureType(gl::SamplerType type, unsigned int samplerIndex) const;
    GLint getUsedSamplerRange(gl::SamplerType type) const;
    void updateSamplerMapping();
    bool validateSamplers(gl::InfoLog *infoLog, const gl::Caps &caps);

    bool usesPointSize() const { return mUsesPointSize; }
    bool usesPointSpriteEmulation() const;
    bool usesGeometryShader() const;
    bool usesInstancedPointSpriteEmulation() const;

    GLenum getBinaryFormat() { return GL_PROGRAM_BINARY_ANGLE; }
    LinkResult load(gl::InfoLog &infoLog, gl::BinaryInputStream *stream);
    gl::Error save(gl::BinaryOutputStream *stream);

    gl::Error getPixelExecutableForFramebuffer(const gl::Framebuffer *fbo, ShaderExecutableD3D **outExectuable);
    gl::Error getPixelExecutableForOutputLayout(const std::vector<GLenum> &outputLayout, ShaderExecutableD3D **outExectuable, gl::InfoLog *infoLog);
    gl::Error getVertexExecutableForInputLayout(const gl::InputLayout &inputLayout, ShaderExecutableD3D **outExectuable, gl::InfoLog *infoLog);
    ShaderExecutableD3D *getGeometryExecutable() const { return mGeometryExecutable; }

    LinkResult link(const gl::Data &data, gl::InfoLog &infoLog) override;
    GLboolean validate(const gl::Caps &caps, gl::InfoLog *infoLog) override;

    void initializeUniformStorage();
    gl::Error applyUniforms();
    gl::Error applyUniformBuffers(const gl::Data &data);
    void assignUniformBlockRegister(gl::UniformBlock *uniformBlock,
                                    GLenum shader,
                                    unsigned int registerIndex,
                                    const gl::Caps &caps);
    void dirtyAllUniforms();

    void setUniform1fv(GLint location, GLsizei count, const GLfloat *v);
    void setUniform2fv(GLint location, GLsizei count, const GLfloat *v);
    void setUniform3fv(GLint location, GLsizei count, const GLfloat *v);
    void setUniform4fv(GLint location, GLsizei count, const GLfloat *v);
    void setUniform1iv(GLint location, GLsizei count, const GLint *v);
    void setUniform2iv(GLint location, GLsizei count, const GLint *v);
    void setUniform3iv(GLint location, GLsizei count, const GLint *v);
    void setUniform4iv(GLint location, GLsizei count, const GLint *v);
    void setUniform1uiv(GLint location, GLsizei count, const GLuint *v);
    void setUniform2uiv(GLint location, GLsizei count, const GLuint *v);
    void setUniform3uiv(GLint location, GLsizei count, const GLuint *v);
    void setUniform4uiv(GLint location, GLsizei count, const GLuint *v);
    void setUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

    void getUniformfv(GLint location, GLfloat *params);
    void getUniformiv(GLint location, GLint *params);
    void getUniformuiv(GLint location, GLuint *params);

    const UniformStorageD3D &getVertexUniformStorage() const { return *mVertexUniformStorage; }
    const UniformStorageD3D &getFragmentUniformStorage() const { return *mFragmentUniformStorage; }

    void reset();

    unsigned int getSerial() const;

    void sortAttributesByLayout(const std::vector<TranslatedAttribute> &unsortedAttributes,
                                int sortedSemanticIndicesOut[gl::MAX_VERTEX_ATTRIBS],
                                const rx::TranslatedAttribute *sortedAttributesOut[gl::MAX_VERTEX_ATTRIBS]) const;
    const SemanticIndexArray &getSemanticIndexes() const { return mSemanticIndexes; }
    const SemanticIndexArray &getAttributesByLayout() const { return mAttributesByLayout; }

    void updateCachedInputLayout(const gl::State &state);
    const gl::InputLayout &getCachedInputLayout() const { return mCachedInputLayout; }

  private:
    class VertexExecutable
    {
      public:
        typedef std::vector<bool> Signature;

        VertexExecutable(const gl::InputLayout &inputLayout,
                         const Signature &signature,
                         ShaderExecutableD3D *shaderExecutable);
        ~VertexExecutable();

        bool matchesSignature(const Signature &signature) const;
        static void getSignature(RendererD3D *renderer,
                                 const gl::InputLayout &inputLayout,
                                 Signature *signatureOut);

        const gl::InputLayout &inputs() const { return mInputs; }
        const Signature &signature() const { return mSignature; }
        ShaderExecutableD3D *shaderExecutable() const { return mShaderExecutable; }

      private:
        gl::InputLayout mInputs;
        Signature mSignature;
        ShaderExecutableD3D *mShaderExecutable;
    };

    class PixelExecutable
    {
      public:
        PixelExecutable(const std::vector<GLenum> &outputSignature, ShaderExecutableD3D *shaderExecutable);
        ~PixelExecutable();

        bool matchesSignature(const std::vector<GLenum> &signature) const { return mOutputSignature == signature; }

        const std::vector<GLenum> &outputSignature() const { return mOutputSignature; }
        ShaderExecutableD3D *shaderExecutable() const { return mShaderExecutable; }

      private:
        std::vector<GLenum> mOutputSignature;
        ShaderExecutableD3D *mShaderExecutable;
    };

    struct Sampler
    {
        Sampler();

        bool active;
        GLint logicalTextureUnit;
        GLenum textureType;
    };

    bool defineUniforms(gl::InfoLog &infoLog, const gl::Caps &caps);
    void defineUniformBase(const ShaderD3D *shader, const sh::Uniform &uniform, unsigned int uniformRegister);
    void defineUniform(const ShaderD3D *shader, const sh::ShaderVariable &uniform, const std::string &fullName,
                       sh::HLSLBlockEncoder *encoder);
    bool indexSamplerUniform(const gl::LinkedUniform &uniform, gl::InfoLog &infoLog, const gl::Caps &caps);
    bool indexUniforms(gl::InfoLog &infoLog, const gl::Caps &caps);
    static bool assignSamplers(unsigned int startSamplerIndex, GLenum samplerType, unsigned int samplerCount,
                               std::vector<Sampler> &outSamplers, GLuint *outUsedRange);

    void defineUniformBlocks(const gl::Caps &caps);
    void defineUniformBlock(const gl::Shader &shader,
                            const sh::InterfaceBlock &interfaceBlock,
                            const gl::Caps &caps);

    template <typename T>
    void setUniform(GLint location, GLsizei count, const T* v, GLenum targetUniformType);

    template <int cols, int rows>
    void setUniformMatrixfv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value, GLenum targetUniformType);

    template <typename T>
    void getUniformv(GLint location, T *params, GLenum uniformType);

    template <typename VarT>
    void defineUniformBlockMembers(const std::vector<VarT> &fields, const std::string &prefix, int blockIndex,
                                   sh::BlockLayoutEncoder *encoder, std::vector<unsigned int> *blockUniformIndexes,
                                   bool inRowMajorLayout);

    LinkResult compileProgramExecutables(gl::InfoLog &infoLog,
                                         int registers,
                                         const std::vector<PackedVarying> &packedVaryings);

    void gatherTransformFeedbackVaryings(const std::vector<gl::LinkedVarying> &varyings);

    void initSemanticIndex();
    void initAttributesByLayout();

    RendererD3D *mRenderer;
    DynamicHLSL *mDynamicHLSL;

    std::vector<VertexExecutable *> mVertexExecutables;
    std::vector<PixelExecutable *> mPixelExecutables;
    ShaderExecutableD3D *mGeometryExecutable;

    std::string mVertexHLSL;
    D3DCompilerWorkarounds mVertexWorkarounds;

    std::string mPixelHLSL;
    D3DCompilerWorkarounds mPixelWorkarounds;
    bool mUsesFragDepth;
    std::vector<PixelShaderOutputVariable> mPixelShaderKey;

    bool mUsesPointSize;

    UniformStorageD3D *mVertexUniformStorage;
    UniformStorageD3D *mFragmentUniformStorage;

    std::vector<Sampler> mSamplersPS;
    std::vector<Sampler> mSamplersVS;
    GLuint mUsedVertexSamplerRange;
    GLuint mUsedPixelSamplerRange;
    bool mDirtySamplerMapping;

    // Cache for validateSamplers
    std::vector<GLenum> mTextureUnitTypesCache;

    // Cache for getPixelExecutableForFramebuffer
    std::vector<GLenum> mPixelShaderOutputFormatCache;

    int mShaderVersion;

    SemanticIndexArray mSemanticIndexes;
    SemanticIndexArray mAttributesByLayout;

    unsigned int mSerial;

    Optional<bool> mCachedValidateSamplersResult;

    std::vector<GLint> mVertexUBOCache;
    std::vector<GLint> mFragmentUBOCache;
    VertexExecutable::Signature mCachedVertexSignature;
    gl::InputLayout mCachedInputLayout;

    std::vector<gl::LinkedVarying> mTransformFeedbackLinkedVaryings;

    static unsigned int issueSerial();
    static unsigned int mCurrentSerial;
};

}

#endif // LIBANGLE_RENDERER_D3D_PROGRAMD3D_H_
