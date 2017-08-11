#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <memory>
#include "Texture.h"

namespace agl {
  class RBO {
	public:
		RBO();
    RBO(RBO&& that);
		~RBO();
		void setActive();
    void allocateStorage(GLenum format, GLint width, GLint height, bool multisample = false);
		GLuint id;
	};
  class FBO {
	public:
		FBO(bool def = false);
    FBO(bool def, GLint i);
    FBO(FBO&& that);
		~FBO();
		void setActive();
    bool isComplete();
    void attachTexture(GLenum attachment, Texture* texture, GLenum texTarget = GL_TEXTURE_2D);
    void attachRBO(GLenum attachment, RBO* rbo);
    void blitTo(FBO& other, int width, int height);
		GLuint id;
	};
  struct FBOTex {
    std::shared_ptr<FBO> fbo;
    std::shared_ptr<Texture> texture;
    FBOTex() : fbo(nullptr), texture(nullptr) {}
    FBOTex(std::shared_ptr<FBO> fbo, std::shared_ptr<Texture> texture) :
      fbo(fbo), texture(texture) {}
    FBOTex(FBOTex&& other) :
      fbo(std::move(other.fbo)), texture(std::move(other.texture)) {}
    FBOTex& operator=(FBOTex&& other) {
      fbo = std::move(other.fbo);
      texture = std::move(other.texture);
      return *this;
    }
  };
  struct FBOTexMS {
    FBOTex ms;
    FBOTex ss;
    FBOTexMS() {}
    FBOTexMS(FBOTex ms, FBOTex ss) :
      ms(std::move(ms)), ss(std::move(ss)) {}
    FBOTexMS(FBOTexMS&& other) :
      ms(std::move(other.ms)), ss(std::move(other.ss)) {}
    FBOTexMS& operator=(FBOTexMS&& other) {
      ms = std::move(other.ms);
      ss = std::move(other.ss);
      return *this;
    }
  };
  FBO* getActiveFBO();
  GLuint getActiveFBOID();
  FBOTex makeFBOForMe(GLint width, GLint height);
  FBOTexMS makeFBOForMeMS(GLint width, GLint height);
  void setDefaultFBOAsActive();
}
