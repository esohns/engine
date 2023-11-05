#ifndef PGE_224_H
#define PGE_224_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_10.h"

class PGE_224
 : public olc::PixelGameEngine
{
 public:
  PGE_224 ();
  virtual ~PGE_224 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle;
  class force
  {
   public:
    force (float x, float y, float k)
     : position_ (x, y)
     , k_ (k)
     , type_ (INV_SQ)
     , minDist_ (ENGINE_PGE_224_DEFAULT_MIN_DISTANCE)
     , b_ (true)
    {}

    enum Type
    {
      INV_SQ = 0,
      LINEAR
    };

    void forceParticle (particle& particle)
    {
      float dx = particle.position_.x - position_.x;
      float dy = particle.position_.y - position_.y;
      float d = std::sqrt (dx * dx + dy * dy);
      if (d < 0.0001f)
        return;

      float force = forceAtDist (d);
      if (force > ENGINE_PGE_224_DEFAULT_MAX_FORCE)
        force = ENGINE_PGE_224_DEFAULT_MAX_FORCE;
      if (force < -ENGINE_PGE_224_DEFAULT_MAX_FORCE)
        force = -ENGINE_PGE_224_DEFAULT_MAX_FORCE;

      b_ = false;
      particle.acceleration_.x += force * dx / d;
      particle.acceleration_.y += force * dy / d;
    }

    float forceAtDist (float d)
    {
      switch (type_)
      {
        case INV_SQ:
          if (d < minDist_)
            return (k_ / (minDist_ * minDist_)) * (d / minDist_);
          else
            return k_ / (d * d);
          break;
        case LINEAR:
          return k_ * d;
      } // end SWITCH

      return 0.0f;
    }

    olc::vf2d position_;
    float     k_;
    enum Type type_;
    float     minDist_;
    bool      b_;
  };

  class particle
  {
   public:
    particle (float x, float y)
     : position_ (x, y)
     , velocity_ ()
     , acceleration_ ()
     , mass_ (1.0f)
     , forces_ ()
    {}
    ~particle ()
    {
      for (std::vector<std::shared_ptr<force> >::iterator iterator = forces_.begin ();
           iterator != forces_.end ();
           ++iterator)
        (*iterator).reset ();
    }

    void draw (olc::PixelGameEngine* engine)
    {
      float speed = velocity_.mag ();
      if (speed > ENGINE_PGE_224_DEFAULT_MAX_SPEED)
        speed = ENGINE_PGE_224_DEFAULT_MAX_SPEED;
      speed /= ENGINE_PGE_224_DEFAULT_MAX_SPEED;

      olc::Pixel color (static_cast<uint8_t> (255 * speed),
                        static_cast<uint8_t> (155 + 100 * (1.0f - speed)),
                        static_cast<uint8_t> (255 * (1.0f - speed)),
                        255);
      engine->FillCircle (position_, 5, color);

      static olc::Pixel color_2 (255, 255, 0, 255);
      engine->FillCircle (position_, 3, color_2);
    }

    void update ()
    {
      acceleration_ *= 0.0f;

      // outside force
      for (std::vector<std::shared_ptr<force> >::iterator iterator = forces_.begin ();
           iterator != forces_.end ();
           ++iterator)
        (*iterator)->forceParticle (*this);

      // friction
      float speed = velocity_.mag ();
      acceleration_.x -= ENGINE_PGE_224_DEFAULT_FRICTION * speed * velocity_.x;
      acceleration_.y -= ENGINE_PGE_224_DEFAULT_FRICTION * speed * velocity_.y;

      velocity_.x += ENGINE_PGE_224_DEFAULT_DT * acceleration_.x / mass_;
      velocity_.y += ENGINE_PGE_224_DEFAULT_DT * acceleration_.y / mass_;
      //velocity_ *= ENGINE_PGE_224_DEFAULT_DT / 2.0f;
      position_ += velocity_;
    }

    void addForce (std::shared_ptr<force> force)
    {
      forces_.push_back (force);
    }

    //olc::vf2d dirTo (particle* particle,
    //                 olc::PixelGameEngine* engine)
    //{
    //  float dx = particle->position_.x - position_.x;
    //  float dy = particle->position_.y - position_.y;

    //  static float half_width_f = engine->ScreenWidth () / 2.0f;
    //  static float half_height_f = engine->ScreenHeight () / 2.0f;

    //  if (dx > half_width_f)
    //    dx -= static_cast<float> (engine->ScreenWidth ());
    //  if (dx < -half_width_f)
    //    dx += static_cast<float> (engine->ScreenWidth ());
    //  if (dy > half_height_f)
    //    dy -= static_cast<float> (engine->ScreenHeight ());
    //  if (dy < -half_height_f)
    //    dy += static_cast<float> (engine->ScreenHeight ());

    //  return olc::vf2d (dx, dy);
    //}

    olc::vf2d                            position_;
    olc::vf2d                            velocity_;
    olc::vf2d                            acceleration_;
    float                                mass_;
    std::vector<std::shared_ptr<force> > forces_;
  };

  int                    columns_;
  int                    rows_;
  std::vector<particle*> particles_;
  std::shared_ptr<force> mouseForce_;
};

#endif // PGE_224_H
