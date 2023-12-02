#ifndef PGE_19_H
#define PGE_19_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines.h"

class PGE_19
 : public olc::PixelGameEngine
{
 private:
  class segment
  {
   public:
    segment (olc::vd2d position_in)
     : angle_ (0.0f)
     , positionA_ (position_in)
    {
      calculateB ();
    }
    segment (const segment& parent_in)
     : angle_ (0.0f)
     , positionA_ (parent_in.positionB_)
    {
      calculateB ();
    }

    void update ()
    {
      calculateB ();
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (positionA_, positionB_, olc::WHITE, 0xFFFFFFFF);
    }

    void setA (olc::vd2d position_in)
    {
      positionA_ = position_in;
      calculateB ();
    }

    void calculateB ()
    {
      float dx = ENGINE_PGE_19_DEFAULT_SEGMENT_LENGTH * std::cos (angle_);
      float dy = ENGINE_PGE_19_DEFAULT_SEGMENT_LENGTH * std::sin (angle_);
      positionB_ = { positionA_.x + dx, positionA_.y + dy };
    }

    void follow (olc::vd2d position_in)
    {
      olc::vd2d direction_s = position_in - positionA_;
      angle_ = static_cast<float> (std::atan2 (direction_s.y, direction_s.x));
      direction_s = direction_s.norm ();
      direction_s *= -ENGINE_PGE_19_DEFAULT_SEGMENT_LENGTH;
      positionA_ = position_in + direction_s;
    }
    void follow (const segment& segment_in)
    {
      follow (segment_in.positionA_);
    }

   public:
    float     angle_;
    olc::vd2d positionA_;
    olc::vd2d positionB_;
  };

  class arm
  {
   public:
    arm (olc::vd2d base_in)
     : base_ (base_in)
    {
      segments_.push_back (new segment ({ ENGINE_PGE_19_DEFAULT_WIDTH / 2.0F, ENGINE_PGE_19_DEFAULT_HEIGHT / 2.0F }));
      for (int i = 1; i < ENGINE_PGE_19_DEFAULT_SEGMENTS; ++i)
        segments_.push_back (new segment (*segments_[i - 1]));
    }
    ~arm ()
    {
      for (int i = 0; i < segments_.size (); ++i)
        delete segments_[i];
    }

    void update (olc::vd2d position_in)
    {
      segment* end_p = segments_[segments_.size () - 1];
      end_p->follow (position_in);
      end_p->update ();

      for (int i = static_cast<int> (segments_.size () - 2); i >= 0; --i)
      {
        segments_[i]->follow (*segments_[i + 1]);
        segments_[i]->update ();
      } // end FOR

      segments_[0]->setA (base_);

      for (int i = 1; i < segments_.size (); ++i)
      {
        segments_[i]->setA (segments_[i - 1]->positionB_);
      } // end FOR
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      for (int i = 0; i < segments_.size (); ++i)
      {
        segments_[i]->show (engine_in);
      } // end FOR
    }

   public:
    olc::vd2d             base_;
    std::vector<segment*> segments_;
  };

 public:
  PGE_19 ();
  ~PGE_19 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<arm*> arms_;
};

#endif // PGE_19_H
