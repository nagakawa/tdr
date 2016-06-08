#include "FBO.h"
#include "debug.h"

using namespace agl;

#include <assert.h>
FBO::FBO(bool def) {
  if (def) id = 0;
  else glGenFramebuffers(1, &id);
}

FBO::FBO(bool def, GLint i) {
  id = i;
}

FBO::~FBO() {
  if (id != 0) glDeleteFramebuffers(1, &id);
}

void FBO::setActive() {
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

bool FBO::isComplete() {
  setActive();
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FBO::attachTexture(GLenum attachment, Texture* texture, GLenum texTarget, GLuint mipmap) {
  setActive();
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, texture->id, 0);
}

void FBO::attachRBO(GLenum attachment, RBO* rbo) {
  setActive();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo->id);
}

void FBO::blitTo(FBO& other, int width, int height) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other.id);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

RBO::RBO() {
  glGenRenderbuffers(1, &id);
}

RBO::~RBO() {
  glDeleteRenderbuffers(1, &id);
}

void RBO::setActive() {
  glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RBO::allocateStorage(GLenum format, GLint width, GLint height, bool multisample) {
  setActive();
  if (multisample) glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, format, width, height);
  else glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

FBOTex agl::makeFBOForMe(GLint width, GLint height) {
  FBO* fbo = new FBO();
  if (fbo == nullptr) throw "For some reason we could not get an FBO.";
  Texture* tex = new Texture(width, height, nullptr, TEX_INIT_FOR_FBO);
  if (tex == nullptr) throw "For some reason we could not get a texture.";
  fbo->attachTexture(GL_COLOR_ATTACHMENT0, tex);
  RBO* rbo = new RBO();
  if (rbo == nullptr) throw "For some reason we could not get an RBO.";
  rbo->allocateStorage(GL_DEPTH24_STENCIL8, width, height);
  fbo->attachRBO(GL_DEPTH_STENCIL_ATTACHMENT, rbo);
  if (!fbo->isComplete()) throw "FBO is not complete";
  return {fbo, tex};
}

FBOTexMS agl::makeFBOForMeMS(GLint width, GLint height) {
  FBO* fbo = new FBO();
  if (fbo == nullptr) throw "For some reason we could not get an FBO.";
  Texture* tex = new Texture(width, height, nullptr, TEX_INIT_FOR_FBO_MS);
  if (tex == nullptr) throw "For some reason we could not get a texture.";
  fbo->attachTexture(GL_COLOR_ATTACHMENT0, tex, GL_TEXTURE_2D_MULTISAMPLE);
  RBO* rbo = new RBO();
  if (rbo == nullptr) throw "For some reason we could not get an RBO.";
  rbo->allocateStorage(GL_DEPTH24_STENCIL8, width, height, true);
  fbo->attachRBO(GL_DEPTH_STENCIL_ATTACHMENT, rbo);
  if (!fbo->isComplete()) throw "FBO is not complete";
  FBOTex ftss = makeFBOForMe(width, height);
  return {{fbo, tex}, ftss};
}

void agl::setDefaultFBOAsActive() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
