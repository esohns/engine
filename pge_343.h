#ifndef PGE_343_H
#define PGE_343_H

#include <vector>

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

class PGE_343
 : public olc::PixelGameEngine
{
 public:
  PGE_343 ();
  inline virtual ~PGE_343 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object ()
     : N_ (Common_Tools::getRandomNumber (0.0f, 100.0f))
     , t_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , nt_ (0.0f)
     , d_ (Common_Tools::getRandomNumber (10.0f, 80.0f))
     , tStep_ (Common_Tools::getRandomNumber (0.05f, 0.1f))
     , tDir_ (Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f)
     , ntStep_ (Common_Tools::getRandomNumber (0.01f, 0.05f))
     , dStep_ (Common_Tools::getRandomNumber (0.1f, 1.0f))
     , color_ ()
    {
      static olc::Pixel palette_a[7] = {olc::Pixel (0xFF, 0x00, 0xA2, 0x10), olc::Pixel (0xFF, 0xF3, 0x00, 0x10), olc::Pixel (0x6D, 0xF4, 0xE8, 0x10),
                                        olc::Pixel (0xAD, 0xFF, 0x6B, 0x10), olc::Pixel (0x44, 0x7F, 0xA6, 0x10), olc::Pixel (0xFF, 0x00, 0x78, 0x10),
                                        olc::Pixel (0x3D, 0xF7, 0xFF, 0x10)};
      int index_i = Common_Tools::getRandomNumber (0, 6);
      color_ = palette_a[index_i];
    }

    void update ()
    {
      t_ += tDir_ * tStep_;
      nt_ += ntStep_;
      d_ -= dStep_;

      if (d_ < 0.0f)
      {
        t_ = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
        d_ = Common_Tools::getRandomNumber (10.0f, 80.0f);
        dStep_ = Common_Tools::getRandomNumber (0.1f, 1.0f);
      } // end IF
    }

    void show (noise::module::Perlin* noise,
               //OpenSimplexNoise* noise,
               olc::PixelGameEngine* engine,
               float max)
    {
      static olc::vi2d center_s (engine->ScreenWidth () / 2,
                                 engine->ScreenHeight () / 2);

      float radius_f =
        Common_GL_Tools::map (static_cast<float> (noise->GetValue (nt_ + N_, 0.0, 0.0)), -1.0f, 1.0f, -max * 0.3f, max * 0.5f);
        //Common_GL_Tools::map (static_cast<float> (noise->Evaluate (nt_ + N_, 0.0, 0.0)), -1.0f, 1.0f, -max * 0.3f, max * 0.6f);
      float x = radius_f * std::cos (t_);
      float y = radius_f * std::sin (t_);

      engine->FillCircle (static_cast<int32_t> (x) + center_s.x, static_cast<int32_t> (y) + center_s.y,
                          static_cast<int32_t> (d_ / 4.0f), color_);
    }

    float      N_;
    float      t_;
    float      nt_;
    float      d_;
    float      tStep_;
    float      tDir_;
    float      ntStep_;
    float      dStep_;
    olc::Pixel color_;
  };

  noise::module::Perlin noise_;
//  OpenSimplexNoise      noise_;
  std::vector<object>   objects_;
  float                 max_;
};

#endif // PGE_343_H
