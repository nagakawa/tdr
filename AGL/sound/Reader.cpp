#include "Reader.h"

#include <stdio.h>
#include <string.h>

using namespace agl;

size_t agl::Reader::read(void* ptr, size_t size, size_t nmemb) {
  size_t n = size * nmemb;
  if (offset + n > length) n = length - offset;
  memcpy(ptr, buffer, n);
  offset += n;
  return n;
}

int agl::Reader::seek(ogg_int64_t off, int whence) {
  switch (whence) {
    case SEEK_SET:
    offset = off;
    return offset;
    case SEEK_CUR:
    offset += off;
    if (offset > length) offset = length;
    return offset;
    case SEEK_END:
    offset = length - off;
    return offset;
  }
  return offset;
}

long agl::Reader::tell() {
  return offset;
}

size_t agl::readerRead(void* ptr, size_t size, size_t nmemb, void* datasource) {
  return ((Reader*) datasource)->read(ptr, size, nmemb);
}

int agl::readerSeek(void* datasource, ogg_int64_t off, int whence) {
  return ((Reader*) datasource)->seek(off, whence);
}

long agl::readerTell(void* datasource) {
  return ((Reader*) datasource)->tell();
}