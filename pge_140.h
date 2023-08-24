#ifndef PGE_140_H
#define PGE_140_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_6.h"

class PGE_140
 : public olc::PixelGameEngine
{
 public:
  PGE_140 ();
  inline virtual ~PGE_140 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    float x;
    float y;
    float z;
    bool  stayOrGo_;

    agent ()
     : x (-1.0f)
     , y (Common_Tools::getRandomNumber (-2.0f, 1.0f))
     , z (Common_Tools::getRandomNumber (0.5f, 1.0f))
     , stayOrGo_ (Common_Tools::testRandomProbability (0.6f))
    {}
  };

  class system
  {
   public:
    std::vector<agent> agents_;

    void initialize ()
    {
      for (int i = ENGINE_PGE_140_DEFAULT_NUMBER_OF_AGENTS; i--;)
        agents_.push_back (agent ());
    }

    void update ()
    {
      for (std::vector<agent>::iterator iterator = agents_.begin ();
           iterator != agents_.end ();
           ++iterator)
      {
        (*iterator).y -= 0.01f;
        if ((*iterator).y < -2)
        {
          agent respawn = agent ();
          (*iterator).x = respawn.x;
          (*iterator).y = 2;
          (*iterator).z = respawn.z;
        }
        if ((*iterator).stayOrGo_)
        {
          if ((*iterator).y < 0.0f)
            (*iterator).x += 0.016f;
        } // end IF
      } // end FOR
    }

    float dist2 (agent& a, agent& b)
    {
      return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    }

    void relativeNeighborhoodGraph (olc::PixelGameEngine* engine_in,
                                    float zoom)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      for (size_t i = agents_.size (); i--;)
        for (size_t j = i; j--;)
        {
          float d = dist2 (agents_[j], agents_[i]);
          bool condition_b = true;
          for (std::vector<agent>::iterator iterator = agents_.begin ();
               iterator != agents_.end ();
               ++iterator)
            if (d > dist2 (agents_[i], *iterator) && d > dist2 (agents_[j], *iterator))
            {
              condition_b = false;
              break;
            } // end IF
          if (condition_b)
          {
            float x1 = (agents_[i].x / agents_[i].z) * zoom;
            float y1 = (agents_[i].y / agents_[i].z) * zoom;
            float x2 = (agents_[j].x / agents_[j].z) * zoom;
            float y2 = (agents_[j].y / agents_[j].z) * zoom;
            //strokeWeight(2 / (agents_[i].z * agents_[j].z));
            engine_in->DrawLine (half_width_i + static_cast<int32_t> (x1), half_height_i + static_cast<int32_t> (y1),
                                 half_width_i + static_cast<int32_t> (x2), half_height_i + static_cast<int32_t> (y2),
                                 olc::BLACK, 0xFFFFFFFF);
          } // end IF
        } // end FOR
    }
  };

  system system_;
  float  zoom_;
};

#endif // PGE_140_H
