#ifndef PGE_36_H
#define PGE_36_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_2.h"

class PGE_36
 : public olc::PixelGameEngine
{
 public:
  PGE_36 ();
  virtual ~PGE_36 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct dna
  {
    float   foodWeight;
    float   poisonWeight;
    int32_t foodPerception;
    int32_t poisonPerception;
  };

  class vehicle
  {
   public:
    vehicle (int x, int y, struct dna* dna_in)
     : position_ ({x, y})
     , acceleration_ ({0.0f, 0.0f})
     , velocity_ ({0.0f, -2.0f})
     , radius_ (ENGINE_PGE_36_DEFAULT_RADIUS)
     , maxSpeed_ (ENGINE_PGE_36_DEFAULT_MAXSPEED)
     , maxForce_ (ENGINE_PGE_36_DEFAULT_MAXFORCE)
     , health_ (ENGINE_PGE_36_DEFAULT_HEALTH)
     , dna_ ()
    {
      if (!dna_in)
      {
        dna_.foodWeight = Common_Tools::getRandomNumber (-ENGINE_PGE_36_DEFAULT_FOOD_WEIGHT, ENGINE_PGE_36_DEFAULT_FOOD_WEIGHT);
        dna_.poisonWeight = Common_Tools::getRandomNumber (-ENGINE_PGE_36_DEFAULT_FOOD_WEIGHT, ENGINE_PGE_36_DEFAULT_FOOD_WEIGHT);
        dna_.foodPerception = Common_Tools::getRandomNumber (0, ENGINE_PGE_36_DEFAULT_FOOD_PERCEPTION);
        dna_.poisonPerception = Common_Tools::getRandomNumber (0, ENGINE_PGE_36_DEFAULT_FOOD_PERCEPTION);
      } // end IF
      else
      {
        dna_.foodWeight = dna_in->foodWeight;
        if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_MUTATION_RATE))
          dna_.foodWeight += Common_Tools::getRandomNumber (-ENGINE_PGE_36_DEFAULT_WEIGHT_MUTATION, ENGINE_PGE_36_DEFAULT_WEIGHT_MUTATION);
        dna_.poisonWeight = dna_in->poisonWeight;
        if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_MUTATION_RATE))
          dna_.poisonWeight += Common_Tools::getRandomNumber (-ENGINE_PGE_36_DEFAULT_WEIGHT_MUTATION, ENGINE_PGE_36_DEFAULT_WEIGHT_MUTATION);
        dna_.foodPerception = dna_in->foodPerception;
        if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_MUTATION_RATE))
          dna_.foodPerception += Common_Tools::getRandomNumber (-ENGINE_PGE_36_DEFAULT_PERCEPTION_MUTATION, ENGINE_PGE_36_DEFAULT_PERCEPTION_MUTATION);
        dna_.poisonPerception = dna_in->poisonPerception;
        if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_MUTATION_RATE))
          dna_.poisonPerception += Common_Tools::getRandomNumber (-ENGINE_PGE_36_DEFAULT_PERCEPTION_MUTATION, ENGINE_PGE_36_DEFAULT_PERCEPTION_MUTATION);
      } // end ELSE
    }
    inline ~vehicle () {}

    void update ()
    {
      health_ -= ENGINE_PGE_36_DEFAULT_DIGESTION_PER_TURN;

      velocity_ += acceleration_;
      if (velocity_.mag () > maxSpeed_)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= static_cast<float> (maxSpeed_);
      } // end IF
      
      position_ += velocity_;

      acceleration_ *= 0.0f;
    }

    void applyForce (const olc::vf2d& force_in)
    {
      acceleration_ += force_in;
    }

    vehicle* clone ()
    {
      if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_CLONE_PROBABILITY))
        return new vehicle (position_.x, position_.y, &dna_);
      return NULL;
    }

    olc::vf2d seek (const olc::vi2d& target_in)
    {
      olc::vf2d desired = target_in - position_;
      desired = desired.norm ();
      desired *= static_cast<float> (maxSpeed_);

      olc::vf2d steer = desired - velocity_;
      if (steer.mag () > maxForce_)
      {
        steer = steer.norm ();
        steer *= maxForce_;
      } // end IF

      return steer;
    }

    typedef std::vector<olc::vi2d> positions_t;
    olc::vf2d eat (positions_t& food_in,
                   float nutrition_in,
                   int32_t perception_in)
    {
      int32_t record = std::numeric_limits<int32_t>::max ();
      olc::vi2d closest = {-1, -1};
      positions_t::iterator iterator = food_in.begin ();
      while (iterator != food_in.end ())
      {
        int32_t d = position_.dist (*iterator);
        if (d < maxSpeed_)
        {
          iterator = food_in.erase (iterator);
          health_ += nutrition_in;
          continue;
        } // end IF
        else if (d < record && d < perception_in)
        {
          record = d;
          closest = *iterator;
        } // end ELSE IF
        iterator++;
      } // end FOR

      if (closest.x != -1 && closest.y != -1)
        return seek (closest);

      return {0.0f, 0.0f};
    }

    void behaviors (positions_t& food_in, positions_t& poison_in)
    {
      olc::vf2d steerF = eat (food_in, ENGINE_PGE_36_DEFAULT_FOOD_NUTRITION, dna_.foodPerception);
      olc::vf2d steerP = eat (poison_in, ENGINE_PGE_36_DEFAULT_POISON_NUTRITION, dna_.poisonPerception);

      steerF *= dna_.foodWeight;
      steerP *= dna_.poisonWeight;

      applyForce (steerF);
      applyForce (steerP);
    }

    bool dead ()
    {
      return (health_ <= 0.0f);
    }

    void display (olc::PixelGameEngine* engine_in,
                  bool debug_in)
    {
      static Common_GL_Color_t green = {0, 255, 0};
      static Common_GL_Color_t red = {255, 0, 0};
      float value_f = (health_ < 0.0f ? 0.0f : health_);
      value_f = (value_f > 1.0f ? 1.0f : value_f);
      Common_GL_Color_t lerped = Common_GL_Tools::lerpRGB (red, green, value_f);

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
      //engine_in->DrawLine (p2, p1, { lerped.r, lerped.g, lerped.b, 255 }, 0xFFFFFFFF);
      //engine_in->DrawLine (p3, p1, { lerped.r, lerped.g, lerped.b, 255 }, 0xFFFFFFFF);
      //engine_in->DrawLine (p2, p3, { lerped.r, lerped.g, lerped.b, 255 }, 0xFFFFFFFF);
      engine_in->FillTriangle (p1, p2, p3, {lerped.r, lerped.g, lerped.b, 255});
      //engine_in->FillCircle(position_, radius_, { lerped.r, lerped.g, lerped.b, 255 });
    }

    void boundaries (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d desired = {0.0f, 0.0f};

      if (position_.x < 0)
        desired = {static_cast<float> (maxSpeed_), velocity_.y};
      else if (position_.x > engine_in->ScreenWidth () - 1)
        desired = {static_cast<float> (-maxSpeed_), velocity_.y};
      else if (position_.y < 0)
        desired = {velocity_.x, static_cast<float> (maxSpeed_)};
      else if (position_.y > engine_in->ScreenHeight () - 1)
        desired = {velocity_.x, static_cast<float> (-maxSpeed_)};

      if (desired.x != 0.0f && desired.y != 0.0f)
      {
        desired = desired.norm ();
        desired *= static_cast<float> (maxSpeed_);
        olc::vf2d steer = desired - velocity_;
        if (steer.mag () > maxForce_)
        {
          steer = steer.norm ();
          steer *= maxForce_;
        } // end IF
        applyForce (steer);
      } // end IF
    }

   public:
    olc::vi2d  position_;
    olc::vf2d  acceleration_;
    olc::vf2d  velocity_;
    int32_t    radius_;
    int32_t    maxSpeed_;
    float      maxForce_;
    float      health_;
    struct dna dna_;
  };

  typedef std::list<vehicle*> vehicles_t;
  vehicles_t           vehicles_;
  vehicle::positions_t food_;
  vehicle::positions_t poison_;
};

#endif // PGE_36_H
