#ifndef PGE_437_H
#define PGE_437_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_437
 : public olc::PixelGameEngine
{
 public:
  PGE_437 ();
  inline virtual ~PGE_437 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct dot
  {
    int32_t x;
    int32_t y;
    int32_t r;
    float   theta;
    float   dMin;
    float   dMax;
  };

  std::vector<struct dot> dots_;
};

#endif // PGE_437_H
