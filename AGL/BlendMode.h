#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	struct BlendMode {
		GLenum eqRGB;
		GLenum eqAlpha;
		GLenum srcFuncRGB;
		GLenum destFuncRGB;
		GLenum srcFuncAlpha;
		GLenum destFuncAlpha;
		void use() const;
	};
	extern const BlendMode BM_ALPHA;
	extern const BlendMode BM_ADD;
	extern const BlendMode BM_SUBTRACT;
	extern const BlendMode BM_MULTIPLY;
	extern const BlendMode BM_SCREEN;
}
