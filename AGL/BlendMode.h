#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	struct BlendMode {
		GLenum eqRGB;
		GLenum eqAlpha;
		GLenum srcFuncRGB;
		GLenum srcFuncAlpha;
		GLenum destFuncRGB;
		GLenum destFuncAlpha;
		void use();
	};
	extern const BlendMode BM_ALPHA;
	extern const BlendMode BM_ADD;
	extern const BlendMode BM_SUBTRACT;
	extern const BlendMode BM_MULTIPLY;
	extern const BlendMode BM_SCREEN;
}
