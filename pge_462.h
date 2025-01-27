#ifndef PGE_462_H
#define PGE_462_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_462
 : public olc::PixelGameEngine
{
 public:
  PGE_462 ();
  inline virtual ~PGE_462 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<int32_t> getEvents (int32_t);

  struct circle
  {
    olc::vf2d position;
    float     radius;
    olc::vf2d velocity;
  };

  olc::Pixel                 startColor_;
  olc::Pixel                 endColor_;
  olc::Pixel                 bgColor_;

  std::vector<struct circle> circles_;
};

#endif // PGE_462_H
