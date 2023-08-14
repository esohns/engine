#ifndef PGE_105_H
#define PGE_105_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

class PGE_105
 : public olc::PixelGameEngine
{
 public:
  PGE_105 ();
  virtual ~PGE_105 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class shape
  {
   public:
    shape (float curveSize_in)
     : position_ ()
     , r_ (curveSize_in + Common_Tools::getRandomNumber (-30.0f, 30.0f))
     , t_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , speed_ (Common_Tools::getRandomNumber (0.1f, 0.5f))
     , hue_ (Common_Tools::getRandomNumber (0.0f, 360.0f))
     , radius_ (Common_Tools::testRandomProbability (0.5) ? 5 : 10)
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      hue_ =
        Common_GL_Tools::map (static_cast<float> (std::fmod (t_ + static_cast<float> (M_PI), 2.0f * static_cast<float> (M_PI))), 0.0f, 2.0f * static_cast<float> (M_PI), 0.0f, 360.0f);

      position_.x = r_ * std::sin (t_ * 4.0f) * std::cos (3.0f * t_) + engine_in->ScreenWidth () / 2.0f;
      position_.y = r_ * std::sin (t_) * std::sin (4.0f * t_) + engine_in->ScreenHeight () / 2.0f;

      t_ += radius_ * 0.02f;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue_, 0.5f, 1.0f, r, g, b);
      olc::Pixel color_s;
      color_s.r = static_cast<uint8_t> (r * 255.0f);
      color_s.g = static_cast<uint8_t> (g * 255.0f);
      color_s.b = static_cast<uint8_t> (b * 255.0f);
      color_s.a = 255;
      engine_in->FillCircle (position_, radius_, color_s);
    }

    olc::vf2d position_;
    float     r_;
    float     t_;
    float     speed_;
    float     hue_;
    int32_t   radius_;
  };

  float               curveSize_;
  std::vector<shape*> shapes_;
};

#endif // PGE_105_H
