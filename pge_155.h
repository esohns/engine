#ifndef PGE_155_H
#define PGE_155_H

#include <vector>

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_155
 : public olc::PixelGameEngine
{
 public:
  PGE_155 ();
  inline virtual ~PGE_155 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    olc::vf2d position_;
    float     size_;

    particle (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , size_ (2.0f)
    {}
  };

  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
  std::vector<particle> particles_;
  float                 t_;
};

#endif // PGE_155_H
