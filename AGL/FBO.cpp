#include "FBO.h"
#include "debug.h"

using namespace agl;

#include <assert.h>
agl::FBO::FBO(bool def) {
  if (def) id = 0;
  else glGenFramebuffers(1, &id);
}

agl::FBO::FBO(bool def, GLint i) {
  (void) def;
  id = i;
}

agl::FBO::FBO(FBO&& that) {
  id = that.id;
  that.id = 0;
}

agl::FBO::~FBO() {
  if (id != 0) glDeleteFramebuffers(1, &id);
}

FBO& agl::FBO::operator=(FBO&& that) {
  std::swap(id, that.id);
  return *this;
}

void agl::FBO::setActive() {
  glBindFramebuffer(GL_FRAMEBUFFER, id);
}

GLuint agl::getActiveFBOID() {
  GLint result;
  glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &result);
  return result;
}

FBO* agl::getActiveFBO() {
  return new FBO(false, getActiveFBOID());
}

bool agl::FBO::isComplete() {
  setActive();
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void agl::FBO::attachTexture(
    GLenum attachment, Texture& texture, GLenum texTarget) {
  setActive();
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, attachment, texTarget, texture.id, 0);
}

void agl::FBO::attachRBO(GLenum attachment, RBO&& rbo) {
  setActive();
  glFramebufferRenderbuffer(
    GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.id);
  rbo.id = 0;
}

void agl::FBO::blitTo(FBO& other, int width, int height) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other.id);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

agl::RBO::RBO() {
  glGenRenderbuffers(1, &id);
}

agl::RBO::RBO(RBO&& that) {
  id = that.id;
  that.id = 0;
}

agl::RBO::~RBO() {
  if (id != 0) glDeleteRenderbuffers(1, &id);
}

RBO& agl::RBO::operator=(RBO&& that) {
  id = that.id;
  if (this != &that) that.id = 0;
  return *this;
}

void agl::RBO::setActive() {
  glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void agl::RBO::allocateStorage(GLenum format, GLint width, GLint height, bool multisample) {
  setActive();
  if (multisample) glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, format, width, height);
  else glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

FBOTex agl::makeFBOForMe(GLint width, GLint height) {
  FBO fbo(false);
  Texture tex(width, height, nullptr, TEX_INIT_FOR_FBO);
  fbo.attachTexture(GL_COLOR_ATTACHMENT0, tex);
  RBO rbo;
  rbo.allocateStorage(GL_DEPTH24_STENCIL8, width, height);
  fbo.attachRBO(GL_DEPTH_STENCIL_ATTACHMENT, std::move(rbo));
  if (!fbo.isComplete()) throw "FBO is not complete";
  return FBOTex{std::move(fbo), std::move(tex)};
}

FBOTexMS agl::makeFBOForMeMS(GLint width, GLint height) {
  FBO fbo(false);
  Texture tex(width, height, nullptr, TEX_INIT_FOR_FBO_MS);
  fbo.attachTexture(GL_COLOR_ATTACHMENT0, tex, GL_TEXTURE_2D_MULTISAMPLE);
  RBO rbo;
  rbo.allocateStorage(GL_DEPTH24_STENCIL8, width, height, true);
  fbo.attachRBO(GL_DEPTH_STENCIL_ATTACHMENT, std::move(rbo));
  if (!fbo.isComplete()) throw "FBO is not complete";
  FBOTex ftss = makeFBOForMe(width, height);
  return FBOTexMS{{std::move(fbo), std::move(tex)}, std::move(ftss)};
}

void agl::setDefaultFBOAsActive() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
