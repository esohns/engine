#ifndef PGE_17_H
#define PGE_17_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines.h"

class PGE_17
 : public olc::PixelGameEngine
{
 private:
  // *NOTE*: see also: https://www.red3d.com/cwr/boids/
  class boid
  {
   public:
    boid ()
    {
      position_ = olc::vd2d (static_cast<double> (Common_Tools::getRandomNumber (0, ENGINE_PGE_17_DEFAULT_WIDTH - 1)),
                             static_cast<double> (Common_Tools::getRandomNumber (0, ENGINE_PGE_17_DEFAULT_HEIGHT - 1)));
      velocity_ = olc::vd2d (Common_Tools::getRandomNumber (1.5, 3.0),
                             Common_Tools::getRandomNumber (1.5, 3.0));
      acceleration_ = olc::vd2d (0.0, 0.0);
      maxForce_ = 0.2;
      maxSpeed_ = 5;
    }

    olc::vd2d align (const std::vector<boid*>& boids_in)
    {
      ACE_UINT32 perception_i = 25;
      olc::vd2d steering_velocity = { 0.0, 0.0 };
      ACE_UINT32 total_i = 0;
      for (std::vector<boid*>::const_iterator iterator = boids_in.begin ();
           iterator != boids_in.end ();
           ++iterator)
      {
        if (*iterator == this)
          continue;
        double distance_d = position_.dist ((*iterator)->position_);
        if (distance_d > static_cast<double> (perception_i))
          continue;
        steering_velocity += (*iterator)->velocity_;
        total_i++;
      } // end FOR
      if (total_i)
      {
        steering_velocity /= static_cast<double> (total_i);
        steering_velocity = steering_velocity.norm ();
        steering_velocity *= maxSpeed_;
        steering_velocity -= velocity_;
        if (steering_velocity.mag () > maxForce_)
        {
          olc::vd2d upper_bound = { maxForce_, maxForce_ };
          olc::vd2d lower_bound = { -1.0 * maxForce_, -1.0 * maxForce_ };
          steering_velocity = steering_velocity.clamp (lower_bound, upper_bound);
        } // end IF
      } // end IF

      return steering_velocity;
    }

    olc::vd2d cohesion (const std::vector<boid*>& boids_in)
    {
      ACE_UINT32 perception_i = 50;
      olc::vd2d steering_velocity = { 0.0, 0.0 };
      ACE_UINT32 total_i = 0;
      for (std::vector<boid*>::const_iterator iterator = boids_in.begin ();
           iterator != boids_in.end ();
           ++iterator)
      {
        if (*iterator == this)
          continue;
        double distance_d = position_.dist ((*iterator)->position_);
        if (distance_d > static_cast<double> (perception_i))
          continue;
        steering_velocity += (*iterator)->position_;
        total_i++;
      } // end FOR
      if (total_i)
      {
        steering_velocity /= static_cast<double> (total_i);
        steering_velocity -= position_;
        steering_velocity = steering_velocity.norm ();
        steering_velocity *= maxSpeed_;
        steering_velocity -= velocity_;
        if (steering_velocity.mag () > maxForce_)
        {
          olc::vd2d upper_bound = { maxForce_, maxForce_ };
          olc::vd2d lower_bound = { -1.0 * maxForce_, -1.0 * maxForce_ };
          steering_velocity = steering_velocity.clamp (lower_bound, upper_bound);
        } // end IF
      } // end IF

      return steering_velocity;
    }

    olc::vd2d separation (const std::vector<boid*>& boids_in)
    {
      ACE_UINT32 perception_i = 24;
      olc::vd2d steering_velocity = { 0.0, 0.0 };
      ACE_UINT32 total_i = 0;
      for (std::vector<boid*>::const_iterator iterator = boids_in.begin ();
           iterator != boids_in.end ();
           ++iterator)
      {
        if (*iterator == this)
          continue;
        double distance_d = position_.dist ((*iterator)->position_);
        if (distance_d > static_cast<double> (perception_i))
          continue;
        olc::vd2d diff = position_ - (*iterator)->position_;
        diff /= distance_d * distance_d;
        steering_velocity += diff;
        total_i++;
      } // end FOR
      if (total_i)
      {
        steering_velocity /= static_cast<double> (total_i);
        steering_velocity = steering_velocity.norm ();
        steering_velocity *= maxSpeed_;
        steering_velocity -= velocity_;
        if (steering_velocity.mag () > maxForce_)
        {
          olc::vd2d upper_bound = { maxForce_, maxForce_ };
          olc::vd2d lower_bound = { -1.0 * maxForce_, -1.0 * maxForce_ };
          steering_velocity = steering_velocity.clamp (lower_bound, upper_bound);
        } // end IF
      } // end IF

      return steering_velocity;
    }

    void flock (const std::vector<boid*>& boids_in)
    {
      olc::vd2d alignment_force = align (boids_in);
      olc::vd2d alignment_force_2 = cohesion (boids_in);
      olc::vd2d alignment_force_3 = separation (boids_in);

      applyForce (alignment_force);
      applyForce (alignment_force_2);
      applyForce (alignment_force_3);
    }

    void update ()
    {
      position_ += velocity_;
      velocity_ += acceleration_;
      if (velocity_.mag () > static_cast<double> (maxSpeed_))
      {
        olc::vd2d upper_bound = { static_cast<double> (maxSpeed_), static_cast<double> (maxSpeed_) };
        olc::vd2d lower_bound = { -1.0 * static_cast<double> (maxSpeed_), -1.0 * static_cast<double> (maxSpeed_) };
        velocity_ = velocity_.clamp (lower_bound, upper_bound);
      } // end IF
      acceleration_ = {0.0, 0.0};
    }

    void applyForce (const olc::vd2d& force_in)
    {
      acceleration_ += force_in;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->Draw (position_, olc::WHITE);
    }

    void edges ()
    {
      if (position_.x > ENGINE_PGE_17_DEFAULT_WIDTH - 1)
      {
        position_.x = 0;
      }
      if (position_.x < 0)
      {
        position_.x = ENGINE_PGE_17_DEFAULT_WIDTH - 1;
      }

      if (position_.y > ENGINE_PGE_17_DEFAULT_HEIGHT - 1)
      {
        position_.y = 0;
      }
      if (position_.y < 0)
      {
        position_.y = ENGINE_PGE_17_DEFAULT_HEIGHT - 1;
      }
    }

   public:
    olc::vd2d  position_;
    olc::vd2d  velocity_;
    olc::vd2d  acceleration_;
    double     maxForce_;
    ACE_UINT32 maxSpeed_;
  };

 public:
  PGE_17 ();
  ~PGE_17 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<boid*> flock_;
};

#endif // PGE_14_H
