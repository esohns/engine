#ifndef PGE_73_H
#define PGE_73_H

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

#include "common_ui_common.h"

class PGE_73
 : public olc::PixelGameEngine
{
 public:
  PGE_73 ();
  virtual ~PGE_73 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (const olc::vf2d& pos1_in, int32_t w1_in, int32_t h1_in, const olc::vf2d& pos2_in, int32_t w2_in, int32_t h2_in, int32_t t_in)
     : position_ (pos1_in)
     , position1_ (pos1_in)
     , position2_ (pos2_in)
     , w_ (w1_in)
     , h_ (h1_in)
     , w1_ (w1_in)
     , h1_ (h1_in)
     , w2_ (w2_in)
     , h2_ (h2_in)
     , t_ (-t_in)
     , t1_ (90)
     , t2_ (t1_ + 30)
     , t3_ (t2_ + 90)
     , t4_ (t3_ + 30)
     , cn_ (0)
     , color_ (olc::BLACK)
     , color0_ (olc::BLACK)
     , color1_ (colors_a[cn_])
    {}

    void update ()
    {
      if (0 < t_ && t_ < t1_)
      {
        float n =
          easeInOutQuart (Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (t1_ - 1), 0.0f, 1.0f));
        w_ =
          static_cast<int32_t> (Common_Math_Tools::lerp (static_cast<float> (w1_), static_cast<float> (w2_), n));
        h_ =
          static_cast<int32_t> (Common_Math_Tools::lerp (static_cast<float> (h1_), static_cast<float> (h2_), n));
        position_.x =
          Common_Math_Tools::lerp (position1_.x, position2_.x, n);
        position_.y =
          Common_Math_Tools::lerp (position1_.y, position2_.y, n);
        Common_GL_Color_t color1, color2;
        color1.r = color0_.r;
        color1.g = color0_.g;
        color1.b = color0_.b;
        color2.r = color1_.r;
        color2.g = color1_.g;
        color2.b = color1_.b;
        Common_GL_Color_t lerped_color =
          Common_GL_Tools::lerpRGB (color1, color2,
                                    Common_GL_Tools::map (static_cast<float> (std::sin (n * static_cast<float> (M_PI))), -0.01f, 1.0f, 0.0f, 1.0f));
        color_.r = lerped_color.r;
        color_.g = lerped_color.g;
        color_.b = lerped_color.b;
      } // end IF

      if (t_ == t1_)
      {
        cn_++;
        color1_ = colors_a[cn_ % 6];
      } // end IF

      if (t2_ < t_ && t_ < t3_)
      {
        float n =
          easeInOutQuart (Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t2_), static_cast<float> (t3_ - 1), 0.0f, 1.0f));
        w_ =
          static_cast<int32_t> (Common_Math_Tools::lerp (static_cast<float> (w2_), static_cast<float> (w1_), n));
        h_ =
          static_cast<int32_t> (Common_Math_Tools::lerp (static_cast<float> (h2_), static_cast<float> (h1_), n));
        position_.x =
          Common_Math_Tools::lerp (position2_.x, position1_.x, n);
        position_.y =
          Common_Math_Tools::lerp (position2_.y, position1_.y, n);
        Common_GL_Color_t color1, color2;
        color1.r = color0_.r;
        color1.g = color0_.g;
        color1.b = color0_.b;
        color2.r = color1_.r;
        color2.g = color1_.g;
        color2.b = color1_.b;
        Common_GL_Color_t lerped_color =
          Common_GL_Tools::lerpRGB (color1, color2,
                                    Common_GL_Tools::map (static_cast<float> (std::sin (n * static_cast<float> (M_PI))), -0.01f, 1.0f, 0.0f, 1.0f));
        color_.r = lerped_color.r;
        color_.g = lerped_color.g;
        color_.b = lerped_color.b;
      } // end IF

      if (t_ > t4_)
      {
        t_ = 0;
        cn_++;
        color1_ = colors_a[cn_ % 6];
      } // end IF

      t_++;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      int32_t x, y;
      x = static_cast<int32_t> (position_.x - w_ / 2.0f);
      y = static_cast<int32_t> (position_.y - h_ / 2.0f);
      engine_in->FillRect (x, y, w_, h_, color_);
    }

   private:
    olc::vf2d  position_;
    olc::vf2d  position1_;
    olc::vf2d  position2_;
    int32_t    w_;
    int32_t    h_;
    int32_t    w1_;
    int32_t    h1_;
    int32_t    w2_;
    int32_t    h2_;
    int32_t    t_;
    int32_t    t1_;
    int32_t    t2_;
    int32_t    t3_;
    int32_t    t4_;
    int32_t    cn_;
    olc::Pixel color_;
    olc::Pixel color0_;
    olc::Pixel color1_;

    float easeInOutQuart (float x_in)
    {
      return x_in < 0.5f ? 8.0f * x_in * x_in * x_in * x_in
                         : 1.0f - static_cast<float> (std::pow (-2.0f * x_in + 2.0f, 4)) / 2.0f;
    }

    static olc::Pixel colors_a[6];
  };
   
  std::vector<object*>             objects_;
  std::vector<Common_UI_Rectangle> array_1_;
  std::vector<Common_UI_Rectangle> array_2_;

  void recRect (int32_t, int32_t, int32_t, int32_t, int32_t, std::vector<Common_UI_Rectangle>&);
};

#endif // PGE_73_H
