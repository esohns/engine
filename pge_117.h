#ifndef PGE_117_H
#define PGE_117_H

#include "olcPixelGameEngine.h"

class PGE_117
 : public olc::PixelGameEngine
{
 public:
  PGE_117 ();
  virtual ~PGE_117 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class point
  {
   public:
    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
    olc::vf2d force_;
    float     m_;
    bool      fixed_;
    float     drag_;
    float     collisionDist_;

    point (float x, float y, bool fixed)
     : position_ (x, y)
     , velocity_ ()
     , acceleration_ ()
     , force_ ()
     , m_ (1.0f)
     , fixed_ (fixed)
     , drag_ (0.8f)
     , collisionDist_ (15.0f)
    {}

    void resetForce ()
    {
      force_ *= (0.0f);
    }

    void addForce (olc::vf2d& force_in)
    {
      force_ += force_in;
    }

    void update (float dt)
    {
      if (fixed_) return;

      acceleration_ = force_ / m_;
      olc::vf2d f = acceleration_;
      f *= dt;
      velocity_ += f;

      f = velocity_;
      f *= dt;
      position_ += f;

      velocity_ *= drag_;
    }

    void fix ()
    {
      fixed_ = true;
    }

    void checkCollisions (int32_t i, std::vector<point*>& points)
    {
      for (int32_t p = 0; p < points.size (); p++)
      {
        if (i == p) continue;

        olc::vf2d rel = position_ - points[p]->position_;
        float d = rel.mag ();
        if (d < collisionDist_)
        {
          rel *= 1.0f / (d + 0.001f);
          force_ += rel;
        } // end IF
      } // end FOR
    }
  };

  class spring
  {
   public:
    spring (point& a, point& b, float l, float k = 0.9f)
     : a_ (&a)
     , b_ (&b)
     , l_ (l)
     , k_ (k)
    {}

    void applyForces ()
    {
      olc::vf2d vecAB = b_->position_ - a_->position_;
      float forceMag = k_ * (l_ - vecAB.mag ());
      olc::vf2d forceAB = vecAB;
      forceAB = forceAB.norm ();
      forceAB *= forceMag;
      olc::vf2d forceBA = forceAB * -1.0f;
      a_->addForce (forceBA);
      b_->addForce (forceAB);
    }

    point* a_;
    point* b_;
    float  l_;
    float  k_;
  };

  class line
  {
   public:
    line (bool closed = false)
     : points_ ()
     , springs_ ()
     , closed_ (closed)
    {}

    ~line ()
    {
      for (std::vector<point*>::iterator iterator = points_.begin ();
           iterator != points_.end ();
           ++iterator)
        delete *iterator;
      for (std::vector<spring*>::iterator iterator = springs_.begin ();
           iterator != springs_.end ();
           ++iterator)
        delete *iterator;
    }

    void addPoint (float x, float y, bool fixed, float len)
    {
      points_.push_back (new point (x, y, fixed));
      if (points_.size () == 1) return;

      if (closed_ && points_.size () > 2)
      {
        delete springs_.back ();
        springs_.pop_back ();
      } // end IF
      springs_.push_back (new spring (*points_[points_.size () - 2], *points_[points_.size () - 1], len));
      if (closed_)
        springs_.push_back (new spring (*points_[points_.size () - 1], *points_[0], len));
    }

    void insertPoint (bool fixed, float len0, float len1, int32_t firstIndex)
    {
      if (points_.size () == 1) return;

      olc::vf2d midPos = points_[firstIndex]->position_ + points_[firstIndex + 1]->position_;
      midPos *= 0.5f;
      std::vector<point*>::iterator iterator = points_.begin (); iterator += firstIndex + 1;
      points_.insert (iterator, new point (midPos.x, midPos.y, fixed));
      std::vector<spring*>::iterator iterator_2 = springs_.begin (); iterator_2 += firstIndex;
      delete* iterator_2; iterator_2 = springs_.erase (iterator_2);
      iterator_2 = springs_.insert (iterator_2, new spring (*points_[firstIndex], *points_[firstIndex + 1], len0));
      springs_.insert (iterator_2 + 1, new spring(*points_[firstIndex + 1], *points_[firstIndex + 2], len1));
    }

    void update (float dt)
    {
      for (int s = 0; s < springs_.size (); s++)
        springs_[s]->applyForces ();
      for (int p = 0; p < points_.size (); p++)
      {
        points_[p]->checkCollisions (p, points_);
        points_[p]->update (dt);
        points_[p]->resetForce ();
      } // end FOR
    }

    void show (bool displayPoints,
               olc::PixelGameEngine* engine_in)
    {
      for (int p = 1; p < points_.size (); p++)
      {
        engine_in->DrawLine (points_[p - 1]->position_, points_[p]->position_, olc::WHITE, 0xFFFFFFFF);
        if (displayPoints)
        {
          engine_in->DrawCircle (points_[p - 1]->position_, 3, olc::WHITE, 0xFF);
          engine_in->DrawCircle (points_[p]->position_, 3, olc::WHITE, 0xFF);
        } // end IF
      } // end FOR
      if (closed_)
        engine_in->DrawLine (points_[0]->position_, points_[points_.size () - 1]->position_, olc::WHITE, 0xFFFFFFFF);
    }

    std::vector<point*>  points_;
    std::vector<spring*> springs_;
    bool                 closed_;
  };

  line* line_;
};

#endif // PGE_117_H
