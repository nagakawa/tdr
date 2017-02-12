#pragma once

#include <stddef.h>

namespace agl {
  class Sound {
  public:
    Sound() : sampleCount(0), samplesPerSecond(0), samples(nullptr) {}
    Sound(size_t sampleCount, size_t samplesPerSecond) :
        sampleCount(sampleCount), samplesPerSecond(samplesPerSecond),
        samples(new float[sampleCount]) {}
    Sound(const Sound&& sound) :
        sampleCount(sound.sampleCount),
        samplesPerSecond(sound.samplesPerSecond),
        samples(sound.samples) {}
    void operator=(const Sound&& sound) {
      sampleCount = sound.sampleCount;
      samplesPerSecond = sound.samplesPerSecond;
      samples = sound.samples;
    }
    ~Sound();
    size_t sampleCount;
    size_t samplesPerSecond;
    float* samples;
  };
}
