#pragma once

#include <stdio.h>

#define TRGLE printf("Error code is 0x%x, at %s:%d.\n", glGetError(), __FILE__, __LINE__)
