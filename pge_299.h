#ifndef PGE_299_H
#define PGE_299_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

class PGE_299
 : public olc::PixelGameEngine
{
 public:
  PGE_299 ();
  virtual ~PGE_299 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

  static float easeInOutQuart (float);

 private:
  struct rectangle
  {
    olc::vf2d position;
    int32_t   w;
    int32_t   h;
  };
  std::vector<struct rectangle> rectangles_;

  class object
  {
   public:
    virtual void show (olc::PixelGameEngine*) = 0;
    virtual void move () = 0;
  };
  std::vector<object*>          objects_;
  int                           ccc_;

  class CRR01
   : public object
  {
   public:
    CRR01 (olc::vf2d& pos, int w, int h, int t, olc::Pixel& col)
     : object ()
     , position_ (pos)
     , w_ (0)
     , h_ (0)
     , color_ (col)
     , rc_ (Common_Tools::testRandomProbability (0.5f))
     , d_ (0.0f)
     , d1_ (std::min (w, h) * 0.1f)
     , w0_ (static_cast<int> (d1_))
     , h0_ (static_cast<int> (d1_))
     , w1_ (w)
     , h1_ (h)
     , t_ (t)
     , t1_ (30)
     , t2_ (t1_ + 30)
     , t3_ (t2_ + 30)
     , t4_ (t3_ + 60)
    {
      if (rc_)
        w_ = w0_;
      else
        h_ = h0_;
    }

    virtual void show (olc::PixelGameEngine* engine)
    {
      int32_t x = static_cast<int32_t> (position_.x - w_ / 2.0f);
      int32_t y = static_cast<int32_t> (position_.y - h_ / 2.0f);

      if (0 < t_ && t_ < t2_)
        engine->FillCircle (x, y, static_cast<int32_t> (d_ / 2.0f), color_);
      
      if (t_ > t1_)
        engine->FillRect (x, y, w_, h_, color_);
    }

    virtual void move ()
    {
      if (0 < t_ && t_ < t1_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (t1_ - 1), 0.0f, 1.0f);
        d_ = Common_Math_Tools::lerp (0.0f, d1_, easeInOutQuart (n));
      } // end IF
      else if (t1_ < t_ && t_ < t2_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t1_), static_cast<float> (t2_ - 1), 0.0f, 1.0f);
        if (rc_)
          w_ = static_cast<int32_t> (Common_Math_Tools::lerp (0.0f, static_cast<float> (w1_), easeInOutQuart (n)));
        else
          h_ = static_cast<int32_t> (Common_Math_Tools::lerp (0.0f, static_cast<float> (h1_), easeInOutQuart (n)));
      } // end ELSE IF
      else if (t2_ < t_ && t_ < t3_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t2_), static_cast<float> (t3_ - 1), 0.0f, 1.0f);
        if (rc_)
          h_ = static_cast<int32_t> (Common_Math_Tools::lerp (static_cast<float> (h0_), static_cast<float> (h1_), easeInOutQuart (n)));
        else
          w_ = static_cast<int32_t> (Common_Math_Tools::lerp (static_cast<float> (w0_), static_cast<float> (w1_), easeInOutQuart (n)));
      } // end ELSE IF

      ++t_;
    }

    olc::vf2d  position_;
    int32_t    w_;
    int32_t    h_;
    olc::Pixel color_;
    bool       rc_;
    float      d_;
    float      d1_;
    int32_t    w0_;
    int32_t    h0_;
    int32_t    w1_;
    int32_t    h1_;
    int        t_;
    int        t1_;
    int        t2_;
    int        t3_;
    int        t4_;
  };

  class CRR02
   : public object
  {
   public:
    CRR02 (olc::vf2d& pos, int w, int h, int t, olc::Pixel& col)
     : object ()
     , position_ (pos)
     , w_ (w)
     , h_ (h)
     , color_ (col)
     , alpha_ (0)
     , t_ (t)
     , t1_ (15)
     , t2_ (t1_ + 15)
     , t3_ (t2_ + 15)
     , t4_ (t3_ + 15)
     , t5_ (t3_ + 50)
     , p1_ (0)
     , p2_ (0)
     , p3_ (0)
     , p4_ (0)
     , rx_ (Common_Tools::testRandomProbability (0.5f) ? -1 : 1)
     , ry_ (Common_Tools::testRandomProbability (0.5f) ? -1 : 1)
    {}

    virtual void show (olc::PixelGameEngine* engine)
    {
      int32_t x = static_cast<int32_t> (position_.x - w_ / 2.0f);
      int32_t y = static_cast<int32_t> (position_.y - h_ / 2.0f);
      //scale(rx_, ry_);
      //strokeWeight(3);
      color_.a = 255;
      if (p1_ > 1)
        engine->DrawLine (rx_ * (x + -(w_ / 2)), ry_ * (y + -(h_ / 2)), rx_ * (x + -(w_ / 2) + p1_), ry_ * (y + -(h_ / 2)),       color_, 0xFFFFFFFF);
      if (p2_ > 1)
        engine->DrawLine (rx_ * (x + (w_ / 2)) , ry_ * (y + -(h_ / 2)), rx_ * (x + (w_ / 2)),        ry_ * (y + -(h_ / 2) + p2_), color_, 0xFFFFFFFF);
      if (p3_ > 1)
        engine->DrawLine (rx_ * (x + (w_ / 2)) , ry_ * (y +  (h_ / 2)), rx_ * (x + (w_ / 2) - p3_),  ry_ * (y + (h_ / 2)),        color_, 0xFFFFFFFF);
      if (p4_ > 1)
        engine->DrawLine (rx_ * (x + -(w_ / 2)), ry_ * (y +  (h_ / 2)), rx_ * (x + -(w_ / 2)),       ry_ * (y + (h_ / 2) - p4_),  color_, 0xFFFFFFFF);
      color_.a = alpha_;

      engine->FillRect (x, y, w_, h_, color_);
    }

    virtual void move ()
    {
      if (0 < t_ && t_ < t1_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (t1_ - 1), 0.0f, 1.0f);
        p1_ = static_cast<int> (Common_Math_Tools::lerp (0.0f, static_cast<float> (w_), n));
      } // end IF
      else if (t1_ < t_ && t_ < t2_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t1_), static_cast<float> (t2_ - 1), 0.0f, 1.0f);
        p2_ = static_cast<int> (Common_Math_Tools::lerp (0.0f, static_cast<float> (h_), n));
      } // end ELSE IF
      else if (t2_ < t_ && t_ < t3_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t2_), static_cast<float> (t3_ - 1), 0.0f, 1.0f);
        p3_ = static_cast<int> (Common_Math_Tools::lerp (0.0f, static_cast<float> (w_), n));
      } // end ELSE IF
      else if (t3_ < t_ && t_ < t4_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t3_), static_cast<float> (t4_ - 1), 0.0f, 1.0f);
        p4_ = static_cast<int> (Common_Math_Tools::lerp (0.0f, static_cast<float> (h_), n));
      } // end ELSE IF
      else if (t4_ < t_ && t_ < t5_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), static_cast<float> (t4_), static_cast<float> (t5_ - 1), 0.0f, 1.0f);
        alpha_ = static_cast<int> (Common_Math_Tools::lerp (0.0f, 255.0f, n));
      } // end ELSE IF

      ++t_;
    }

    olc::vf2d  position_;
    int32_t    w_;
    int32_t    h_;
    olc::Pixel color_;
    uint8_t    alpha_;
    int        t_;
    int        t1_;
    int        t2_;
    int        t3_;
    int        t4_;
    int        t5_;
    int        p1_;
    int        p2_;
    int        p3_;
    int        p4_;
    int        rx_;
    int        ry_;
  };

  void initialize ();
  void divideRect (int, int, int, int, int);

  inline static olc::Pixel colors_a[6] = {olc::Pixel (0x22, 0x22, 0x22), olc::Pixel (0xff, 0xff, 0xff), olc::Pixel (0x1c, 0x5d, 0x99),
                                          olc::Pixel (0x63, 0x9f, 0xab), olc::Pixel (0xef, 0xa0, 0x0b), olc::Pixel (0xd6, 0x51, 0x08)};
};

#endif // PGE_299_H
