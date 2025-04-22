#ifndef PGE_112_H
#define PGE_112_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

class PGE_112
 : public olc::PixelGameEngine
{
 public:
  PGE_112 ();
  virtual ~PGE_112 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class mover
  {
   public:
    olc::vf2d  direction_;
    olc::vf2d  position_;

    float      speed_;
    float      SPEED_;

    float      noiseScale_;
    float      noiseStrength_;
    float      forceStrength_;

    float      radius_;

    olc::Pixel color_;

    mover (olc::PixelGameEngine* engine_in)
    {
      setRandomValues (engine_in);
    }

    //mover (float x, float y)
    //{
    //  setRandomValues ();
    //}

    void setRandomValues (olc::PixelGameEngine* engine_in)
    {
      position_ = olc::vf2d (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                             Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)));
      radius_ = Common_Tools::getRandomNumber (2.0f, 7.5f);

      float angle = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      direction_ = olc::vf2d (std::cos (angle), std::sin (angle));

      speed_ = Common_Tools::getRandomNumber (4.0f, 7.0f);
      SPEED_ = speed_;
      noiseScale_ = 80.0f;
      noiseStrength_ = 1.0f;
      forceStrength_ = Common_Tools::getRandomNumber (0.1f, 0.2f);

      setRandomColor ();
    }

    void setRandomColor ()
    {
      int colorDice = Common_Tools::getRandomNumber (0, 3);
      if (colorDice == 0) color_ = {0xff, 0xed, 0xbc, 255};
      else if (colorDice == 1) color_ = {0xA7, 0x52, 0x65, 255};
      else if (colorDice == 2) color_ = {0xec, 0x72, 0x63, 255};
      else color_ = {0xfe, 0xbe, 0x7e, 255};
    }

    void update (olc::PixelGameEngine* engine_in,
                 noise::module::Perlin* noise_in,
                 int32_t frameCount_in)
    {
      update (0, engine_in, noise_in, frameCount_in);
    }

    void update (int mode,
                 olc::PixelGameEngine* engine_in,
                 noise::module::Perlin* noise_in,
                 int32_t frameCount_in)
    {
      if (mode == 0) // bouncing ball
      {
        speed_ = SPEED_ * 0.7f;
        move ();
        checkEdgesAndBounce (engine_in);
      }
      else if (mode == 1) // noise
      {
        speed_ = SPEED_ * 0.7f;
        addNoise (noise_in, frameCount_in);
        move ();
        checkEdgesAndRelocate (engine_in);
      }
      else if (mode == 2) // steer
      {
        steer (static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ()));
        move ();
      }
      else if (mode == 3) // seek
      {
        speed_ = SPEED_ * 0.7f;
        seek (static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ()));
        move ();
      }
      else // radial
      {
        speed_ = SPEED_ * 0.7f;
        addRadial (engine_in, noise_in, frameCount_in);
        move ();
        checkEdges (engine_in);
      }

      show (engine_in);
    }

    void flock (std::vector<mover*>& boids)
    {
      olc::vf2d other;
      float otherSize;

      olc::vf2d cohesionSum (0.0f, 0.0f);
      float cohesionCount = 0;

      olc::vf2d seperationSum (0.0f, 0.0f);
      float seperationCount = 0;

      olc::vf2d alignSum (0.0f, 0.0f);
      float speedSum = 0;
      float alignCount = 0;

      for (size_t i = 0; i < boids.size (); i++)
      {
        other = boids[i]->position_;
        otherSize = boids[i]->radius_;

        float distance = position_.dist (other);
        if (distance > 0.0f && distance < 70.0f)
        {
          cohesionSum += other;
          cohesionCount++;

          alignSum += boids[i]->direction_;
          speedSum += boids[i]->speed_;
          alignCount++;
        } // end IF

        if (distance > 0.0f && distance < (radius_ + otherSize) * 1.2f)
        {
          float angle = std::atan2 (position_.y - other.y, position_.x - other.x);
          seperationSum += olc::vf2d (std::cos (angle), std::sin (angle));
          seperationCount++;
        } // end IF

        if (alignCount > 8 && seperationCount > 12)
          break;
      } // end FOR

      if (cohesionCount > 0)
      {
        cohesionSum /= cohesionCount;
        cohesion (cohesionSum, 1.0f);
      } // end IF

      if (alignCount > 0)
      {
        speedSum /= alignCount;
        alignSum /= alignCount;
        align (alignSum, speedSum, 1.3f);
      } // end IF

      if (seperationCount > 0)
      {
        seperationSum /= seperationCount;
        seperation (seperationSum, 2.0f);
      } // end IF
    }

    void cohesion (olc::vf2d force, float strength)
    {
      steer (force.x, force.y, strength);
    }

    void seperation (olc::vf2d force, float strength)
    {
      float limit_f = strength * forceStrength_;
      if (force.mag () > limit_f)
      {
        force = force.norm ();
        force *= limit_f;
      } // end IF

      direction_ += force;
      direction_ = direction_.norm ();

      speed_ *= 1.1f;
      if (speed_ < 0.0) speed_ = 0.0f;
      if (speed_ > SPEED_ * 1.5f) speed_ = SPEED_ * 1.5f;
    }

    void align (olc::vf2d force, float forceSpeed, float strength)
    {
      speed_ = Common_Math_Tools::lerp (speed_, forceSpeed, strength * forceStrength_);

      force = force.norm ();
      force *= strength * forceStrength_;

      direction_ += force;
      direction_ = direction_.norm ();
    }

    void steer (float x, float y)
    {
      steer (x, y, 1.0f);
    }

    void steer (float x, float y, float strength)
    {
      float angle = std::atan2 (y - position_.y, x - position_.x);

      olc::vf2d force (std::cos (angle), std::sin (angle));
      force *= forceStrength_ * strength;

      direction_ += force;
      direction_ = direction_.norm ();

      float currentDistance = position_.dist({x, y});
      if (currentDistance < 70.0f) speed_ = Common_GL_Tools::map (currentDistance, 0.0f, 70.0f, 0.0f, SPEED_);
      else speed_ = SPEED_;
    }

    void seek (float x, float y)
    {
      seek (x, y, 1.0f);
    }

    void seek (float x, float y, float strength)
    {
      float angle = std::atan2 (y - position_.y, x - position_.x);

      olc::vf2d force (std::cos (angle), std::sin (angle));
      force *= forceStrength_ * strength;

      direction_ += force;
      direction_ = direction_.norm ();
    }

    void addRadial (olc::PixelGameEngine* engine_in,
                    noise::module::Perlin* noise_in,
                    int32_t frameCount_in)
    {
      float m =
        static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (static_cast<double> (frameCount_in / (2.0 * noiseScale_)), 0.0, 0.0), -1.0, 1.0, 0.0, 1.0));
      m = Common_GL_Tools::map (m, 0.0f, 1.0f, -1.2f, 1.2f);

      olc::vf2d origin_s;
      float maxDistance = origin_s.dist ({engine_in->ScreenWidth () / 2.0f, engine_in->ScreenHeight () / 2.0f});
      float distance = position_.dist ({engine_in->ScreenWidth () / 2.0f, engine_in->ScreenHeight () / 2.0f});

      float angle = m * Common_GL_Tools::map (distance, 0.0f, maxDistance, 0.0f, 2.0f * static_cast<float> (M_PI));

      olc::vf2d force (std::cos (angle), std::sin (angle));
      force *= forceStrength_;

      direction_ += force;
      direction_ = direction_.norm ();
    }

    void addNoise (noise::module::Perlin* noise_in,
                   int32_t frameCount_in)
    {
      float noiseValue =
        static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (position_.x / noiseScale_, position_.y / noiseScale_, frameCount_in / noiseScale_), -1.0, 1.0, 0.0, 1.0));
      noiseValue *= 2.0f * static_cast<float> (M_PI) * noiseStrength_;

      olc::vd2d force (std::cos (noiseValue), std::sin (noiseValue));
      force *= forceStrength_;

      direction_ += force;
      direction_ = direction_.norm ();
    }

    void move ()
    {

      olc::vf2d velocity = direction_;
      velocity *= speed_;
      position_ += velocity;
    }

    void checkEdgesAndRelocate (olc::PixelGameEngine* engine_in)
    {
      float diameter = radius_ * 2.0f;

      if (position_.x < -diameter / 2)
      {
        position_.x = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenWidth () + diameter / 2.0f);
        position_.y = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenHeight () + diameter / 2);
      } // end IF
      else if (position_.x > engine_in->ScreenWidth () + diameter / 2.0f)
      {
        position_.x = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenWidth () + diameter / 2.0f);
        position_.y = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenHeight () + diameter / 2.0f);
      } // end ELSE IF

      if (position_.y < -diameter / 2.0f)
      {
        position_.x = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenWidth () + diameter / 2.0f);
        position_.y = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenHeight () + diameter / 2.0f);
      } // end IF
      else if (position_.y > engine_in->ScreenHeight () + diameter / 2.0f)
      {
        position_.x = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenWidth () + diameter / 2.0f);
        position_.y = Common_Tools::getRandomNumber (-diameter / 2.0f, engine_in->ScreenHeight () + diameter / 2.0f);
      } // end ELSE IF
    }

    void checkEdges (olc::PixelGameEngine* engine_in)
    {
      float diameter = radius_ * 2.0f;

      if (position_.x < -diameter / 2.0f)
        position_.x = engine_in->ScreenWidth () + diameter / 2.0f;
      else if (position_.x > engine_in->ScreenWidth () + diameter / 2.0f)
        position_.x = -diameter / 2.0f;

      if (position_.y < -diameter / 2.0f)
        position_.y = engine_in->ScreenHeight () + diameter / 2.0f;
      else if (position_.y > engine_in->ScreenHeight () + diameter / 2.0f)
        position_.y = -diameter / 2.0f;
    }

    void checkEdgesAndBounce (olc::PixelGameEngine* engine_in)
    {
      if (position_.x < radius_)
      {
        position_.x = radius_;
        direction_.x = direction_.x * -1.0f;
      } // end IF
      else if (position_.x > engine_in->ScreenWidth () - radius_)
      {
        position_.x = engine_in->ScreenWidth () - radius_;
        direction_.x *= -1.0f;
      } // end ELSE IF

      if (position_.y < radius_)
      {
        position_.y = radius_;
        direction_.y *= -1.0f;
      } // end IF
      else if (position_.y > engine_in->ScreenHeight () - radius_)
      {
        position_.y = engine_in->ScreenHeight () - radius_;
        direction_.y *= -1.0f;
      } // end ELSE IF
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (radius_), color_);
    }
  };

  std::vector<mover*>   movers_;
  bool                  transparentBG_;
  int                   mode_;
  noise::module::Perlin noise_;
};

#endif // PGE_112_H
