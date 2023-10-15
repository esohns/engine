#ifndef PGE_200_H
#define PGE_200_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "defines_9.h"

class PGE_200
 : public olc::PixelGameEngine
{
 public:
  PGE_200 ();
  inline virtual ~PGE_200 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (float center_x, float center_y, float tilt, double ns, float rShift, int topNum)
     : center_ (center_x, center_y)
     , tilt_ (tilt)
     , rAng_ (0.0f)
     , ns_ (ns)
     , rShift_ (rShift)
     , topNum_ (topNum)
     , xy_ ()
    {}

    void update (olc::PixelGameEngine* engine_in,
                 noise::module::Perlin* noise_in)
    {
      rAng_ += 2.0f * static_cast<float> (M_PI) / 720.0f;
      float r =
        (engine_in->ScreenWidth () * 3.0f + rShift_) * static_cast<float> (std::pow (1.0f - std::cos (rAng_), ENGINE_PGE_200_DEFAULT_D));
      xy_.clear ();
      for (int i = 0; i < topNum_; i++)
      {
        olc::vf2d pos;
        pos.x =
          center_.x + r * std::cos (tilt_ + (2.0f * static_cast<float> (M_PI) / static_cast<float> (topNum_) * i)) * (Common_GL_Tools::map (static_cast<float> (noise_in->GetValue (ns_ + i / 1.0f, 0.0, 0.0)),
                                                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f) * 0.8f + 0.2f);
        pos.y =
          center_.y + r * std::sin (tilt_ + (2.0f * static_cast<float> (M_PI) / static_cast<float> (topNum_) * i)) * (Common_GL_Tools::map (static_cast<float> (noise_in->GetValue (ns_ + i / 1.0f, 0.0, 0.0)),
                                                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f) * 0.8f + 0.2f);
        xy_.push_back (pos);
      } // end FOR
    }

    olc::vf2d              center_;
    float                  tilt_;
    float                  rAng_;
    double                 ns_;
    float                  rShift_;
    int                    topNum_;
    std::vector<olc::vf2d> xy_;
  };

  std::vector<object>   objects_;
  int                   count_;
  float                 rRate_;
  noise::module::Perlin noise_;
  std::vector<double>   noiseInit_;
  float                 noiseSpeed_;
  int                   objectNum_;
  std::vector<float>    RShift_;
  std::vector<int>      topNum_;
  int                   freqCount_;
};

#endif // PGE_200_H
