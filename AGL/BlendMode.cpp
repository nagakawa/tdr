#include "BlendMode.h"

using namespace agl;

void agl::BlendMode::use() const {
	glBlendEquationSeparate(eqRGB, eqAlpha);
	glBlendFuncSeparate(srcFuncRGB, destFuncRGB, srcFuncAlpha, destFuncAlpha);
}

const BlendMode agl::BM_ALPHA = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
	GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
};

const BlendMode agl::BM_ADD = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_SRC_ALPHA, GL_ONE,
	GL_ONE, GL_ONE
};

const BlendMode agl::BM_SUBTRACT = {
	GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT,
	GL_SRC_ALPHA, GL_ONE,
	GL_ONE, GL_ONE
};

const BlendMode agl::BM_MULTIPLY = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_DST_COLOR, GL_ZERO,
	GL_ONE, GL_ONE_MINUS_SRC_ALPHA
};

const BlendMode agl::BM_SCREEN = {
	GL_FUNC_ADD, GL_FUNC_ADD,
	GL_ONE_MINUS_DST_COLOR, GL_ONE,
	GL_ONE, GL_ONE_MINUS_SRC_ALPHA
};

const BlendMode agl::BM_SHADOW = {
	GL_FUNC_REVERSE_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_DST_COLOR, GL_ONE,
	GL_ONE, GL_ONE
};

const BlendMode agl::blendModes[] = {
	BM_ALPHA,
	BM_ADD,
	BM_SUBTRACT,
	BM_MULTIPLY,
	BM_SCREEN,
	BM_SHADOW,
};
