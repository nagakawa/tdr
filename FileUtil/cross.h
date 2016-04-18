#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

errno_t openFile(FILE** streamptr,
	const char* filename,
	const char* mode);
