#ifndef PGE_158_H
#define PGE_158_H

#include <list>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_6.h"

class PGE_158
 : public olc::PixelGameEngine
{
 public:
  PGE_158 ();
  inline virtual ~PGE_158 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class ball
  {
   public:
    olc::vf2d position_;
    olc::vf2d velocity_;
    float     mass_;

    ball (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , velocity_ ()
     , mass_ (Common_Tools::getRandomNumber (ENGINE_PGE_158_DEFAULT_MASS_MIN, ENGINE_PGE_158_DEFAULT_MASS_MAX))
    {}

    ball (float x, float y)
     : position_ (x, y)
     , velocity_ ()
     , mass_ (2.0f)
    {}
  };

  std::list<ball> balls_;
};

#endif // PGE_158_H
