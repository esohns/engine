#ifndef PGE_97_H
#define PGE_97_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

class PGE_97
 : public olc::PixelGameEngine
{
 public:
  PGE_97 ();
  inline virtual ~PGE_97 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y, int32_t column, int32_t row, float baseR)
     : position_ (x, y)
     , index_ (column, row)
     , velocity_ (0.0f, 0.0f)
     , acceleration_ (0.0f, 0.0f)
     , mass_ (1.0f)
     , locked_ (false)
     , active_ (false)
     , radius_ (baseR)
     , targetR_ (baseR)
     , color_ (olc::BLACK)
    {}

    void applyForce (olc::vf2d& force)
    {
      olc::vf2d f = force;
      f /= mass_;
      acceleration_ += f;
    }

    void update (float baseR, float maxR)
    {
      velocity_ += acceleration_;
      position_ += velocity_;
      acceleration_ *= 0.0f;
      velocity_ *= 0.98f; // damping

      radius_ += 0.1f * (targetR_ - radius_);
      
      static Common_GL_Color_t color1 = {0, 0, 0}, color2 = {255, 30, 0};
      float amount = Common_GL_Tools::map ((radius_ - baseR) / (maxR - baseR), -1.0f, 1.0f, 0.0f, 1.0f);
      Common_GL_Color_t lerped_color =
        Common_GL_Tools::lerpRGB (color1, color2, amount);
      color_.r = lerped_color.r;
      color_.g = lerped_color.g;
      color_.b = lerped_color.b;
    }

    void applyAttRep (std::vector<olc::vi2d>& neighbours, std::vector<particle*>& particles, int32_t columns, float baseR, float maxR, float spacing)
    {
      for (std::vector<olc::vi2d>::iterator iterator = neighbours.begin ();
           iterator != neighbours.end ();
           ++iterator)
      {
        particle* target = particles[index_.x + (*iterator).x + (index_.y + (*iterator).y) * columns];
        if (target->active_)
        {
          olc::vf2d force = position_ - target->position_;
          float d = force.mag ();
          force = force.norm ();
          force *= (target->radius_ - baseR) / (d * 0.035f);
          acceleration_ += force;

          targetR_ = baseR * Common_GL_Tools::map (d, 0.0f, spacing, 0.0f, 0.5f);
        } // end IF
      } // end FOR
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (radius_), color_);
    }

    olc::vf2d  position_;
    olc::vi2d  index_;
    olc::vf2d  velocity_;
    olc::vf2d  acceleration_;
    float      mass_;
    bool       locked_;
    bool       active_;
    float      radius_;
    float      targetR_;
    olc::Pixel color_;
  };

  class spring
  {
   public:
    spring (float k, int32_t rL, particle& a, particle& b)
     : k_ (k)
     , restLength_ (rL)
     , a_ (&a)
     , b_ (&b)
    {}

    void update ()
    {
      olc::vf2d force = a_->position_ - b_->position_;
      float x = force.mag () - static_cast<float> (restLength_);
      force = force.norm ();
      force *= k_ * x;
      b_->applyForce (force);
      force *= -1.0f; //two opposite direction of force, a, b order is important
      a_->applyForce (force);
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (a_->position_, b_->position_, {200, 200, 200, 255}, 0xFFFFFFFF);
    }

    float     k_;
    int32_t   restLength_;
    particle* a_;
    particle* b_;
  };

  std::vector<olc::vi2d> neighbours_;
  std::vector<particle*> particles_;
  std::vector<spring*>   springs_;
  int32_t                columns_;
  int32_t                rows_;
  float                  spacing_;
  float                  baseR_;
  float                  maxR_;
};

#endif // PGE_97_H
