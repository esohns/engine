#ifndef PGE_37_H
#define PGE_37_H

#include "olcPixelGameEngine.h"

class PGE_37
 : public olc::PixelGameEngine
{
 public:
  PGE_37 ();
  inline virtual ~PGE_37 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  typedef std::vector<olc::vf2d> positions_t;
  positions_t circlePath_;
  positions_t trianglePath_;
  float       angle_;

  olc::vf2d polarToCartesian (int32_t, float);
  olc::vf2d lerpVector (const olc::vf2d&, const olc::vf2d&, float);
  float lerp (float, float, float);
};

#endif // PGE_37_H
