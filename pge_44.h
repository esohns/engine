#ifndef PGE_44_H
#define PGE_44_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_2.h"

class PGE_44
 : public olc::PixelGameEngine
{
 public:
  PGE_44 ();
  virtual ~PGE_44 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class vehicle
  {
   public:
    vehicle (int x_in, int y_in)
     : position_ (static_cast<float> (x_in), static_cast<float> (y_in))
     , velocity_ ({1.0f, 0.0f})
     , acceleration_ ({0.0f, 0.0f})
     , radius_(ENGINE_PGE_44_DEFAULT_RADIUS)
     , maxSpeed_ (ENGINE_PGE_44_DEFAULT_MAXSPEED)
     , maxForce_ (ENGINE_PGE_44_DEFAULT_MAXFORCE)
     //, wanderTheta_ (static_cast<float> (M_PI) / 2.0f)
     , xOffset_ (Common_Tools::getRandomNumber (0.0, 1.0))
     , yOffset_ (Common_Tools::getRandomNumber (0.0, 1.0))
     , path_ ()
     //, paths_ ()
    {}

    void wander (noise::module::Perlin* noise_in)
    {
      float angle =
        static_cast<float> (noise_in->GetValue (xOffset_, yOffset_, ENGINE_PGE_31_DEFAULT_NOISE_Z) * M_PI);
      olc::vf2d steer = {std::cos (angle), std::sin (angle)};
      steer = steer.norm ();
      steer *= maxForce_;
      applyForce (steer);
      xOffset_ += 0.01;
    }

    olc::vf2d seek (olc::vf2d& target_in,
                    bool arrival_in = false)
    {
      olc::vf2d force = target_in - position_;
      float desiredSpeed = maxSpeed_;
      if (arrival_in)
      {
        float slowRadius = 100.0f;
        float distance = force.mag ();
        if (distance < slowRadius)
          desiredSpeed = Common_GL_Tools::map (distance, 0.0f, slowRadius, 0.0f, maxSpeed_);
      } // end IF
      force = force.norm ();
      force *= desiredSpeed;
      force -= velocity_;
      if (force.mag () > maxForce_)
      {
        force = force.norm ();
        force *= maxForce_;
      } // end IF
      return force;
    }

    olc::vf2d pursue (vehicle& vehicle_in)
    {
      olc::vf2d target = vehicle_in.position_;
      olc::vf2d prediction = vehicle_in.velocity_;
      prediction *= 10.0;
      target += prediction;
      return seek (target);
    }

    olc::vf2d evade (vehicle& vehicle_in)
    {
      olc::vf2d pursuit = pursue (vehicle_in);
      pursuit *= -1.0f;
      return pursuit;
    }

    olc::vf2d arrive (olc::vf2d& target_in)
    {
      // 2nd argument true enables the arrival behavior
      return seek (target_in, true);
    }

    olc::vf2d flee (olc::vf2d& target_in)
    {
      olc::vf2d force = seek (target_in);
      force *= -1.0;
      return force;
    }

    void applyForce (olc::vf2d& force_in)
    {
      acceleration_ += force_in;
    }

    void update (float fElapsedTime)
    {
      velocity_ += acceleration_;
      if (velocity_.mag () > maxSpeed_)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= maxSpeed_;
      } // end IF
      position_ += velocity_;
      acceleration_ = {0.0f, 0.0f};

      path_.push_back (position_);

      size_t total = path_.size ();
      if (total > 200 || (total > 10 && fElapsedTime > 0.3f))
        path_.erase (path_.begin ());
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d direction = velocity_;
      direction = direction.norm ();
      direction *= static_cast<float> (radius_);
      olc::vf2d p1 = position_ + direction;
      olc::vf2d x = position_ + -direction;
      olc::vf2d dot_direction = direction.perp ();
      dot_direction = dot_direction.norm ();
      dot_direction *= static_cast<float> (radius_);
      olc::vf2d p2 = x + dot_direction;
      olc::vf2d p3 = x + -dot_direction;
      //engine_in->DrawLine (p2, p1, olc::WHITE, 0xFFFFFFFF);
      //engine_in->DrawLine (p3, p1, olc::WHITE, 0xFFFFFFFF);
      //engine_in->DrawLine (p2, p3, olc::WHITE, 0xFFFFFFFF);
      engine_in->FillTriangle (p1, p2, p3, olc::WHITE);
      //engine_in->FillCircle(position_, radius_, olc::WHITE);

      std::vector<olc::vf2d>::iterator iterator_2 = path_.begin ();
      if (path_.size () > 1)
        std::advance (iterator_2, 1);
      for (std::vector<olc::vf2d>::iterator iterator = path_.begin ();
           iterator != path_.end ();
           ++iterator)
      { 
        if (iterator_2 == path_.end ())
          engine_in->Draw (*iterator, olc::WHITE);
        else
        {
          engine_in->DrawLine (*iterator, *iterator_2, olc::WHITE, 0xFFFFFFFF);
          std::advance (iterator_2, 1);
      } // end ELSE
      } // end FOR
    }

    void edges (olc::PixelGameEngine* engine_in)
    {
      bool hitEdge = false;
      if (position_.x > engine_in->ScreenWidth () + radius_)
      {
        position_.x = static_cast<float> (-radius_);
        hitEdge = true;
      } // end IF
      else if (position_.x < -radius_)
      {
        position_.x = static_cast<float> (engine_in->ScreenWidth () + radius_);
        hitEdge = true;
      } // end ELSE IF

      if (position_.y > engine_in->ScreenHeight () + radius_)
      {
        position_.y = static_cast<float> (-radius_);
        hitEdge = true;
      } // end IF
      else if (position_.y < -radius_)
      {
        position_.y = static_cast<float> (engine_in->ScreenHeight () + radius_);
        hitEdge = true;
      } // end ELSE IF

      if (hitEdge)
      {
        path_.clear ();
      } // end IF
    }

    olc::vf2d              position_;
    olc::vf2d              velocity_;
    int32_t                radius_;

   private:
    olc::vf2d              acceleration_;
    float                  maxSpeed_;
    float                  maxForce_;
    //float                  wanderTheta_;
    double                 xOffset_;
    double                 yOffset_;
    std::vector<olc::vf2d> path_;
    //, paths_ ()
  };

  //class target
  // : public vehicle
  //{
  //  target (int x_in, int y_in)
  //   : vehicle (x_in, y_in)
  //  {
  //    velocity_ = { Common_Tools::getRandomNumber (0.0f, 1.0f),
  //                  Common_Tools::getRandomNumber (0.0f, 1.0f) };
  //    velocity_ = velocity_.norm ();
  //    velocity_ *= 5.0f;
  //  }

  //  void show (olc::PixelGameEngine* engine_in)
  //  {
  //    engine_in->DrawCircle (position_, radius_, olc::WHITE, 0xFF);
  //  }
  //};

  noise::module::Perlin noise_;
  std::vector<vehicle*> vehicles_;
};

#endif // PGE_44_H
