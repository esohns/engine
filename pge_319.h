#ifndef PGE_319_H
#define PGE_319_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_14.h"

class PGE_319
 : public olc::PixelGameEngine
{
 public:
  PGE_319 ();
  inline virtual ~PGE_319 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class ball
  {
   public:
    ball (olc::PixelGameEngine* engine)
     : position_ (engine->ScreenWidth () / 2.0f, engine->ScreenHeight () / 2.0f)
     , velocity_ (Common_Tools::getRandomNumber (1.0f, 3.0f),
                  Common_Tools::getRandomNumber (1.0f, 3.0f))
     , size_ (Common_Tools::getRandomNumber (ENGINE_PGE_319_DEFAULT_MIN_SIZE, ENGINE_PGE_319_DEFAULT_MAX_SIZE))
    {
      if (Common_Tools::testRandomProbability (0.5f))
        velocity_.x *= -1.0f;
      if (Common_Tools::testRandomProbability (0.5f))
        velocity_.y *= -1.0f;
    }

    void run (olc::PixelGameEngine* engine)
    {
      display (engine);
      move (engine);
    }

    void display (olc::PixelGameEngine* engine)
    {
      static olc::vf2d center_s (engine->ScreenWidth () / 2.0f, engine->ScreenHeight () / 2.0f);
      float distance_f = center_s.x - position_.dist (center_s);
      float hue =
        Common_GL_Tools::map (distance_f, 0.0f, center_s.x, 0.0f, 360.0f);
      uint8_t alpha =
        static_cast<uint8_t> (Common_GL_Tools::map (distance_f, 0.0f, center_s.x, 0.0f, 255.0f));
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), alpha);
      engine->FillCircle (position_, size_, color);
    }

    void move (olc::PixelGameEngine* engine)
    {
      position_ += velocity_;

      if (position_.x >= static_cast<float> (engine->ScreenWidth () - size_))
        velocity_.x *= -1.0f;
      if (position_.x <= static_cast<float> (size_))
        velocity_.x *= -1.0f;
      if (position_.y >= static_cast<float> (engine->ScreenHeight () - size_))
        velocity_.y *= -1.0f;
      if (position_.y <= static_cast<float> (size_))
        velocity_.y *= -1.0f;
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
    int32_t   size_;
  };

  std::vector<ball> balls_;
};

#endif // PGE_319_H
