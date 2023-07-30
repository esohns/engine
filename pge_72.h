#ifndef PGE_72_H
#define PGE_72_H

#include "olcPixelGameEngine.h"

class PGE_72
 : public olc::PixelGameEngine
{
 public:
  PGE_72 ();
  inline virtual ~PGE_72 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float f_;

  void vertex (int32_t, int32_t, const olc::Pixel&);
};

#endif // PGE_72_H
