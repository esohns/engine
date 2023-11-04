#ifndef PGE_221_H
#define PGE_221_H

#include <numeric>
#include <utility>
#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

class PGE_221
 : public olc::PixelGameEngine
{
 public:
  PGE_221 ();
  inline virtual ~PGE_221 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (olc::PixelGameEngine* engine_in)
     : id_ (Common_Tools::getRandomNumber (0.0f, 10000.0f))
     , position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine_in->ScreenHeight () - 1)))
     , prevPosition_ (position_)
     , velocity_ ()
    {}

    std::pair<float, olc::vf2d> findClosestCornerPair (std::vector<agent*>& agents, float milliseconds, olc::PixelGameEngine* engine)
    {
      std::pair<float, olc::vf2d> closest;
      closest.first = std::numeric_limits<float>::max ();

      float range_factor_f =
        Common_GL_Tools::map (std::sin (milliseconds * 0.0005f + id_), -1.0f, 1.0f, 0.3f, 0.8f);
      float max_distance_f =
        std::min (static_cast<float> (engine->ScreenWidth ()), static_cast<float> (engine->ScreenHeight ())) * range_factor_f;
      std::vector<agent*> within_range_a;
      for (std::vector<agent*>::iterator iterator = agents.begin ();
           iterator != agents.end ();
           ++iterator)
        if ((*iterator)->position_.dist (position_) < max_distance_f)
          within_range_a.push_back (*iterator);
      if (within_range_a.size () == 0)
        return closest;

      static std::vector<float> sides_a = {-1.0f, 1.0f};
      for (std::vector<float>::iterator iterator = sides_a.begin ();
           iterator != sides_a.end ();
           ++iterator)
        for (std::vector<float>::iterator iterator_2 = sides_a.begin ();
             iterator_2 != sides_a.end ();
             ++iterator_2)
        {
          float xmax_f = std::numeric_limits<float>::min ();
          for (std::vector<agent*>::iterator iterator_3 = within_range_a.begin ();
               iterator_3 != within_range_a.end ();
               ++iterator_3)
          {
            float x = (*iterator_3)->position_.x * (*iterator);
            if (x > xmax_f)
              xmax_f = x;
          } // end FOR
          xmax_f += 1.0f;
          float ymax_f = std::numeric_limits<float>::min ();
          for (std::vector<agent*>::iterator iterator_3 = within_range_a.begin ();
               iterator_3 != within_range_a.end ();
               ++iterator_3)
          {
            float y = (*iterator_3)->position_.y * (*iterator_2);
            if (y > ymax_f)
              ymax_f = y;
          } // end FOR
          ymax_f += 1.0f;

          olc::vf2d position ((*iterator) * xmax_f,
                              (*iterator_2) * ymax_f);
          float score_f =
            static_cast<float> (std::pow (position_.dist (position) / std::min (static_cast<float> (engine->ScreenWidth ()), static_cast<float> (engine->ScreenHeight ())), 1)) * 10.0f * range_factor_f;
          if (score_f < closest.first)
          {
            closest.first = score_f;
            closest.second = position;
          } // end IF
        } // end FOR
      return closest;
    }

    class reduce_agent_positions
    {
     public:
      reduce_agent_positions (olc::vf2d& initial,
                              size_t size)
       : initial_ (initial)
       , size_ (size)
      { ACE_ASSERT (size_); }

      olc::vf2d& operator() (agent* lhs, agent* rhs)
      {
        olc::vf2d temp = lhs->position_;
        temp /= static_cast<float> (size_);
        initial_ += temp;

        temp = rhs->position_;
        temp /= static_cast<float> (size_);
        initial_ += temp;

        return initial_;
      }

      olc::vf2d& operator() (olc::vf2d& lhs, agent* rhs)
      {
        olc::vf2d temp = rhs->position_;
        temp /= static_cast<float> (size_);
        lhs += temp;

        return lhs;
      }
      olc::vf2d& operator() (agent* lhs, olc::vf2d& rhs)
      {
        olc::vf2d temp = lhs->position_;
        temp /= static_cast<float> (size_);
        rhs += temp;

        return rhs;
      }

     private:
      olc::vf2d& initial_;
      size_t     size_;
    };

    std::pair<float, olc::vf2d> findClosestSidePair (std::vector<agent*>& agents, float milliseconds, olc::PixelGameEngine* engine, noise::module::Perlin* noise)
    {
      std::pair<float, olc::vf2d> closest;
      closest.first = std::numeric_limits<float>::max ();

      float range_factor_f = Common_GL_Tools::map (std::sin (milliseconds * 0.0005f + id_), -1.0f, 1.0f, 0.6f, 0.8f);
      float size_factor_f = Common_GL_Tools::map (std::sin (milliseconds * 0.0007f), -1.0f, 1.0f, 0.2f, 0.3f);
      float max_distance_f = std::min (static_cast<float> (engine->ScreenWidth ()), static_cast<float> (engine->ScreenHeight ())) * range_factor_f;
      std::vector<agent*> within_range_a;
      for (std::vector<agent*>::iterator iterator = agents.begin ();
           iterator != agents.end ();
           ++iterator)
        if ((*iterator)->position_.dist (position_) < max_distance_f)
          within_range_a.push_back (*iterator);
      if (within_range_a.size () == 0)
        return closest;

      olc::vf2d average_s;
      reduce_agent_positions reduce_agent_positions_c (average_s, within_range_a.size ());
      average_s = std::reduce (within_range_a.begin (), within_range_a.end (), average_s, reduce_agent_positions_c);

      closest.second = position_;
      closest.second -= average_s;
      // position.x += map(noise(this.id, millis()*0.001, 0), 0, 1, -20, 20)
      // position.y += map(noise(this.id, millis()*0.001, 500), 0, 1, -20, 20)
      closest.second.x *=
        std::min (static_cast<float> (engine->ScreenWidth ()), static_cast<float> (engine->ScreenHeight ())) * size_factor_f / (std::abs (closest.second.x) + 1e-4f);
      closest.second.y *=
        std::min (static_cast<float> (engine->ScreenWidth ()), static_cast<float> (engine->ScreenHeight ())) * size_factor_f / (std::abs (closest.second.y) + 1e-4f);
      closest.second += average_s;

      if (std::abs (closest.second.x - position_.x) < std::abs (closest.second.y - position_.y))
        closest.second.y =
          position_.y + Common_GL_Tools::map (static_cast<float> (noise->GetValue (id_, milliseconds * 0.001, 2000.0)), -1.0f, 1.0f, -10.0f, 10.0f);
      else
        closest.second.x =
          position_.x + Common_GL_Tools::map (static_cast<float> (noise->GetValue (id_, milliseconds * 0.001, 2500.0f)), -1.0f, 1.0f, -10.0f, 10.0f);

      closest.first =
        static_cast<float> (std::pow (position_.dist (closest.second) / std::min (static_cast<float> (engine->ScreenWidth ()), static_cast<float> (engine->ScreenHeight ())), 2)) * 0.5f / range_factor_f;
      return closest;
    }

    void plan (std::vector<agent>& agents, float milliseconds, olc::PixelGameEngine* engine, noise::module::Perlin* noise)
    {
      std::vector<agent*> others_a;
      for (std::vector<agent>::iterator iterator = agents.begin ();
           iterator != agents.end ();
           ++iterator)
        if ((*iterator).id_       != id_ &&
            (*iterator).position_ != position_)
          others_a.push_back (&*iterator);

      std::pair<float, olc::vf2d> closestCornerPair =
        findClosestCornerPair (others_a, milliseconds, engine);
      std::pair<float, olc::vf2d> closestSidePair =
        findClosestSidePair (others_a, milliseconds, engine, noise);

      // randomly adjust scores to randomize chances that an agent
      // tries to be a corner or part of a side
      closestCornerPair.first +=
        Common_GL_Tools::map (static_cast<float> (noise->GetValue (id_, milliseconds * 0.001, 1000.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      closestSidePair.first +=
        Common_GL_Tools::map (static_cast<float> (noise->GetValue (id_, milliseconds * 0.001, 1500.0)), -1.0f, 1.0f, 0.0f, 1.0f);

      if (closestCornerPair.first < 5.0f && closestCornerPair.first < closestSidePair.first)
      {
        // Move to become the corner
        olc::vf2d toCorner = closestCornerPair.second;
        toCorner -= position_;
        float magnitude_f = std::min (toCorner.mag () * 0.8f, 5.0f);
        toCorner = toCorner.norm ();
        toCorner *= magnitude_f;
        velocity_.lerp (toCorner, 0.5);
      } // end IF
      else if (closestSidePair.first < 5.0f)
      {
        // Move to become part of the side
        olc::vf2d toSide = closestSidePair.second;
        toSide -= position_;
        float magnitude_f = std::min (toSide.mag () * 0.8f, 5.0f);
        toSide = toSide.norm ();
        toSide *= magnitude_f;
        velocity_.lerp (toSide, 0.5);
      } // end ELSE IF
      else
      {
        // Random walk
        olc::vf2d randomNewVelocity = velocity_;
        randomNewVelocity += olc::vf2d (static_cast<float> (noise->GetValue (id_, milliseconds * 0.005, 3000.0)),
                                        static_cast<float> (noise->GetValue (id_, milliseconds * 0.005, 3500.0)));

        randomNewVelocity = randomNewVelocity.norm ();
        randomNewVelocity *= 5.0f;
        velocity_.lerp (randomNewVelocity, 0.1);
      } // end ELSE

      // Keep from drifting by going back to the center
      olc::vf2d temp (engine->ScreenWidth () / 2.0f, engine->ScreenHeight () / 2.0f);
      temp = position_ - temp;
      temp *= -0.007f;
      velocity_ += temp;
    }

    void update ()
    {
      prevPosition_ = position_;
      position_ += velocity_;
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (prevPosition_, position_, olc::WHITE, 0xFFFFFFFF);
      engine_in->FillCircle (position_, 3, olc::WHITE);
    }

    float     id_;
    olc::vf2d position_;
    olc::vf2d prevPosition_;
    olc::vf2d velocity_;
  };

  std::vector<agent>    agents_;
  noise::module::Perlin noise_;
};

#endif // PGE_221_H
