#ifndef PGE_381_H
#define PGE_381_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_17.h"

class PGE_381
 : public olc::PixelGameEngine
{
 public:
  PGE_381 ();
  inline virtual ~PGE_381 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
     particle (float x, float y, float r, olc::vf2d* dir = NULL)
     : position_ (x, y)
     , direction_ ()
     , radius_ (r)
    {
      if (dir)
        direction_ = *dir;
      else
      {
        direction_ = olc::vf2d (ENGINE_PGE_381_DEFAULT_SPEED, 0.0f);
        float angle_f = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
        float x_rotated = std::cos (angle_f) * direction_.x - std::sin (angle_f) * direction_.y;
        float y_rotated = std::sin (angle_f) * direction_.x + std::cos (angle_f) * direction_.y;
        direction_.x = x_rotated;
        direction_.y = y_rotated;
      } // end ELSE
    }

    void move ()
    {
      position_ += direction_;

      static float angles_a[2] = { 0.0f, 0.05f };
      int index_i = Common_Tools::getRandomNumber (0, 1);
      float angle_f = angles_a[index_i];
      float x_rotated = std::cos (angle_f) * direction_.x - std::sin (angle_f) * direction_.y;
      float y_rotated = std::sin (angle_f) * direction_.x + std::cos (angle_f) * direction_.y;
      direction_.x = x_rotated;
      direction_.y = y_rotated;
    }

    void constrain (olc::PixelGameEngine* engine_in)
    {
      static float screen_width_f = static_cast<float> (engine_in->ScreenWidth ());
      static float screen_height_f = static_cast<float> (engine_in->ScreenHeight ());

      float x = std::min (std::max (position_.x, radius_), screen_width_f - radius_);
      float y = std::min (std::max (position_.y, radius_), screen_height_f - radius_);
      if (x != position_.x || y != position_.y)
      {
        position_.x = x;
        position_.y = y;

        float angle_f = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
        float x_rotated = std::cos (angle_f) * direction_.x - std::sin (angle_f) * direction_.y;
        float y_rotated = std::sin (angle_f) * direction_.x + std::cos (angle_f) * direction_.y;
        direction_.x = x_rotated;
        direction_.y = y_rotated;
      } // end IF
    }
    
    bool inCircle (float cx, float cy, float r)
    {
      olc::vf2d center_s (cx, cy);
      return position_.dist (center_s) <= r + radius_;
    }

    float penetration (const particle& other)
    {
      float d = position_.dist (other.position_);
      float td = radius_ + other.radius_;
      return td - d;
    }
    void interact (const particle& other,
                   float minR)
    {
      float pen = penetration (other);
      if (std::abs (pen) < minR * ENGINE_PGE_381_DEFAULT_TOUCH_RATIO)
      {
        float angle_f = 0.1f;
        float x_rotated = std::cos (angle_f) * direction_.x - std::sin (angle_f) * direction_.y;
        float y_rotated = std::sin (angle_f) * direction_.x + std::cos (angle_f) * direction_.y;
        direction_.x = x_rotated;
        direction_.y = y_rotated;
      }
      else if (pen > 0.0f)
      {
        // Move away from overlapping elem
        olc::vf2d sub = position_ - other.position_;
        sub = sub.norm ();
        sub *= ENGINE_PGE_381_DEFAULT_SPEED;
        direction_ = sub;
        // olv::vf2d n = (other.position_ - position_).norm ();
        // direction_.reflect (n);
      } // end ELSE IF
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (radius_), olc::WHITE);

      olc::vf2d v = direction_;
      v = v.norm ();
      v *= radius_;
      olc::vf2d q = position_ + v;
      engine_in->DrawLine (position_, q, olc::BLACK, 0xFFFFFFFF);
    }

    olc::vf2d position_;
    olc::vf2d direction_;
    float radius_;
  };

  enum mode
  {
    MODE_ELEMENTS = 0,
    MODE_PICTURE
  };

  particle randomElement ();
  void generate ();
  void simulate ();

  float                 R_;
  float                 minR_;
  float                 maxR_;
  enum mode             mode_;
  std::vector<particle> particles_;
};

#endif // PGE_381_H
