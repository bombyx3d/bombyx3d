//
// Copyright 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// ProgramImpl.h: Defines the abstract rx::ProgramImpl class.

#ifndef LIBANGLE_RENDERER_PROGRAMIMPL_H_
#define LIBANGLE_RENDERER_PROGRAMIMPL_H_

#include "common/angleutils.h"
#include "libANGLE/BinaryStream.h"
#include "libANGLE/Constants.h"
#include "libANGLE/Program.h"
#include "libANGLE/Shader.h"
#include "libANGLE/renderer/Renderer.h"

#include <map>

namespace rx
{

struct LinkResult
{
    bool linkSuccess;
    gl::Error error;
    LinkResult(bool linkSuccess, const gl::Error &error);
};

class ProgramImpl : angle::NonCopyable
{
  public:
    ProgramImpl(const gl::Program::Data &data);
    virtual ~ProgramImpl();

    virtual int getShaderVersion() const = 0;

    virtual GLenum getBinaryFormat() = 0;
    virtual LinkResult load(gl::InfoLog &infoLog, gl::BinaryInputStream *stream) = 0;
    virtual gl::Error save(gl::BinaryOutputStream *stream) = 0;

    virtual LinkResult link(const gl::Data &data, gl::InfoLog &infoLog) = 0;
    virtual GLboolean validate(const gl::Caps &caps, gl::InfoLog *infoLog) = 0;

    virtual void setUniform1fv(GLint location, GLsizei count, const GLfloat *v) = 0;
    virtual void setUniform2fv(GLint location, GLsizei count, const GLfloat *v) = 0;
    virtual void setUniform3fv(GLint location, GLsizei count, const GLfloat *v) = 0;
    virtual void setUniform4fv(GLint location, GLsizei count, const GLfloat *v) = 0;
    virtual void setUniform1iv(GLint location, GLsizei count, const GLint *v) = 0;
    virtual void setUniform2iv(GLint location, GLsizei count, const GLint *v) = 0;
    virtual void setUniform3iv(GLint location, GLsizei count, const GLint *v) = 0;
    virtual void setUniform4iv(GLint location, GLsizei count, const GLint *v) = 0;
    virtual void setUniform1uiv(GLint location, GLsizei count, const GLuint *v) = 0;
    virtual void setUniform2uiv(GLint location, GLsizei count, const GLuint *v) = 0;
    virtual void setUniform3uiv(GLint location, GLsizei count, const GLuint *v) = 0;
    virtual void setUniform4uiv(GLint location, GLsizei count, const GLuint *v) = 0;
    virtual void setUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void setUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

    virtual void getUniformfv(GLint location, GLfloat *params) = 0;
    virtual void getUniformiv(GLint location, GLint *params) = 0;
    virtual void getUniformuiv(GLint location, GLuint *params) = 0;

    // TODO: The following functions are possibly only applicable to D3D backends. The should be carefully evaluated to
    // determine if they can be removed from this interface.
    virtual bool validateSamplers(gl::InfoLog *infoLog, const gl::Caps &caps) = 0;

    const std::vector<gl::LinkedUniform*> &getUniforms() const { return mUniforms; }
    const std::map<GLuint, gl::VariableLocation> &getUniformIndices() const { return mUniformIndex; }
    const std::vector<gl::UniformBlock*> &getUniformBlocks() const { return mUniformBlocks; }

    std::vector<gl::LinkedUniform*> &getUniforms() { return mUniforms; }
    std::map<GLuint, gl::VariableLocation> &getUniformIndices() { return mUniformIndex; }
    std::vector<gl::UniformBlock*> &getUniformBlocks() { return mUniformBlocks; }

    gl::LinkedUniform *getUniformByLocation(GLint location) const;
    gl::LinkedUniform *getUniformByName(const std::string &name) const;
    gl::UniformBlock *getUniformBlockByIndex(GLuint blockIndex) const;

    GLint getUniformLocation(const std::string &name) const;
    GLuint getUniformIndex(const std::string &name) const;
    GLuint getUniformBlockIndex(const std::string &name) const;

    virtual void reset();

  protected:
    const gl::Program::Data &mData;

    std::vector<gl::LinkedUniform*> mUniforms;

    // TODO: use a hash map
    std::map<GLuint, gl::VariableLocation> mUniformIndex;

    std::vector<gl::UniformBlock*> mUniformBlocks;
};

}

#endif // LIBANGLE_RENDERER_PROGRAMIMPL_H_