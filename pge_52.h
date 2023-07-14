#ifndef PGE_52_H
#define PGE_52_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_52
 : public olc::PixelGameEngine
{
 public:
  PGE_52 ();
  virtual ~PGE_52 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class attractor
  {
   public:
    attractor (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , clockwise_ (Common_Tools::testRandomProbability (0.5f))
     , velocity_ (Common_Tools::getRandomNumber (-0.3f, 0.3f), Common_Tools::getRandomNumber (-0.3f, 0.3f))
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      position_ += velocity_;

      if (position_.x >= engine_in->ScreenWidth ()  || position_.x <= 0) velocity_.x *= -1.0f;
      if (position_.y >= engine_in->ScreenHeight () || position_.y <= 0) velocity_.y *= -1.0f;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, 25, clockwise_ ? olc::RED : olc::GREEN, 0xFF);
    }

    olc::vf2d position_;
    bool clockwise_;

   private:
    olc::vf2d velocity_;
  };

  class particle
  {
   public:
    particle (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , velocity_ (0.0f, 0.0f)
    {}

    void update (std::vector<attractor*>& attractors_in,
                 olc::PixelGameEngine* engine_in)
    {
      velocity_ = {0.0f, 0.0f};
      for (int i = 0; i < attractors_in.size (); i++)
      {
        olc::vf2d v = position_ - attractors_in[i]->position_;
        float magnitude_f = v.mag ();
        v = v.norm ();
        v *= 1.0f / magnitude_f;
        float angle_f = static_cast<float> (M_PI) / 2.0f;
        if (!attractors_in[i]->clockwise_)
          angle_f = -angle_f;
        olc::vf2d v_rot = { v.x * std::cos (angle_f) - v.y * std::sin (angle_f),
                            v.x * std::sin (angle_f) + v.y * std::cos (angle_f) };
        velocity_ += v_rot;
      } // end FOR
      velocity_ = velocity_.norm ();
      position_ += velocity_;
      
      //if (position_.x >= engine_in->ScreenWidth () || position_.x < 0 || position_.y >= engine_in->ScreenHeight () || position_.y < 0)
      //  position_ = {Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
      //               Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1))};
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (static_cast<int32_t> (position_.x), static_cast<int32_t> (position_.y), static_cast<int32_t> (position_.x - velocity_.x), static_cast<int32_t> (position_.y - velocity_.y), olc::WHITE, 0xFFFFFFFF);
    }

   private:
    olc::vf2d position_;
    olc::vf2d velocity_;
  };

  std::vector<attractor*> attractors_;
  std::vector<particle*>  particles_;
};

#endif // PGE_52_H
