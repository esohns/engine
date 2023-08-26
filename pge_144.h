#ifndef PGE_144_H
#define PGE_144_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_144
 : public olc::PixelGameEngine
{
 public:
  PGE_144 ();
  inline virtual ~PGE_144 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class circle
  {
   public:
    olc::vf2d  position_;
    olc::vf2d  velocity_;
    olc::Pixel color_;

    circle (olc::PixelGameEngine* engine_in, float degree, int i)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , velocity_ (std::cos (degree) * 4.0f,
                  std::sin (degree) * 4.0f)
     , color_ ()
    {
      static olc::Pixel colors_a[3] = {{255, 0, 0, 3}, {0, 255, 0, 3}, {0, 0, 255, 3}};
      color_ = colors_a[i % 3];
    }
  };

  std::vector<circle> circles_;
};

#endif // PGE_144_H
