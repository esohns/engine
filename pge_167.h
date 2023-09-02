#ifndef PGE_167_H
#define PGE_167_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_7.h"

class PGE_167
 : public olc::PixelGameEngine
{
 public:
  PGE_167 ();
  inline virtual ~PGE_167 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    olc::vf2d position_;
    olc::vf2d velocity_;

    particle (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , velocity_ (std::cos (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI))) / ENGINE_PGE_167_DEFAULT_SPEED,
                  std::sin (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI))) / ENGINE_PGE_167_DEFAULT_SPEED)
    {}
  };

  std::vector<particle> particles_;
};

#endif // PGE_167_H
