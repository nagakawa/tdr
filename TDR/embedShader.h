#pragma once

#define EMBED_SHADER(sourceName, name, STATIC) \
  extern "C" const char _binary_##sourceName##_glsl_start[]; \
  extern "C" const char _binary_##sourceName##_glsl_end[]; \
  STATIC size_t name##SourceSize = \
    _binary_##sourceName##_glsl_end - _binary_##sourceName##_glsl_start; \
  STATIC const char* name##Source = \
     _binary_##sourceName##_glsl_start;
