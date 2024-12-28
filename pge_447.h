#ifndef PGE_447_H
#define PGE_447_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_447
 : public olc::PixelGameEngine
{
 public:
  PGE_447 ();
  inline virtual ~PGE_447 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct colors
  {
    olc::Pixel color1;
    olc::Pixel color2;
    olc::Pixel color3;

    olc::Pixel background;
  };

  struct plotter
  {
    int32_t    radius;
    olc::Pixel color;
  };

  void initializeColors ();
  struct plotter getPlotter (float);

  struct colors         colors_;
  noise::module::Perlin noise_;
};

#endif // PGE_447_H
