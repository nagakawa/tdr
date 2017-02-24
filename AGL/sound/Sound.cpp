#include "Sound.h"

#include <string.h>
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
  vorbis_info* info = ov_info(&vf, -1);
  int channels = info->channels;
  int rate = info->rate;
  // std::cout << channels << " " << rate << "\n";
  float** data;
  //uint16_t buffer[4096];
  int currentSection = -1;
  size_t capacity = 4096;
  float* samples = new float[capacity];
  size_t size = 0;
  while (true) {
    long read = ov_read_float(&vf, &data, 4096, &currentSection);
    if (size + read > capacity) {
      float* newSamples = new float[capacity << 1];
      memcpy(newSamples, samples, capacity * sizeof(float));
      delete[] samples;
      samples = newSamples;
      capacity <<= 1;
    }
    if (read == 0) break;
    else if (read < 0) throw "There is an error in the stream.";
    else {
      for (long i = 0; i < read; ++i) {
        float avg = 0.0f;
        for (int j = 0; j < channels; ++j) {
          avg += data[j][i];
        }
        samples[size + i] = avg / channels;
      }
      // std::cout << '\n';
      size += read;
    }
  }
  this->samples = samples;
  this->sampleCount = size;
  this->samplesPerSecond = rate;
  ov_clear(&vf);
}
