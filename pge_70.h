#ifndef PGE_70_H
#define PGE_70_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

class PGE_70
 : public olc::PixelGameEngine
{
 public:
  PGE_70 ();
  virtual ~PGE_70 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class mobile
  {
   public:
    mobile (olc::PixelGameEngine* engine_in,
            noise::module::Perlin* noise_in,
            float a1_in, float a2_in, float a3_in)
     : position_ ()
     , prev_ (static_cast<float> (Common_Tools::getRandomNumber (0, engine_in->ScreenWidth () - 1)),
              static_cast<float> (Common_Tools::getRandomNumber (0, engine_in->ScreenHeight () - 1)))
     , velocity_ (200.0f, 200.0f)
     , color_ ()
    {
      position_ = prev_;

      color_.r =
        static_cast<uint8_t> (std::fmod (Common_GL_Tools::map (noise_in->GetValue (a1_in * std::cos (M_PI * position_.x * engine_in->ScreenWidth ()),
                                                                                   a1_in * std::sin (M_PI * position_.y / static_cast<double> (engine_in->ScreenHeight ())),
                                                                                   0.0), -1.0, 1.0, 0.0, 1.0) * 510.0,
                                         Common_Tools::getRandomNumber (0.0, 255.0)));
      color_.g =
        static_cast<uint8_t> (Common_GL_Tools::map (noise_in->GetValue (a2_in * std::sin (M_PI * position_.x * engine_in->ScreenWidth ()),
                                                                        a2_in * std::sin (M_PI * position_.y / static_cast<double> (engine_in->ScreenHeight ())),
                                                                        0.0), -1.0, 1.0, 0.0, 1.0) * 255.0);
      color_.b =
        static_cast<uint8_t> (Common_GL_Tools::map (noise_in->GetValue (a3_in * std::cos (M_PI * position_.x * engine_in->ScreenWidth ()),
                                                                        a3_in * std::cos (M_PI * position_.y / static_cast<double> (engine_in->ScreenHeight ())),
                                                                        0.0), -1.0, 1.0, 0.0, 1.0) * 255.0);
      color_.a = static_cast<uint8_t> (Common_Tools::getRandomNumber (50, 100));
    }

    void update (olc::PixelGameEngine* engine_in,
                 noise::module::Perlin* noise_in,
                 float a2_in, float a3_in, float a4_in, float a5_in)
    {
      velocity_ = {1.0f - 2.0f * static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (a4_in + a2_in * std::sin (static_cast<float> (M_PI) * 2.0f * position_.x / static_cast<float> (engine_in->ScreenWidth ())),
                                                                                               a4_in + a2_in * std::sin (static_cast<float> (M_PI) * 2.0f * position_.y / static_cast<float> (engine_in->ScreenHeight ())),
                                                                                               0.0),
                                                                           -1.0, 1.0, 0.0, 1.0)),
                   1.0f - 2.0f * static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (a2_in + a3_in * std::cos (static_cast<float> (M_PI) * 2.0f * position_.x / static_cast<float> (engine_in->ScreenWidth ())),
                                                                                               a4_in + a3_in * std::cos (static_cast<float> (M_PI) * 2.0f * position_.y / static_cast<float> (engine_in->ScreenHeight ())),
                                                                                               0.0),
                                                                           -1.0, 1.0, 0.0, 1.0))};
      velocity_ *= a5_in;
      float angle_f =
        static_cast<float> (std::sin (100.0) * Common_GL_Tools::map (noise_in->GetValue (a4_in + a3_in * std::sin (static_cast<float> (M_PI) * 2.0f * position_.x / static_cast<float> (engine_in->ScreenWidth ())),
                                                                                                                                   Common_Tools::getRandomNumber (0.0, 2.0), Common_Tools::getRandomNumber (0.0, 2.0)),
                                                                                                               -1.0, 1.0, 0.0, 1.0));
      olc::vf2d rotated;
      rotated.x = std::cos (angle_f) * velocity_.x - std::sin (angle_f) * velocity_.y;
      rotated.y = std::sin (angle_f) * velocity_.x + std::cos (angle_f) * velocity_.y;
      velocity_ = rotated;
      prev_ = position_;
      position_ += velocity_;
    
      if (position_.x > static_cast<float> (engine_in->ScreenWidth () - 1)  || position_.x < 0.0f ||
          position_.y > static_cast<float> (engine_in->ScreenHeight () - 1) || position_.y < 0.0f)
      {
        prev_ = {static_cast<float> (Common_Tools::getRandomNumber (0, engine_in->ScreenWidth () - 1)),
                 static_cast<float> (Common_Tools::getRandomNumber (0, engine_in->ScreenHeight () - 1))};
        position_ = prev_;
      } // end IF
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (static_cast<int32_t> (prev_.x), static_cast<int32_t> (prev_.y),
                           static_cast<int32_t> (position_.x), static_cast<int32_t> (position_.y),
                           color_, 0xFFFFFFFF);
    }

    void run (olc::PixelGameEngine* engine_in,
              noise::module::Perlin* noise_in,
              float a2_in, float a3_in, float a4_in, float a5_in)
    {
      update (engine_in, noise_in, a2_in, a3_in, a4_in, a5_in);
      show (engine_in);
    }

    olc::vf2d  position_;
    olc::vf2d  prev_;
    olc::vf2d  velocity_;
    olc::Pixel color_;
  };
   
  noise::module::Perlin noise_;
  float                 a1_;
  float                 a2_;
  float                 a3_;
  float                 a4_;
  float                 a5_;
  float                 amax_;
  std::vector<mobile*>  mobiles_;

  void reset ();
};

#endif // PGE_70_H
