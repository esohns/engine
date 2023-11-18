#ifndef PGE_244_H
#define PGE_244_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

class PGE_244
 : public olc::PixelGameEngine
{
 public:
  PGE_244 ();
  virtual ~PGE_244 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class block
  {
   public:
    block (float x, float y, float tx, float ty, bool isSquare)
     : position_ (x, y)
     , target_ (tx, ty)
     , isSquare_ (isSquare)
    {}

    void show (olc::PixelGameEngine* engine_in)
    {
      if (isSquare_)
      {
        static olc::Pixel color (200, 50, 50);
        engine_in->FillRect (static_cast<int32_t> (position_.x) - 5, static_cast<int32_t> (position_.y) - 5, 10, 10, color);
      } // end IF
      else
      {
        static olc::Pixel color (50, 50, 200);
        engine_in->FillCircle (position_, 5, color);
      } // end ELSE
    }

    bool inPlace ()
    {
      if (position_.dist (target_) < 0.5)
      {
        position_ = target_;
        return true;
      } // end IF

      return false;
    }

    olc::vf2d position_;
    olc::vf2d target_;
    bool      isSquare_;
  };

  class cursor
  {
   public:
    cursor (float x, float y, block* block)
     : start_ (x, y)
     , position_ (x, y)
     , block_ (block)
     , target_ (block->position_.x, block->position_.y)
     , carry_ (false)
     , speed_ (5.0f)
     , a_ (Common_Tools::getRandomNumber (static_cast<float> (M_PI), 2.0f * static_cast<float> (M_PI)))
     , t_ (Common_Tools::getRandomNumber (0.0f, 1000.0f))
     , done_ (false)
    {}

    void update (noise::module::Perlin* noise)
    {
      olc::vf2d v = target_ - position_;
      olc::vf2d temp = v.norm ();
      float angle_with_x = std::atan2 (temp.y, temp.x);
      float h =
        angle_with_x + (Common_GL_Tools::map (static_cast<float> (noise->GetValue (t_, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * a_;
      t_ += 0.01f;
      olc::vf2d v2 (std::cos (h), std::sin (h));
      v2 = v2.norm ();
      v2 *= std::min (speed_, target_.dist (position_) * 0.25f);
      position_ += v2;

      if (!done_ && target_.dist (position_) < 1.0f)
      {
        carry_ = true;
        target_ = block_->target_;
      } // end IF

      if (carry_)
      {
        block_->position_ = position_;
      } // end IF

      if (block_->inPlace ())
      {
        target_ = start_;
        carry_ = false;
        done_ = true;
      } // end IF
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d v2 = position_;
      v2.x += 13.0f; v2.y += 13.0f;
      engine_in->DrawLine (position_, v2, olc::BLACK, 0xFFFFFFFF);
      olc::vf2d v3 = position_;
      v3.x += 7.0f; v3.y += 13.0f;
      engine_in->DrawLine (v2, v3, olc::BLACK, 0xFFFFFFFF);
      v2 = position_;
      v2.x += 11.0f; v2.y += 20.0f;
      engine_in->DrawLine (v3, v2, olc::BLACK, 0xFFFFFFFF);
      v3 = position_;
      v3.x += 8.0f; v3.y += 20.0f;
      engine_in->DrawLine (v2, v3, olc::BLACK, 0xFFFFFFFF);
      v2 = position_;
      v2.x += 4.0f; v2.y += 13.0f;
      engine_in->DrawLine (v3, v2, olc::BLACK, 0xFFFFFFFF);
      v3 = position_;
      v3.x += 0.0f; v3.y += 17.0f;
      engine_in->DrawLine (v2, v3, olc::BLACK, 0xFFFFFFFF);
      engine_in->DrawLine (v3, position_, olc::BLACK, 0xFFFFFFFF);
    }

    olc::vf2d start_;
    olc::vf2d position_;
    block*    block_;
    olc::vf2d target_;
    bool      carry_;
    float     speed_;
    float     a_;
    float     t_;
    bool      done_;
  };

  std::vector<block*>   blocks_;
  int                   countdown_;
  std::vector<cursor>   cursors_;
  bool                  drawCircle_;
  noise::module::Perlin noise_;

  void setup ();
  void circleSetup ();
  void rectSetup ();
  void lineSetup (olc::vf2d&, olc::vf2d&, int, int);
};

#endif // PGE_244_H
