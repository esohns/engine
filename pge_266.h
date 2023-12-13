#ifndef PGE_266_H
#define PGE_266_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_266
 : public olc::PixelGameEngine
{
 public:
  PGE_266 ();
  inline virtual ~PGE_266 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<float> x_;
  bool               increasing_;
  float              count_;

  float createCounter (float, float);
};

#endif // PGE_266_H
