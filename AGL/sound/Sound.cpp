#include "Sound.h"

using namespace agl;

agl::Sound::~Sound() {
  delete[] samples;
}