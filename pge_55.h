#ifndef PGE_55_H
#define PGE_55_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_math_tools.h"

#include "common_gl_tools.h"

#include "defines_2.h"

class PGE_55
 : public olc::PixelGameEngine
{
 public:
  PGE_55 ();
  virtual ~PGE_55 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (int32_t id_in)
     : id_ (id_in)
    {}

    float activation (float t_in)
    {
      return ((1.0f - std::cos (2.0f * static_cast<float> (M_PI) * static_cast<float> (t_in))) / 2.0f);
    }

    void draw (unsigned int frameCount_in,
               olc::PixelGameEngine* engine_in,
               noise::module::Perlin* noise_in,
               double xOffset_in)
    {
      float t =
        (frameCount_in % ENGINE_PGE_55_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_55_DEFAULT_NUMBER_OF_FRAMES);
      float x0 =
        Common_Math_Tools::lerp (0.0f, static_cast<float> (engine_in->ScreenWidth ()), id_ / static_cast<float> (ENGINE_PGE_55_DEFAULT_NUMBER_OF_OBJECTS));

      float theta = static_cast<float> (M_PI_2);

      float xx = x0;
      float yy = 0.0f;

      int32_t Nt = 75;

      float step = static_cast<float> (engine_in->ScreenHeight ()) / static_cast<float> (Nt);

      float turn =
        Common_Math_Tools::lerp (0.0f, 0.4f, activation ((id_ / static_cast<float> (ENGINE_PGE_55_DEFAULT_NUMBER_OF_OBJECTS)) * t));

      engine_in->Draw (static_cast<int32_t> (xx), static_cast<int32_t> (yy), olc::WHITE);

      for (int i = 0; i <= Nt; i++)
      {
        float noise_f =
          static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (xOffset_in, 0.0, 0.0), -1.0, 1.0, 0.0, 1.0));
        float noise_2 =
          static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (0.2 * id_ / static_cast<double> (ENGINE_PGE_55_DEFAULT_NUMBER_OF_OBJECTS), 0.02 * i, 0.0), -1.0, 1.0, 0.0, 1.0));
        theta += turn * std::sin (100.0f * noise_f + 2.0f * static_cast<float> (M_PI) * (15.0f * noise_2 + t));
        xx += step * std::cos (theta);
        yy += step * std::sin (theta);

        float xx2 =
          Common_Math_Tools::lerp (xx, x0, (i / static_cast<float> (Nt)) * (i / static_cast<float> (Nt)) * (i / static_cast<float> (Nt)));
        float yy2 =
          Common_Math_Tools::lerp (yy, Common_Math_Tools::lerp (0.0f, static_cast<float> (engine_in->ScreenHeight ()), i / static_cast<float> (Nt)), std::max ((i / static_cast<float> (Nt)), 1.0f - std::sqrt (i / static_cast<float> (Nt))));

        engine_in->Draw (static_cast<int32_t> (xx2), static_cast<int32_t> (yy2), olc::WHITE);
      } // end FOR
    }

   private:
    int32_t id_;
  };

  noise::module::Perlin noise_;
  std::vector<object*>  objects_;
};

#endif // PGE_55_H
