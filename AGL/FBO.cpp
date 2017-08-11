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

void agl::FBO::attachTexture(GLenum attachment, Texture* texture, GLenum texTarget) {
  setActive();
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, texture->id, 0);
}

void agl::FBO::attachRBO(GLenum attachment, RBO* rbo) {
  setActive();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo->id);
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
  glDeleteRenderbuffers(1, &id);
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
  std::shared_ptr<FBO> fbo = std::make_shared<FBO>();
  if (fbo == nullptr) throw "For some reason we could not get an FBO.";
  std::shared_ptr<Texture> tex = std::make_shared<Texture>(
    width, height, nullptr, TEX_INIT_FOR_FBO);
  if (tex == nullptr) throw "For some reason we could not get a texture.";
  fbo->attachTexture(GL_COLOR_ATTACHMENT0, tex.get());
  RBO* rbo = new RBO();
  if (rbo == nullptr) throw "For some reason we could not get an RBO.";
  rbo->allocateStorage(GL_DEPTH24_STENCIL8, width, height);
  fbo->attachRBO(GL_DEPTH_STENCIL_ATTACHMENT, rbo);
  if (!fbo->isComplete()) throw "FBO is not complete";
  return FBOTex(fbo, tex);
}

FBOTexMS agl::makeFBOForMeMS(GLint width, GLint height) {
  std::shared_ptr<FBO> fbo = std::make_shared<FBO>();
  if (fbo == nullptr) throw "For some reason we could not get an FBO.";
  std::shared_ptr<Texture> tex = std::make_shared<Texture>(
    width, height, nullptr, TEX_INIT_FOR_FBO_MS);
  if (tex == nullptr) throw "For some reason we could not get a texture.";
  fbo->attachTexture(GL_COLOR_ATTACHMENT0, tex.get(), GL_TEXTURE_2D_MULTISAMPLE);
  RBO* rbo = new RBO();
  if (rbo == nullptr) throw "For some reason we could not get an RBO.";
  rbo->allocateStorage(GL_DEPTH24_STENCIL8, width, height, true);
  fbo->attachRBO(GL_DEPTH_STENCIL_ATTACHMENT, rbo);
  if (!fbo->isComplete()) throw "FBO is not complete";
  FBOTex ftss = makeFBOForMe(width, height);
  return FBOTexMS(FBOTex(fbo, tex), std::move(ftss));
}

void agl::setDefaultFBOAsActive() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
