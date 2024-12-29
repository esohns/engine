#ifndef PGE_447_H
#define PGE_447_H

#include <chrono>
#include <functional>
#include <vector>

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
  std::vector<olc::vf2d> makePolygon (olc::vf2d&, float, int);
  std::vector<olc::vf2d> applyToEveryEdge (std::vector<olc::vf2d>&, std::function<std::vector<olc::vf2d> (olc::vf2d&, olc::vf2d&)>);

  noise::module::Perlin noise_;
  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1_;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1_;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
};

#endif // PGE_447_H
