#pragma once

#include <stddef.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace agl {
  struct Reader {
    Reader(void* buffer, size_t length) :
      buffer(buffer), length(length) {}
    size_t read(void* ptr, size_t size, size_t nmemb);
    int seek(ogg_int64_t off, int whence);
    long tell();
    void* buffer;
    size_t length;
    size_t offset = 0;
  };
  size_t readerRead(void* ptr, size_t size, size_t nmemb, void* datasource);
  int readerSeek(void* datasource, ogg_int64_t off, int whence);
  long readerTell(void* datasource);
  const ov_callbacks readerCallbacks = {
    readerRead,
    readerSeek,
    nullptr,
    readerTell
  };
}