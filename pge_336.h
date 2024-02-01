#ifndef PGE_336_H
#define PGE_336_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_336
 : public olc::PixelGameEngine
{
 public:
  PGE_336 ();
  inline virtual ~PGE_336 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    int32_t r;
    float   theta;
  };

  std::vector<struct particle> particles_;
};

#endif // PGE_336_H
