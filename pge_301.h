#ifndef PGE_301_H
#define PGE_301_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

class PGE_301
 : public olc::PixelGameEngine
{
 public:
  PGE_301 ();
  inline virtual ~PGE_301 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct spot
  {
    olc::vf2d position;
    float     d;
  };
  std::vector<struct spot>      spots_;

  struct rectangle
  {
    olc::vf2d position;
    int       w;
    int       h;
  };
  std::vector<struct rectangle> rectangles_;

  class object
  {
   public:
    object (olc::vf2d& pos, int w1, int h1, olc::vf2d& pos2, int w2, int h2, int t)
     : position_ (pos)
     , w_ (w1)
     , h_ (h1)
     , position1_ (pos)
     , w1_ (w1)
     , h1_ (h1)
     , position2_ (pos2)
     , w2_ (w2)
     , h2_ (h2)
     , r_ (std::max (w1, w2))
     , color_ (olc::BLACK)
     , t_ (t)
     , t0_ (t)
     , t1_ (50)
     , t2_ (t1_ + 100)
     , t3_ (t2_ + 50)
    {
      static olc::Pixel colors_a[8] = {olc::Pixel (0xe6, 0x30, 0x2b), olc::Pixel (0xfd, 0x78, 0x00), olc::Pixel (0xfb, 0xd4, 0x00), olc::Pixel (0x51, 0xb7, 0x2d),
                                       olc::Pixel (0x2a, 0xbd, 0xe4), olc::Pixel (0x55, 0x34, 0xeb), olc::Pixel (0xf4, 0x77, 0xc3), olc::Pixel (0xff, 0xff, 0xff)};
      int index_i = Common_Tools::getRandomNumber (0, 7);
      color_ = colors_a[index_i];
    }

    void show (olc::PixelGameEngine* engine)
    {
      int32_t x = static_cast<int32_t> (position_.x - w_ / 2);
      int32_t y = static_cast<int32_t> (position_.y - h_ / 2);
      engine->FillRect (x, y, w_, h_, color_);
    }

    float easeInOutCirc (float x)
    {
      return x < 0.5f ? (1.0f - std::sqrt (1.0f - static_cast<float> (std::pow (2.0f * x, 2)))) / 2.0f
                      : (std::sqrt (1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 2))) + 1.0f) / 2.0f;
    }

    void move ()
    {
      t_++;
      if (0 < t_ && t_ < t1_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (t1_ - 1), 0.0f, 1.0f);
        position_.x = Common_Math_Tools::lerp (position1_.x, position2_.x, easeInOutCirc (n));
        position_.y = Common_Math_Tools::lerp (position1_.y, position2_.y, easeInOutCirc (n));
        w_ =
          static_cast<int> (Common_Math_Tools::lerp (static_cast<float> (w1_), static_cast<float> (w2_), easeInOutCirc (n)));
        h_ =
          static_cast<int> (Common_Math_Tools::lerp (static_cast<float> (h1_), static_cast<float> (h2_), easeInOutCirc (n)));
        r_ =
          static_cast<int> (Common_Math_Tools::lerp (static_cast<float> (std::max (w1_, h1_)) / 2.0f, 0.0f, easeInOutCirc(n)));
      } // end IF

      if (t2_ < t_ && t_ < t3_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t2_), static_cast<float> (t3_ - 1), 0.0f, 1.0f);
        position_.x = Common_Math_Tools::lerp (position2_.x, position1_.x, easeInOutCirc (n));
        position_.y = Common_Math_Tools::lerp (position2_.y, position1_.y, easeInOutCirc (n));
        w_ =
          static_cast<int> (Common_Math_Tools::lerp (static_cast<float> (w2_), static_cast<float> (w1_), easeInOutCirc (n)));
        h_ =
          static_cast<int> (Common_Math_Tools::lerp (static_cast<float> (h2_), static_cast<float> (h1_), easeInOutCirc (n)));
        r_ =
          static_cast<int> (Common_Math_Tools::lerp (0.0f, static_cast<float> (std::max (w1_, h1_)) / 2.0f, easeInOutCirc(n)));
      } // end IF

      if (t_ > t3_)
        t_ = -100;
    }

    olc::vf2d  position_;
    int        w_;
    int        h_;
    olc::vf2d  position1_;
    int        w1_;
    int        h1_;
    olc::vf2d  position2_;
    int        w2_;
    int        h2_;
    int        r_;
    olc::Pixel color_;
    int        t_;
    int        t0_;
    int        t1_;
    int        t2_;
    int        t3_;
  };
  std::vector<object>           objects_;

  void recRect (int, int, int, int, int, std::vector<struct rectangle>&);
};

#endif // PGE_301_H
