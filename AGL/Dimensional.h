#pragma once

namespace agl {
  class Dimensional {
  public:
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual int getActualWidth() = 0;
    virtual int getActualHeight() = 0;
  };
}
