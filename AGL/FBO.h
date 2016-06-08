#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include "Texture.h"

namespace agl {
  class RBO {
	public:
		RBO();
		~RBO();
		void setActive();
    void allocateStorage(GLenum format, GLint width, GLint height, bool multisample = false);
		GLuint id;
	};
  class FBO {
	public:
		FBO(bool def = false);
    FBO(bool def, GLint i);
		~FBO();
		void setActive();
    bool isComplete();
    void attachTexture(GLenum attachment, Texture* texture, GLenum texTarget = GL_TEXTURE_2D, GLuint mipmap = 0);
    void attachRBO(GLenum attachment, RBO* rbo);
    void blitTo(FBO& other, int width, int height);
		GLuint id;
	};
  struct FBOTex {
    FBO* fbo;
    Texture* texture;
  };
  struct FBOTexMS {
    FBOTex ms;
    FBOTex ss;
  };
  FBO* getActiveFBO();
  GLuint getActiveFBOID();
  FBOTex makeFBOForMe(GLint width, GLint height);
  FBOTexMS makeFBOForMeMS(GLint width, GLint height);
  void setDefaultFBOAsActive();
}
