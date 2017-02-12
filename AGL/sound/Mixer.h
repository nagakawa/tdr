#pragma once

#include <stddef.h>
#include <memory>
#include <unordered_map>
#include <portaudio.h>
#include "Sound.h"

namespace agl {
  struct MixerEntry {
    int key;
    float volume;
    size_t curr;
  };
  class Mixer {
  public:
    void addSound(int index, const Sound&& s);
    int playSound(int index, float volume = 1.0);
    void advance(unsigned long n, float* output);
    void regist();
    void stop();
    std::unordered_map<int, Sound> sounds;
    int handleCount;
    std::unordered_map<int, MixerEntry> entries;
    PaStream* stream;
  };
  int mixerCallback(
    const void* input,
    void* output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData
  );
}