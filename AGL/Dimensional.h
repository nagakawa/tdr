#pragma once

namespace agl {
  class Dimensional {
  public:
    virtual ~Dimensional();
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getActualWidth() const = 0;
    virtual int getActualHeight() const = 0;
    virtual int getFBOID() const = 0;
  };
}
