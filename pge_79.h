#ifndef PGE_79_H
#define PGE_79_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "noise/noise.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_3.h"

class PGE_79
 : public olc::PixelGameEngine
{
 public:
  PGE_79 ();
  virtual ~PGE_79 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (float x_in, float y_in)
     : position_ (x_in, y_in)
     , size_ (Common_Tools::getRandomNumber (0.5f, 2.0f))
     , speed_ (Common_Tools::getRandomNumber (-5.0f, 5.0f))
     , color_ (olc::BLACK)
    {
      Common_GL_Color_t color = Common_GL_Tools::randomColor ();
      color_.r = color.r;
      color_.g = color.g;
      color_.b = color.b;
      //color_.a = 255;
    }

    void update (noise::module::Perlin* noise_in,
                 int frameCount_in,
                 olc::PixelGameEngine* engine_in)
    {
      float angle_f =
        Common_GL_Tools::map (static_cast<float> (noise_in->GetValue (position_.x / ENGINE_PGE_79_DEFAULT_NOISE_SCALE_X, position_.y / ENGINE_PGE_79_DEFAULT_NOISE_SCALE_Y, 0.001 * frameCount_in)), -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI);
      position_.x += std::cos (angle_f) * speed_;
      position_.y += std::sin (angle_f) * speed_;

      if (position_.x < 0.0f)
        position_.x += static_cast<float> (engine_in->ScreenWidth ());
      if (position_.x >= static_cast<float> (engine_in->ScreenWidth ()))
        position_.x -= static_cast<float> (engine_in->ScreenWidth ());
      if (position_.y < 0.0f)
        position_.y += static_cast<float> (engine_in->ScreenHeight ());
      if (position_.y >= static_cast<float> (engine_in->ScreenHeight ()))
        position_.y -= static_cast<float> (engine_in->ScreenHeight ());
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (size_ * 2.0f), color_);
    }

    olc::vf2d  position_;
    float      size_;
    float      speed_;
    olc::Pixel color_;
  };

  std::vector<agent*>   agents_;
  noise::module::Perlin noise_;
};

#endif // PGE_79_H
