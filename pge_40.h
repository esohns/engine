#ifndef PGE_40_H
#define PGE_40_H

#include "ace/OS.h"

#include "olcPixelGameEngine.h"

#include "defines_2.h"

class PGE_40
 : public olc::PixelGameEngine
{
 public:
  PGE_40 ();
  virtual ~PGE_40 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class point;
  class stick
  {
   public:
    stick (point& p0_in, point& p1_in, float length_in)
     : selected_(false)
     , p0 (&p0_in)
     , p1 (&p1_in)
     , length_ (length_in)
     , active_ (true)
    {
    }

    void update ()
    {
      if (!active_)
        return;

      olc::vf2d diff = p0->position_ - p1->position_;
      float dist = std::sqrt (diff.x * diff.x + diff.y * diff.y);
      float diffFactor = (length_ - dist) / dist;
      olc::vf2d offset = diff * diffFactor * 0.5f;

      p0->position_ = {p0->position_.x + offset.x, p0->position_.y + offset.y};
      p1->position_ = {p1->position_.x - offset.x, p1->position_.y - offset.y};
    }

    void draw (olc::PixelGameEngine* engine_in) const
    {
      if (!active_)
        return;

      engine_in->DrawLine (p0->position_, p1->position_, selected_ ? olc::RED : olc::WHITE, 0xFFFFFFFF);
    }

    void deactivate ()
    {
      active_ = false;
    }

    bool selected_;

   private:
    point* p0;
    point* p1;
    float  length_;
    bool   active_;
  };

  class point
  {
   public:
    point (float x, float y)
     : position_ (x, y)
     , prevPosition_ (x, y)
     , initPosition_ (x, y)
     , pinned_ (false)
     , selected_ (false)
     , sticks_ ()
    {
      ACE_OS::memset (sticks_, 0, sizeof (stick*[2]));
    }

    void addStick (stick* stick_in, int index_in)
    { ACE_ASSERT (index_in <= 1);
      sticks_[index_in] = stick_in;
    }

    void pin ()
    {
      pinned_ = true;
    }

    void update (float deltaTime_in, float drag_in, const olc::vf2d& acceleration_in, float elasticity_in, olc::PixelGameEngine* engine_in)
    {
      olc::vi2d mouse_position = engine_in->GetMousePos ();
      olc::vf2d cursorToPosDir = position_ - mouse_position;
      float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
      selected_ =
        cursorToPosDist < ENGINE_PGE_40_DEFAULT_CURSOR_SIZE * ENGINE_PGE_40_DEFAULT_CURSOR_SIZE;

      if (sticks_[0])
        sticks_[0]->selected_ = selected_;
      if (sticks_[1])
        sticks_[1]->selected_ = selected_;

      if (engine_in->GetMouse (0).bHeld && selected_)
      {
        static olc::vi2d prev_mouse_position = {0, 0};
        olc::vf2d difference = mouse_position - prev_mouse_position;
        prev_mouse_position = mouse_position;

        difference *= ENGINE_PGE_40_DEFAULT_MOUSE_FACTOR;

        // constrain difference by elasticity
        if (difference.x > elasticity_in) difference.x = elasticity_in;
        if (difference.y > elasticity_in) difference.y = elasticity_in;
        if (difference.x < -elasticity_in) difference.x = -elasticity_in;
        if (difference.y < -elasticity_in) difference.y = -elasticity_in;

        prevPosition_ = position_ - difference;
      } // end IF

      if (engine_in->GetMouse (1).bHeld && selected_)
      {
        if (sticks_[0])
          sticks_[0]->deactivate ();
        if (sticks_[1])
          sticks_[1]->deactivate ();
      } // end IF

      if (pinned_)
      {
        position_ = initPosition_;
        return;
      } // end IF

      // verlet integration
      olc::vf2d newPos =
        position_ + (position_ - prevPosition_) * (1.0f - drag_in) + acceleration_in * (1.0f - drag_in) * deltaTime_in * deltaTime_in;
//      2.0f * position_ - prevPosition_ + acceleration_in * deltaTime_in * deltaTime_in;
      prevPosition_ = position_;
      position_ = newPos;
      keepInsideView (engine_in->ScreenWidth (), engine_in->ScreenHeight ());
    }

    olc::vf2d position_;

   private:
    olc::vf2d prevPosition_;
    olc::vf2d initPosition_;
    bool pinned_;
    bool selected_;
    stick* sticks_[2];

    void keepInsideView (int windowWidth_in, int windowHeight_in)
    {
      if (position_.x >= static_cast<float> (windowWidth_in))
      {
        position_.x = static_cast<float> (windowWidth_in - 1);
        prevPosition_.x = position_.x;
      } // end IF
      else if (position_.x < 0.0f)
      {
        position_.x = 0.0f;
        prevPosition_.x = position_.x;
      } // end ELSE IF

      if (position_.y >= static_cast<float> (windowHeight_in))
      {
        position_.y = static_cast<float> (windowHeight_in - 1);
        prevPosition_.y = position_.y;
      } // end IF
      else if (position_.y < 0)
      {
        position_.y = 0.0f;
        prevPosition_.y = position_.y;
      } // end ELSE IF
    }
  };

  class cloth
  {
   public:
    cloth (int width, int height, int spacing, int startX, int startY)
     : gravity_ (0.0f, ENGINE_PGE_40_DEFAULT_GRAVITY)
     , drag_ (ENGINE_PGE_40_DEFAULT_DRAG)
     , elasticity_ (ENGINE_PGE_40_DEFAULT_ELASTICITY)
    {
      for (int y = 0; y <= height; y++)
        for (int x = 0; x <= width; x++)
        {
          point* point_p =
            new point (static_cast<float> (startX + x * spacing), static_cast<float> (startY + y * spacing));

          if (x != 0)
          {
            point* leftPoint = points_[points_.size () - 1];
            stick* s = new stick (*point_p, *leftPoint, static_cast<float> (spacing));
            leftPoint->addStick (s, 0);
            point_p->addStick (s, 0);
            sticks_.push_back (s);
          } // end IF

          if (y != 0)
          {
            point* upPoint = points_[x + (y - 1) * (width + 1)];
            stick* s = new stick (*point_p, *upPoint, static_cast<float> (spacing));
            upPoint->addStick (s, 1);
            point_p->addStick (s, 1);
            sticks_.push_back (s);
          } // end IF

          if (y == 0 && (x % ENGINE_PGE_40_DEFAULT_Y0_X_MOD_PIN == 0))
            point_p->pin ();

          points_.push_back (point_p);
        } // end FOR
    }
    ~cloth ()
    {
      for (std::vector<point*>::iterator iterator = points_.begin ();
           iterator != points_.end ();
           ++iterator)
        delete *iterator;

      for (std::vector<stick*>::iterator iterator = sticks_.begin ();
           iterator != sticks_.end ();
           ++iterator)
        delete* iterator;
    }

    void update (olc::PixelGameEngine* engine_in, float deltaTime_in)
    {
      for (int i = 0; i < points_.size (); i++)
        points_[i]->update (deltaTime_in, drag_, gravity_, elasticity_, engine_in);

      for (int i = 0; i < sticks_.size (); i++)
        sticks_[i]->update ();
    }

    void draw (olc::PixelGameEngine* engine_in) const
    {
      for (int i = 0; i < sticks_.size (); i++)
        sticks_[i]->draw (engine_in);
    }

   private:
    olc::vf2d           gravity_;
    float               drag_;
    float               elasticity_;
    std::vector<point*> points_;
    std::vector<stick*> sticks_;
  };

  cloth* cloth_;
};

#endif // PGE_40_H
