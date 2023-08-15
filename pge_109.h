#ifndef PGE_109_H
#define PGE_109_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_4.h"

class PGE_109
 : public olc::PixelGameEngine
{
 public:
  PGE_109 ();
  virtual ~PGE_109 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class ball
  {
   public:
    ball (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , w_ (Common_Tools::getRandomNumber (1.0f / ENGINE_PGE_109_DEFAULT_THOLD, ENGINE_PGE_109_DEFAULT_THOLD))
     , Xv_ (0.0f)
     , Yv_ (0.0f)
     , pX_ (position_.x)
     , pY_ (position_.y)
    {}

    void show (olc::PixelGameEngine* engine_in,
               float mX, float mY)
    {
      if (!engine_in->GetMouse (0).bHeld)
      {
        Xv_ /= ENGINE_PGE_109_DEFAULT_SPIFAC;
        Yv_ /= ENGINE_PGE_109_DEFAULT_SPIFAC;
      } // end IF
      Xv_ += ENGINE_PGE_109_DEFAULT_DRAG * (mX - position_.x) * w_;
      Yv_ += ENGINE_PGE_109_DEFAULT_DRAG * (mY - position_.y) * w_;
      position_.x += Xv_;
      position_.y += Yv_;

      engine_in->DrawLine (static_cast<int32_t> (position_.x), static_cast<int32_t> (position_.y), static_cast<int32_t> (pX_), static_cast<int32_t> (pY_), {255, 255, 255, 5}, 0xFFFFFFFF);

      pX_ = position_.x;
      pY_ = position_.y;
    }

    olc::vf2d position_;
    float     w_;
    float     Xv_;
    float     Yv_;
    float     pX_;
    float     pY_;
  };

  std::vector<ball*> balls_;
  float              mX_;
  float              mY_;
};

#endif // PGE_109_H
