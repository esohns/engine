#ifndef PGE_334_H
#define PGE_334_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_15.h"

class PGE_334
 : public olc::PixelGameEngine
{
 public:
  PGE_334 ();
  inline virtual ~PGE_334 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (olc::PixelGameEngine* engine)
     : position_ (static_cast<float> (Common_Tools::getRandomNumber (0, engine->ScreenWidth () - 1)),
                  static_cast<float> (Common_Tools::getRandomNumber (0, engine->ScreenHeight () - 1)))
     , incr_ (/*Common_Tools::getRandomNumber (0.0, static_cast<double> (engine->ScreenWidth () - 1))*/0.0)
     , theta_ (0.0f)
     , color_ ()
    {
      float adj =
        Common_GL_Tools::map (position_.y, 0.0f, static_cast<float> (engine->ScreenHeight () - 1), 205.0f, 0.0f);
      color_ = olc::Pixel (20U, static_cast<uint8_t> (adj), 255U);
    }

    void update (noise::module::Perlin* noise)
    {
      incr_ += ENGINE_PGE_334_DEFAULT_INCREMENT;
      theta_ =
        Common_GL_Tools::map (static_cast<float> (noise->GetValue (position_.x * 0.002, position_.y * 0.004, incr_)), -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI);
      position_.x += static_cast<float> (ENGINE_PGE_334_DEFAULT_SPEED) * std::cos (theta_);
      position_.y += static_cast<float> (ENGINE_PGE_334_DEFAULT_SPEED) * std::sin (theta_);
    }

    void show (noise::module::Perlin* noise,
               olc::PixelGameEngine* engine)
    {
      engine->Draw (position_, color_);
      static float n =
        Common_GL_Tools::map (static_cast<float> (noise->GetValue (0.01 * engine->ScreenWidth (), 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      olc::Pixel color (255, 255, 255, Common_Tools::getRandomNumber (0, 255));
      engine->Draw (int32_t(position_.x + n), int32_t(position_.y + Common_Tools::getRandomNumber (0.0f, 12.0f)), color);
    }

    void wrap (olc::PixelGameEngine* engine)
    {
      if (position_.x < 0.0f) position_.x = static_cast<float> (engine->ScreenWidth () - 1);
      else if (position_.x > static_cast<float> (engine->ScreenWidth () - 1)) position_.x = 0.0f;
      if (position_.y < 0.0f) position_.y = static_cast<float> (engine->ScreenHeight () - 1);
      else if (position_.y > static_cast<float> (engine->ScreenHeight () - 1)) position_.y = 0.0f;
    }

    olc::vf2d  position_;
    double     incr_;
    float      theta_;
    olc::Pixel color_;
  };

  void reset ();

  noise::module::Perlin noise_;
  std::vector<particle> particles_;
};

#endif // PGE_334_H
