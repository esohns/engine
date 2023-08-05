#ifndef PGE_74_H
#define PGE_74_H

#include "olcPixelGameEngine.h"

class PGE_74
 : public olc::PixelGameEngine
{
 public:
  PGE_74 ();
  inline virtual ~PGE_74 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float   a_;
  float   b_;
  //float   c_;
  //float   d_;
  int32_t t_;

  int32_t x (int32_t);
  int32_t y (int32_t);
  //int32_t x2 (int32_t);
  //int32_t y2 (int32_t);
};

#endif // PGE_74_H
