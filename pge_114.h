#ifndef PGE_114_H
#define PGE_114_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_114
 : public olc::PixelGameEngine
{
 public:
  PGE_114 ();
  virtual ~PGE_114 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    olc::vf2d  position_;
    float      radius_;
    olc::Pixel color_;
    int32_t    i_;
    int32_t    j_;

    particle (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , radius_ (Common_Tools::getRandomNumber (1.0f,  5.0f))
     , color_ ()
     , i_ (1)
     , j_ (1)
    {
      int j = Common_Tools::getRandomNumber (0, 3);
      if (j == 0)
        color_ = {0x05, 0xCD, 0xE5, 95};
      else if (j == 1)
        color_ = {0xFF, 0xB8, 0x03, 95};
      else if (j == 2)
        color_ = {0xFF, 0x03, 0x5B, 95};
      else if (j == 3)
        color_ = {0x3D, 0x3E, 0x3E, 95};
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      position_.x = position_.x + j_ * 0.01f;
      position_.y = position_.y + i_ * 0.01f;
      if (position_.y > static_cast<int32_t> (engine_in->ScreenHeight () - radius_)) i_ = -1;
      if (position_.y < 0 + static_cast<int32_t> (radius_)) i_ = 1;
      if (position_.x > static_cast<int32_t> (engine_in->ScreenWidth () - radius_)) j_ = -1;
      if (position_.x < 0 + static_cast<int32_t> (radius_)) j_ = 1;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (radius_), color_);
    }
  };

  std::vector<particle*> particles_;
  bool                   flag_;
};

#endif // PGE_114_H
