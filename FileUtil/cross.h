#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int openFile(FILE** streamptr,
	const char* filename,
	const char* mode);
