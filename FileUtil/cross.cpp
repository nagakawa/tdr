#include "cross.h"

#ifdef _WIN32
extern "C" {
#include <windows.h>
}
#endif

int openFile(FILE** streamptr,
	const char* filename,
	const char* mode) {
#ifdef _WIN32
	/* Windows uses UTF-16 filenames */
	int modelen = strlen(mode);
	wchar_t* wmode = (wchar_t*) malloc(sizeof(wchar_t) * (modelen + 11 + 1));
	MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, (modelen + 11 + 1));
	wcscpy_s(wmode + modelen, 12, L", ccs=UTF-8");
	/* Convert from UTF-8 to UTF-16. */
	/* The size will never be more than twice as large. */
	int fnamelen = strlen(filename);
	wchar_t* wname = (wchar_t*) malloc(sizeof(wchar_t) * (fnamelen + 1));
	MultiByteToWideChar(CP_UTF8, 0, filename, -1, wname, fnamelen + 1);
	int err = _wfopen_s(streamptr, wname, wmode);
	free(wmode);
	free(wname);
	return err;
#else
	/* Non-Window OSes use UTF-8 filenames */
	// return fopen_s(streamptr, filename, mode);
	*streamptr = fopen(filename, mode);
	if (*streamptr != NULL) return 0;
	else return -1;
#endif
}
