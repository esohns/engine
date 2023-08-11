#ifndef PGE_98_H
#define PGE_98_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

class PGE_98
 : public olc::PixelGameEngine
{
 public:
  PGE_98 ();
  inline virtual ~PGE_98 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class loop
  {
   public:
    loop (float diameter, float min, float max)
     : diameter_ (diameter)
     , min_ (min)
     , max_ (max)
    {}

    float value (float theta,
                 int frameCount,
                 noise::module::Perlin* noise_in)
    {
      double xoff = Common_GL_Tools::map (std::cos (static_cast<double> (theta)), -1.0, 1.0, 0.0, static_cast<double> (diameter_));
      double yoff = Common_GL_Tools::map (std::sin (static_cast<double> (theta)), -1.0, 1.0, 0.0, static_cast<double> (diameter_));
      float r =
        static_cast<float> (noise_in->GetValue (xoff, yoff, frameCount / 100.0f));
      return Common_GL_Tools::map (r, -1.0f, 1.0f, min_, max_);
    }

    float diameter_;
    float min_;
    float max_;
  };

  loop                  xLoop_;
  loop                  yLoop_;
  noise::module::Perlin noise_;
  float                 xMove_;
  float                 yMove_;
  float                 scaler_;
};

#endif // PGE_98_H
