#ifndef PGE_252_H
#define PGE_252_H

#include "olcPixelGameEngine.h"

#include "opensimplexnoise.h"

class PGE_252
 : public olc::PixelGameEngine
{
 public:
  PGE_252 ();
  inline virtual ~PGE_252 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  OpenSimplexNoise noise_;
  olc::Pixel       insideColor_;
  olc::Pixel       outsideColor_;
  olc::Pixel       edgeColor_;

  float length (float, float);
  float edge (float, float);
  float sdfBox (float, float, float, float, float, float);
  float sdfRepeat (float, float);
  float sdf (float, float, int);
};

#endif // PGE_252_H
