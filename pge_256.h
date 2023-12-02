#ifndef PGE_256_H
#define PGE_256_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_11.h"

olc::vf2d getGridPos (int, int, float, float&);
float smoothMin (float, float, float);
float diameterFunc (float);

class PGE_256
 : public olc::PixelGameEngine
{
 public:
  PGE_256 ();
  inline virtual ~PGE_256 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class zap
  {
   public:
    zap (olc::Pixel& color, int layerCounter)
     : color_ (color)
     , length_ (ENGINE_PGE_256_DEFAULT_ZAPLENGTH)
     , phase_ (0.0f)
     , alive_ (true)
     , trail_ ()
    {
      initTrail (layerCounter);
    }

    void initTrail (int layerCounter)
    {
      trail_.clear ();
      int j = Common_Tools::getRandomNumber (0, ENGINE_PGE_256_DEFAULT_MAXJ);
      for (int i = 0; i <= length_; i++)
        trail_.push_back (olc::vi2d (i + layerCounter, j));
    }

    void update (int layerCounter)
    {
      phase_ += 1 / 6.0f;
      if (phase_ > 1.0f)
      {
        phase_ -= 1.0f;
        olc::vi2d head = trail_[length_];
        trail_.erase (trail_.begin ());
        trail_.push_back ({head.x + 1, head.y - (int)std::floor (Common_Tools::getRandomNumber (0.0f, 2.0f))});
        if (trail_[0].x > 22 + layerCounter)
          alive_ = false;
      } // end IF
    }

    static float weightFunction (float x)
    {
      return 1.0f - std::abs ((x - (ENGINE_PGE_256_DEFAULT_ZAPLENGTH - 1) / 2.0f) / static_cast<float> ((ENGINE_PGE_256_DEFAULT_ZAPLENGTH - 1) * 2.0f));
    }

    void show (olc::PixelGameEngine* engine_in, int layerCounter, float phase)
    {
      std::vector<olc::vf2d> vertices_forward_a;
      std::vector<olc::vf2d> vertices_backward_a;

      float tailCoef = zap::weightFunction (-phase_) / (zap::weightFunction (-phase_) - zap::weightFunction (1.0f - phase_));
      float dummy_f;
      olc::vf2d pos1 = getGridPos (trail_[0].x - layerCounter, trail_[0].y, phase, dummy_f);
      olc::vf2d pos2 = getGridPos (trail_[1].x - layerCounter, trail_[1].y, phase, dummy_f);
      vertices_forward_a.push_back (olc::vf2d (pos1.x * (1.0f - tailCoef) + pos2.x * tailCoef,
                                               pos1.y * (1.0f - tailCoef) + pos2.y * tailCoef));

      for (int i = 0; i < length_ - 1; i++)
      {
        olc::vf2d pos1a = getGridPos (trail_[i].x - layerCounter, trail_[i].y, phase, dummy_f);
        float r;
        olc::vf2d pos2a = getGridPos (trail_[i + 1].x - layerCounter, trail_[i + 1].y, phase, r);
        olc::vf2d pos3a = getGridPos (trail_[i + 2].x - layerCounter, trail_[i + 2].y, phase, dummy_f);
        olc::vf2d vec1 = {pos2a.x - pos1a.x, pos2a.y - pos1a.y};
        olc::vf2d vec2 = {pos3a.x - pos2a.x, pos3a.y - pos2a.y};
        vec1 = vec1.norm ();
        vec2 = vec2.norm ();
        olc::vf2d vec2_inv = vec2;
        vec2_inv *= -1.0f;
        // angle between vec1 and vec2_inv
        float dot_f = vec1.x * vec2_inv.x + vec1.y * vec2_inv.y;
        float det_f = vec1.x * vec2_inv.y - vec1.y * vec2_inv.x;
        float angle_f = std::atan2 (det_f, dot_f);
        float angleFactor = std::abs (1.0f / std::sin (angle_f / 2.0f));
        float diamFactor = diameterFunc (r) / 12.0f;
        olc::vf2d temp = vec1 + vec2;
        olc::vf2d temp_rot;
        temp_rot.x =
          std::cos (-static_cast<float> (M_PI_2)) * temp.x - std::sin (-static_cast<float> (M_PI_2)) * temp.y;
        temp_rot.y =
          std::sin (-static_cast<float> (M_PI_2)) * temp.x + std::cos (-static_cast<float> (M_PI_2)) * temp.y;
        temp_rot = temp_rot.norm ();
        temp_rot *=
          static_cast<float> (ENGINE_PGE_256_DEFAULT_ZAPWIDTH) * zap::weightFunction (i + 1 - phase_) * angleFactor * diamFactor;
        olc::vf2d sideVector = temp_rot;
        //let sideVector = p5.Vector.add(vec1,vec2).rotate(-PI/2).setMag(5);

        vertices_forward_a.push_back ({pos2a.x + sideVector.x, pos2a.y + sideVector.y});
        vertices_backward_a.insert (vertices_backward_a.begin (), {pos2a.x - sideVector.x, pos2a.y - sideVector.y});
      } // end FOR

      float headCoef =
        zap::weightFunction (length_ - 1 - phase_) / (zap::weightFunction (length_ - 1 - phase_) - zap::weightFunction (length_ - phase_));
      pos1 =
        getGridPos (trail_[length_ - 1].x - layerCounter, trail_[length_ - 1].y, phase, dummy_f);
      pos2 =
        getGridPos (trail_[length_].x - layerCounter, trail_[length_].y, phase, dummy_f);
      vertices_backward_a.insert (vertices_backward_a.begin (), {pos1.x * (1.0f - tailCoef) + pos2.x * tailCoef, pos1.y * (1.0f - tailCoef) + pos2.y * tailCoef});

      olc::vf2d prev_s;
      bool is_first_b = true;
      for (std::vector<olc::vf2d>::iterator iterator = vertices_forward_a.begin ();
           iterator != vertices_forward_a.end ();
           ++iterator)
      {
        if (is_first_b)
        {
          is_first_b = false;
          prev_s = *iterator;
        } // end IF
        engine_in->DrawLine (prev_s, *iterator, color_, 0xFFFFFFFF);
        prev_s = *iterator;
      } // end FOR
      is_first_b = true;
      for (std::vector<olc::vf2d>::iterator iterator = vertices_backward_a.begin ();
           iterator != vertices_backward_a.end ();
           ++iterator)
      {
        if (is_first_b)
        {
          is_first_b = false;
          prev_s = *iterator;
        } // end IF
        engine_in->DrawLine (prev_s, *iterator, color_, 0xFFFFFFFF);
        prev_s = *iterator;
      } // end FOR
      //engine_in->DrawLine (prev_s, *vertices_forward_a.begin (), color_, 0xFFFFFFFF);
    }

    olc::Pixel             color_;
    int                    length_;
    float                  phase_;
    bool                   alive_;
    std::vector<olc::vi2d> trail_;
  };

  float            currentHue_;
  int              layerCounter_;
  float            phase_;
  float            spawnPhase_;
  std::vector<zap> zaps_;

  void render ();
};

#endif // PGE_256_H
