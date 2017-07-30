#include "Sound.h"

#include <string.h>
#include <type_traits>
#include "Reader.h"

using namespace agl;

agl::Sound::~Sound() {
  delete[] samples;
}

agl::Sound::Sound(void* buffer, size_t length) {
  Reader reader(buffer, length);
  OggVorbis_File vf;
  int stat = ov_open_callbacks(
    (void*) &reader, &vf,
    nullptr, 0,
    readerCallbacks
  );
  if (stat < 0) throw "Invalid file.";
  initialiseFromOVFile(vf);
}

agl::Sound::Sound(const char* fname) {
  OggVorbis_File vf;
  int stat = ov_fopen(fname, &vf);
  if (stat < 0) throw "Invalid file.";
  initialiseFromOVFile(vf);
}

void agl::Sound::initialiseFromOVFile(OggVorbis_File& vf) {
  constexpr size_t BUFFER_SIZE = 4096;
  vorbis_info* info = ov_info(&vf, -1);
  int channels = info->channels;
  int rate = info->rate;
  // std::cout << channels << " " << rate << "\n";
  int currentSection = -1;
  size_t capacity = BUFFER_SIZE;
  SType* samples = new SType[capacity];
  SType* data = new SType[BUFFER_SIZE * channels];
  size_t size = 0;
  while (true) {
    long read = ov_read(
      &vf, (char*) data, BUFFER_SIZE * channels * sizeof(SType),
      0, sizeof(SType), std::is_signed<SType>::value, &currentSection
    );
    std::cout << read << ' ';
    long sampleCount = read / sizeof(SType) / channels;
    while (size + sampleCount >= capacity) {
      SType* newSamples = new SType[capacity << 1];
      memcpy(newSamples, samples, capacity * sizeof(SType));
      delete[] samples;
      samples = newSamples;
      capacity <<= 1;
    }
    if (read == 0) break;
    else if (read < 0) throw "There is an error in the stream.";
    else {
      for (long i = 0; i < sampleCount; ++i) {
        int sum = 0;
        for (int j = 0; j < channels; ++j) {
          sum += data[channels * i + j];
          // std::cout << data[channels * i + j] << ' ';
        }
        samples[size + i] = sum / channels;
        // std::cout << " | " << samples[size + i];
        // std::cout << '\n';
      }
      size += sampleCount;
    }
  }
  std::cout << '\n';
  for (size_t i = 0; i < size; ++i) {
    std::cout << samples[i] << ' ';
  }
  std::cout << '\n';
  this->samples = samples;
  this->sampleCount = size;
  this->samplesPerSecond = rate;
  delete[] data;
  ov_clear(&vf);
}
