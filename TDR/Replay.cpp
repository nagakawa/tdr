#include "Replay.h"

using namespace tdr;

namespace tdr {
  Replay::Replay() {
  }

  Replay::~Replay() {
  }

  ReplaySection::ReplaySection() {
  }

  ReplaySection::~ReplaySection() {
  }

  bool ReplaySection::read(uint64_t& keys, uint16_t& fps) {
    if (idx >= keypresses.size()) {
      ++idx;
      return false;
    }
    keys = keypresses[idx];
    if (idx < fpses.size()) fps = fpses[idx];
    else fps = 65535;
    ++idx;
    return true;
  }

  void ReplaySection::write(uint64_t keys, uint16_t fps) {
    if (keypresses.size() <= idx + 1) keypresses.resize(idx + 1);
    if (fpses.size() <= idx + 1) fpses.resize(idx + 1);
    keypresses[idx] = keys;
    fpses[idx] = fps;
  }
}
