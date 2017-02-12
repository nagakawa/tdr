#pragma once

#include <stddef.h>
#include <vorbis/vorbisfile.h>
#include <iostream>

namespace agl {
  class Sound {
  public:
    Sound() : sampleCount(0), samplesPerSecond(0), samples(nullptr) {}
    Sound(size_t sampleCount, size_t samplesPerSecond) :
        sampleCount(sampleCount), samplesPerSecond(samplesPerSecond),
        samples(new float[sampleCount]) {}
    Sound(Sound&& sound) :
        sampleCount(sound.sampleCount),
        samplesPerSecond(sound.samplesPerSecond),
        samples(sound.samples) {
      sound.samples = nullptr;
    }
    void operator=(Sound&& sound) {
      sampleCount = sound.sampleCount;
      samplesPerSecond = sound.samplesPerSecond;
      samples = sound.samples;
      sound.samples = nullptr;
    }
    Sound(void* buffer, size_t length);
    Sound(const char* fname);
    void initialiseFromOVFile(OggVorbis_File& vf);
    ~Sound();
    size_t sampleCount;
    size_t samplesPerSecond;
    float* samples;
  };
}
