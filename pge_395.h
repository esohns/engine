#ifndef PGE_395_H
#define PGE_395_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_18.h"

class PGE_395
 : public olc::PixelGameEngine
{
 public:
  PGE_395 ();
  inline virtual ~PGE_395 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (olc::PixelGameEngine* engine, int frameCount)
     : position_ (static_cast<float> (Common_Tools::getRandomNumber (0, engine->ScreenWidth () - 1)),
                  static_cast<float> (Common_Tools::getRandomNumber (0, engine->ScreenHeight () - 1)))
     , color_ ()
    {
      int n = static_cast<int> (position_.x) + static_cast<int> (position_.y) - 2 * frameCount;
      int rem = ((n % ENGINE_PGE_395_DEFAULT_CYCLE) + ENGINE_PGE_395_DEFAULT_CYCLE) % ENGINE_PGE_395_DEFAULT_CYCLE;
      color_ = rem <= ENGINE_PGE_395_DEFAULT_CYCLE / 2 ? olc::BLACK : olc::WHITE;
    }

    void update (//noise::module::Perlin* noise,
                 OpenSimplexNoise* noise,
                 olc::PixelGameEngine* engine)
    {
      static double screen_width_d = static_cast<double> (engine->ScreenWidth () - 1);
      static double screen_height_d = static_cast<double> (engine->ScreenHeight () - 1);

      float angle_f =
//        Common_GL_Tools::map (static_cast<float> (noise->GetValue (position_.x / screen_width_d, position_.y / screen_height_d, 0.0)),
        Common_GL_Tools::map (static_cast<float> (noise->Evaluate (position_.x / screen_width_d, position_.y / screen_height_d)),
                              -1.0f, 1.0f, 0.0f, 1.0f) * 100.0f;
      olc::vf2d to (std::cos (angle_f) * ENGINE_PGE_395_DEFAULT_STEP, std::sin (angle_f) * ENGINE_PGE_395_DEFAULT_STEP);
      engine->DrawLine (position_, position_ + to, color_, 0xFFFFFFFF);
      position_ += to;
    }

    olc::vf2d position_;
    olc::Pixel color_;
  };

  std::vector<agent>    agents_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
};

#endif // PGE_395_H
