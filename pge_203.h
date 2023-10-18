#ifndef PGE_203_H
#define PGE_203_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_9.h"

class PGE_203
 : public olc::PixelGameEngine
{
 public:
  PGE_203 ();
  virtual ~PGE_203 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class ball
  {
   public:
    ball (olc::PixelGameEngine* engine_in,
          olc::Pixel& color_inout)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , radius_ (Common_Tools::getRandomNumber (ENGINE_PGE_203_DEFAULT_MIN_RADIUS, ENGINE_PGE_203_DEFAULT_MAX_RADIUS))
    {
      color_inout.r = Common_Tools::getRandomNumber (0, 255);
      color_inout.g = Common_Tools::getRandomNumber (0, 255);
      color_inout.b = Common_Tools::getRandomNumber (0, 255);
      color_inout.a = 255;
    }

    void show (olc::PixelGameEngine* engine_in,
               olc::Pixel& color_in)
    {
      engine_in->FillCircle (position_, radius_, color_in);
    }

    //void update (noise::module::Perlin* noise_in,
    void update (OpenSimplexNoise* noise_in,
                 olc::PixelGameEngine* engine_in,
                 bool hexCurve_in)
    {
      float n =
        //static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (position_.x / engine_in->ScreenWidth (), position_.y / engine_in->ScreenHeight (), 0.0),
        static_cast<float> (Common_GL_Tools::map (noise_in->Evaluate (position_.x / engine_in->ScreenWidth (), position_.y / engine_in->ScreenHeight (), 0.0),
                                                  -1.0, 1.0, 0.0, 1.0));
      float angle_f = hexCurve_in ? (int(n * 10.0f) * static_cast<float> (M_PI)) / 3.0f : n * 10.0f;
      position_.x += std::cos (angle_f) * radius_ * 2.0f;
      position_.y += std::sin (angle_f) * radius_ * 2.0f;
    }

    bool isFrameOut (olc::PixelGameEngine* engine_in)
    {
      return (position_.x < -radius_ ||
              position_.y < -radius_ ||
              position_.x > engine_in->ScreenWidth () + radius_ ||
              position_.y > engine_in->ScreenHeight () + radius_);
    }

    bool isOverlapped (std::vector<ball>& history_in)
    {
      bool condition_b = false;
      for (std::vector<ball>::iterator iterator = history_in.begin ();
           iterator != history_in.end ();
           ++iterator)
      {
        float d = position_.dist ((*iterator).position_);
        if (d + 1e-10f > (*iterator).radius_ + radius_) continue;
        if (d < (*iterator).radius_ - radius_) continue;
        condition_b = true;
        break;
      } // end FOR
      return condition_b;
    }

    olc::vf2d position_;
    int32_t   radius_;
  };

  ball*                 ball_;
  olc::Pixel            color_; // current-
  bool                  hexCurve_;
  std::vector<ball>     history_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
};

#endif // PGE_203_H
