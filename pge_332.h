#ifndef PGE_332_H
#define PGE_332_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_image_tools.h"

class PGE_332
 : public olc::PixelGameEngine
{
 public:
  PGE_332 ();
  inline virtual ~PGE_332 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (olc::PixelGameEngine* engine)
     : position_ (engine->ScreenWidth () / 2.0f,
                  static_cast<float> (engine->ScreenHeight ()))
     , last_ (position_)
     , velocity_ (Common_Tools::getRandomNumber (-3.0f, 3.0f),
                  Common_Tools::getRandomNumber (-13.0f, -16.0f))
     , hue_ (Common_Tools::getRandomNumber (0.0f, 360.0f))
    {}

    void run (olc::PixelGameEngine* engine)
    {
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue_, 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
      hue_ += 1.0f;
      hue_ = std::fmod (hue_, 360.0f);

      position_ += velocity_;
      velocity_.y += 0.2f;

      //engine->FillCircle (position_, 3, color);
      engine->DrawLine (last_, position_, color, 0xFFFFFFFF);

      last_ = position_;
    }

    olc::vf2d position_;
    olc::vf2d last_;
    olc::vf2d velocity_;
    float     hue_;
  };

  std::vector<particle> particles_;
};

#endif // PGE_332_H
