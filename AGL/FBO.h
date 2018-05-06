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
    RBO& operator=(RBO&& that);
		void setActive();
    void allocateStorage(GLenum format, GLint width, GLint height, bool multisample = false);
		GLuint id = 0;
	};
  class FBO {
	public:
		FBO(bool def = true);
    //FBO(bool def, GLint i);
    FBO(FBO&& that);
		~FBO();
    FBO& operator=(FBO&& that);
		void setActive();
    void setActiveNoViewport();
    bool isComplete();
    void attachTexture(GLenum attachment, Texture& texture, GLenum texTarget = GL_TEXTURE_2D);
    void attachRBO(GLenum attachment, RBO&& rbo);
    void blitTo(FBO& other, int width, int height);
		GLuint id = 0;
    int width = 0, height = 0;
	};
  struct FBOTex {
    FBO fbo;
    Texture texture;
    FBOTex(FBO&& fbo, Texture&& texture) :
      fbo(std::move(fbo)), texture(std::move(texture)) {}
    FBOTex(FBOTex&& that) :
      fbo(std::move(that.fbo)), texture(std::move(that.texture)) {}
  };
  struct FBOTexMS {
    FBOTex ms;
    FBOTex ss;
    FBOTexMS(FBOTex&& ms, FBOTex&& ss) :
      ms(std::move(ms)), ss(std::move(ss)) {}
    FBOTexMS(FBOTexMS&& that) :
      ms(std::move(that.ms)), ss(std::move(that.ss)) {}
  };
  //FBO* getActiveFBO();
  //GLuint getActiveFBOID();
  FBOTex makeFBOForMe(GLint width, GLint height);
  FBOTexMS makeFBOForMeMS(GLint width, GLint height);
  void setDefaultFBOAsActive();
}
