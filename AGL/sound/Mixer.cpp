#include "Mixer.h"

using namespace agl;

const size_t defaultRate = 44100;

void agl::Mixer::addSound(int index, Sound&& s) {
  if (sounds.count(index) != 0) throw "Already a sound at index";
  sounds[index] = std::move(s);
}

int agl::Mixer::playSound(int index, float volume) {
  MixerEntry m = { index, volume, 0, 0.0f };
  entries[handleCount] = m;
  return handleCount++;
}

void agl::Mixer::advance(unsigned long n, float* output) {
  for (unsigned long i = 0; i < n; ++i)
    output[i] = 0.0f;
  for (auto it = entries.begin(); it != entries.end();) {
    MixerEntry& me = it->second;
    const Sound& s = sounds[me.key];
    float ratio = ((float) s.samplesPerSecond) / defaultRate;
    for (unsigned long i = 0; i < n; ++i) {
      float prog = i * ratio;
      size_t iprog = (size_t) prog;
      float frac = prog - iprog;
      if (iprog > s.sampleCount - me.curr - 1) break;
      float curr = s.samples[me.curr + iprog];
      float next = s.samples[me.curr + iprog + 1];
      output[i] += (curr * frac + next * (1 - frac)) * me.volume;
    }
    me.interm += ((float) n) * ratio;
    size_t adv = (size_t) me.interm;
    me.interm -= adv;
    me.curr += adv;
    if (me.curr >= s.sampleCount)
      it = entries.erase(it);
    else ++it;
  }
}

int agl::mixerCallback(
    const void* input,
    void* output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
  (void) input; (void) timeInfo; (void) statusFlags;
  Mixer* mixer = (Mixer*) userData;
  float* out = (float*) output;
  mixer->advance(frameCount, out);
  return 0;
}
#include <iostream>
void agl::Mixer::regist() {
    PaError err;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(
      &stream,
      0,
      1,
      paFloat32,
      defaultRate,
      paFramesPerBufferUnspecified,
      mixerCallback,
      (void*) this
    );
    if (err != paNoError) throw "Couldn't open PortAudio stream.";
    err = Pa_StartStream(stream);
    if (err != paNoError) throw "Couldn't start stream.";
}

void agl::Mixer::stop() {
  Pa_StopStream(stream);
}