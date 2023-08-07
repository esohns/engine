#ifndef PGE_82_H
#define PGE_82_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_82
 : public olc::PixelGameEngine
{
 public:
  PGE_82 ();
  inline virtual ~PGE_82 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct object
  {
    olc::vf2d p;
    float     la;
    float     lv;
    float     d;
    float     n;
    float     so;
    float     sa;
  };
  std::vector<object>   objects_;
  double                x_motion_;
  double                y_motion_;
  olc::Pixel            color_;
  float                 hue_;
  noise::module::Perlin noise_;
  std::vector<float>    trail_map_;

  void compute (int);
};

#endif // PGE_82_H
