#ifndef PGE_20_H
#define PGE_20_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines.h"

class PGE_20
 : public olc::PixelGameEngine
{
 private:
  class mover
  {
   public:
    mover (olc::vd2d position_in)
     : position_ (position_in)
     , velocity_ ()
     , acceleration_ ()
     , mass_ (24.0f)
     , damping_ (0.98f)
     , dragOffset_ ()
     , dragging_ (false)
    {
    }

    void update ()
    {
      velocity_ += acceleration_;
      velocity_ *= damping_;
      position_ += velocity_;
      acceleration_ = {0.0f, 0.0f};
    }

    void applyForce (olc::vd2d force_in)
    {
      olc::vd2d force = force_in;
      force /= mass_;
      acceleration_ += force;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, static_cast<int32_t> (mass_ * 2.0f), olc::WHITE, 0xFF);
    }

    void clicked (int mx, int my)
    {
      float d =
        static_cast<float> (position_.dist ({ static_cast<double> (mx), static_cast<double> (my) }));
      if (d < mass_)
      {
        dragging_ = true;
        dragOffset_.x = position_.x - mx;
        dragOffset_.y = position_.y - my;
      } // end IF
    }

    void stopDragging ()
    {
      dragging_ = false;
    }

    void drag (int mx, int my)
    {
      if (dragging_)
      {
        position_.x = mx + dragOffset_.x;
        position_.y = my + dragOffset_.y;
      } // end IF
    }

   public:
    olc::vd2d position_;
    olc::vd2d velocity_;
    olc::vd2d acceleration_;
    float     mass_;
    float     damping_;
    olc::vd2d dragOffset_;
    bool      dragging_;
  };

  class spring
  {
   public:
    spring (olc::vd2d base_in, int length_in)
     : base_ (base_in)
     , length_ (length_in)
     , k_ (0.2f)
    {
    }

    void connect (mover& mover_in)
    {
      olc::vd2d force = mover_in.position_ - base_;
      float d = static_cast<float> (force.mag ());
      // Stretch is difference between current distance and rest length
      float stretch = d - length_;

      // Calculate force according to Hooke's Law
      // F = k * stretch
      force = force.norm ();
      force *= (-1.0f * k_ * stretch);
      mover_in.applyForce (force);
    }

    void constrainLength (mover& mover_in, float minlen, float maxlen)
    {
      olc::vd2d direction = mover_in.position_ - base_;
      float d = static_cast<float> (direction.mag ());
      // Is it too short?
      if (d < minlen)
      {
        direction = direction.norm ();
        direction *= minlen;
        // Reset position and stop from moving (not realistic physics)
        mover_in.position_ = base_ + direction;
        mover_in.velocity_ = { 0.0, 0.0 };
      } // end IF
      // Is it too long?
      else if (d > maxlen)
      {
        direction = direction.norm ();
        direction *= maxlen;
        // Reset position and stop from moving (not realistic physics)
        mover_in.position_ = base_ + direction;
        mover_in.velocity_ = { 0.0, 0.0 };
      } // end ELSE IF
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawRect (base_, {10, 10}, olc::WHITE);
    }
    void showLine (olc::PixelGameEngine* engine_in, mover& mover_in)
    {
      engine_in->DrawLine (base_, mover_in.position_, olc::WHITE, 0xFFFFFFFF);
    }

   public:
    olc::vd2d base_;
    int       length_;
    float     k_;
  };

 public:
  PGE_20 ();
  ~PGE_20 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  mover*  mover_;
  spring* spring_;
};

#endif // PGE_20_H
