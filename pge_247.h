#ifndef PGE_247_H
#define PGE_247_H

#include "olcPixelGameEngine.h"

class PGE_247
 : public olc::PixelGameEngine
{
 public:
  PGE_247 ();
  inline virtual ~PGE_247 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float pad_;
  float spacing_;

  float pDistance (float, float, float, float, float, float);
};

#endif // PGE_247_H
