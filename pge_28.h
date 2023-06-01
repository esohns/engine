#ifndef PGE_28_H
#define PGE_28_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines.h"

class PGE_28
 : public olc::PixelGameEngine
{
  class particle
  {
   public:
    particle (int x, int y)
     : position_ (x, y)
     , prev_ (x, y)
     , velocity_ ()
     , acceleration_ ()
    {}
    ~particle () {}

    void update ()
    {
      velocity_ += acceleration_;

      // limit
      if (velocity_.mag () > ENGINE_PGE_28_DEFAULT_VEL_LIMIT)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= ENGINE_PGE_28_DEFAULT_VEL_LIMIT;
      } // end IF

      position_ += velocity_;

      acceleration_ = {0.0f, 0.0f};
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (prev_, position_, olc::WHITE, 0xFFFFFFFF);
      prev_ = position_;
    }

    void attracted (const olc::vi2d& target_in)
    {
      // var dir = target - this.pos
      olc::vf2d force = target_in - position_;
      float d = force.mag ();
      d =
        (d < ENGINE_PGE_28_DEFAULT_DIST_LIMIT_MIN ? ENGINE_PGE_28_DEFAULT_DIST_LIMIT_MIN
                                                  : d > ENGINE_PGE_28_DEFAULT_DIST_LIMIT_MAX ? ENGINE_PGE_28_DEFAULT_DIST_LIMIT_MAX 
                                                                                             : d);
      float strength = ENGINE_PGE_28_DEFAULT_G / (d * d);
      force = force.norm ();
      force *= strength;
      if (d < ENGINE_PGE_28_DEFAULT_DIST_LIMIT) {
        force *= ENGINE_PGE_28_DEFAULT_REPEL_FORCE_MAG; // repel when particle gets too close
      }
      acceleration_ += force;
    }

   private:
    olc::vi2d position_;
    olc::vi2d prev_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
  };

 public:
  PGE_28 ();
  virtual ~PGE_28 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  typedef std::vector<particle*> particles_t;
  particles_t                    particles_;
  typedef std::vector<olc::vi2d> attractors_t;
  attractors_t                   attractors_;
};

#endif // PGE_28_H
