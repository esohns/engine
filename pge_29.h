#ifndef PGE_29_H
#define PGE_29_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines.h"

class PGE_29
 : public olc::PixelGameEngine
{
  class wall
  {
   public:
    wall (const olc::vf2d& position_1, const olc::vf2d& position_2)
     : a_ (position_1)
     , b_ (position_2)
    {}
    ~wall () {}

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (a_, b_, olc::WHITE, 0xFFFFFFFF);
    }

   public:
    olc::vf2d a_;
    olc::vf2d b_;
  };

  class ray
  {
   public:
    ray (const olc::vf2d& position, int angle) // degrees
     : position_ (&position)
     , direction_ (cosf (static_cast<float> (static_cast<double> (angle) * M_PI / 180.0)),
                   sinf (static_cast<float> (static_cast<double> (angle) * M_PI / 180.0)))
    {}
    ~ray () {}

    //void lookAt (int x, int y)
    //{
    //  direction_.x = x - position_->x;
    //  direction_.y = y - position_->y;
    //  direction_ = direction_.norm ();
    //}

    //void show (olc::PixelGameEngine* engine_in)
    //{
    //  olc::vf2d end;
    //  end = *position_ + direction_;
    //  end = end.norm ();
    //  end *= 10;

    //  engine_in->DrawLine (*position_, end, olc::WHITE, 0xFFFFFFFF);
    //}

    olc::vf2d cast (const wall& wall_in)
    {
      float x1 = wall_in.a_.x;
      float y1 = wall_in.a_.y;
      float x2 = wall_in.b_.x;
      float y2 = wall_in.b_.y;

      float x3 = position_->x;
      float y3 = position_->y;
      float x4 = position_->x + direction_.x;
      float y4 = position_->y + direction_.y;

      float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
      if (den == 0.0f)
        return olc::vf2d (std::numeric_limits<float>::min (), std::numeric_limits<float>::min ());

      float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
      float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
      if (t > 0.0f && t < 1.0f && u > 0.0f)
      {
        olc::vf2d pt;
        pt.x = x1 + t * (x2 - x1);
        pt.y = y1 + t * (y2 - y1);
        return pt;
      } // end IF

      return olc::vf2d (std::numeric_limits<float>::min (), std::numeric_limits<float>::min ());
    }

   private:
    const olc::vf2d* position_;
    olc::vf2d        direction_;
  };

  class particle
  {
   public:
    particle (int x, int y)
     : position_ (static_cast<float> (x), static_cast<float> (y))
     , rays_ ()
    {
      for (int i = 0; i < 360; ++i)
        rays_.push_back (new ray (position_, i));
    }
    ~particle ()
    {
      for (int i = 0; i < rays_.size (); i++)
        delete rays_[i];
    }

    void update (int x, int y)
    {
      position_ = {static_cast<float> (x), static_cast<float> (y)};
    }

    void look (const std::vector<wall*>& walls_in,
               olc::PixelGameEngine* engine_in)
    {
      for (int i = 0; i < rays_.size (); i++)
      {
        olc::vf2d closest_pt (std::numeric_limits<float>::min (), std::numeric_limits<float>::min ());
        float record = std::numeric_limits<float>::max ();
        for (std::vector<wall*>::const_iterator iterator = walls_in.begin ();
             iterator != walls_in.end ();
             ++iterator)
        {
          olc::vf2d pt = rays_[i]->cast (**iterator);
          if ((pt.x == std::numeric_limits<float>::min ()) && (pt.y == std::numeric_limits<float>::min ()))
            continue;
          float d = position_.dist (pt);
          if (d < record)
          {
            record = d;
            closest_pt = pt;
          } // end IF
        } // end FOR
        engine_in->DrawLine (position_, closest_pt, olc::WHITE, 0xFFFFFFFF);
      } // end FOR
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, 4, olc::WHITE);
      //for (int i = 0; i < rays_.size (); i++)
      //  rays_[i]->show (engine_in);
    }

   private:
    olc::vf2d         position_;
    std::vector<ray*> rays_;
  };

 public:
  PGE_29 ();
  virtual ~PGE_29 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  particle*          particle_;
  std::vector<wall*> walls_;
};

#endif // PGE_29_H
