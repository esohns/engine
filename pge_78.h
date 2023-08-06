#ifndef PGE_78_H
#define PGE_78_H

#include "olcPixelGameEngine.h"

class PGE_78
 : public olc::PixelGameEngine
{
 public:
  PGE_78 ();
  inline virtual ~PGE_78 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;
  float v1_;

  inline float x (float);
  inline float y (float);
  inline float x2 (float);
  inline float y2 (float);
};

#endif // PGE_78_H
