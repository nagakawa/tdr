#include "Mixer.h"

using namespace agl;

void agl::Mixer::addSound(int index, const Sound&& s) {
  if (sounds.count(index) != 0) throw "Already a sound at index";
  sounds[index] = std::move(s);
}

int agl::Mixer::playSound(int index, float volume) {
  MixerEntry m = { index, volume, 0 };
  entries[handleCount] = m;
  return handleCount++;
}

void agl::Mixer::advance(unsigned long n, float* output) {
  for (unsigned long i = 0; i < n; ++i)
    output[i] = 0.0f;
  for (auto it = entries.begin(); it != entries.end();) {
    MixerEntry& me = it->second;
    const Sound& s = sounds[me.key];
    for (unsigned long i = 0; i < n; ++i) {
      if (me.curr + i >= s.sampleCount) break;
      output[i] += s.samples[me.curr + i] * me.volume;
    }
    me.curr += n;
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

void agl::Mixer::regist() {
    PaError err;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(
      &stream,
      0,
      2,
      paFloat32,
      48000,
      paFramesPerBufferUnspecified,
      mixerCallback,
      (void*) this
    );
    if (err != paNoError) throw "Couldn't register mixer to PortAudio.";
    err = Pa_StartStream(stream);
    if (err != paNoError) throw "Couldn't register mixer to PortAudio.";
}

void agl::Mixer::stop() {
  Pa_StopStream(stream);
}