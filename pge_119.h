#ifndef PGE_119_H
#define PGE_119_H

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "defines_4.h"

class PGE_119
 : public olc::PixelGameEngine
{
 public:
  PGE_119 ();
  virtual ~PGE_119 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class part
  {
   public:
    olc::vf2d position_;
    olc::vf2d speed_;
    olc::vf2d origin_;

    part (float x, float y)
     : position_ (x, y)
     , speed_ ()
     , origin_ (position_)
    {}

    void update (olc::vf2d m,
                 bool mode,
                 olc::PixelGameEngine* engine_in)
    {
      olc::vf2d tmp = origin_;
      tmp -= m;
      float d = tmp.mag ();
      float c = Common_GL_Tools::map (d, 0.0f, ENGINE_PGE_119_DEFAULT_DIST, 0.0f, static_cast<float> (M_PI));
      tmp = tmp.norm ();
      if (mode)
        tmp *= ENGINE_PGE_119_DEFAULT_DISTORTION * std::sin (c);
      float radius_f = 1.0f;
      if (d < ENGINE_PGE_119_DEFAULT_DIST)
      {
        radius_f = 1.0f + 10.0f * std::abs (std::cos (c / 2.0f));
        if (!mode)
          tmp *= ENGINE_PGE_119_DEFAULT_DISTORTION * std::sin (c);
      } // end IF
      else
        radius_f =
          Common_GL_Tools::map (std::min (d, static_cast<float> (engine_in->ScreenWidth ())), 0.0f, static_cast<float> (engine_in->ScreenWidth ()), 5.0f, 0.1f);

      olc::vf2d target (origin_.x + tmp.x, origin_.y + tmp.y);
      tmp = position_;
      tmp -=  target;
      tmp *= Common_GL_Tools::map (m.dist (position_), 0.0f, 2.0f * static_cast<float> (engine_in->ScreenWidth ()), -0.1f, -0.01f);
      speed_ += tmp;
      speed_ *= 0.87f;
      position_ += speed_;

      engine_in->FillCircle (position_, static_cast<int32_t> (radius_f / 2.0f), olc::BLACK);
    }
  };

  part** parts_;
  bool   mode_;
};

#endif // PGE_119_H
