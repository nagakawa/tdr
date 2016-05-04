#include "BlendMode.h"

using namespace agl;

void agl::BlendMode::use() {
	glBlendEquationSeparate(eqRGB, eqAlpha);
	glBlendFuncSeparate(srcFuncRGB, destFuncRGB, srcFuncAlpha, destFuncAlpha);
}

const BlendMode BM_ALPHA = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
	GL_ONE, GL_ONE_MINUS_SRC_ALPHA
};

const BlendMode BM_ADD = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_SRC_ALPHA, GL_ONE,
	GL_ONE, GL_ONE
};

const BlendMode BM_SUBTRACT = {
	GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT,
	GL_SRC_ALPHA, GL_ONE,
	GL_ONE, GL_ONE
};

const BlendMode BM_MULTIPLY = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_DST_COLOR, GL_ZERO,
	GL_ONE, GL_ONE_MINUS_SRC_ALPHA
};

const BlendMode BM_SCREEN = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_ONE_MINUS_DST_COLOR, GL_ONE,
	GL_ONE, GL_ONE_MINUS_SRC_ALPHA
};
