#ifndef PGE_399_H
#define PGE_399_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_18.h"

class PGE_399
 : public olc::PixelGameEngine
{
 public:
  PGE_399 ();
  inline virtual ~PGE_399 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void createNodes ();
  void createLinks ();
  void grabNodesNearMouse ();

  struct node
  {
    void update (float limit)
    {
      if (pinned_)
        return;
      olc::vf2d acc = force_ * ENGINE_PGE_399_DEFAULT_FORCE_FACTOR;

      velocity_ += acc;
      if (velocity_.mag () > limit)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= limit;
      } // end IF
      position_ += velocity_;

      force_ *= 0.0f;
      velocity_ *= ENGINE_PGE_399_DEFAULT_FRICTION;
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d force_;
    bool      pinned_;
  };
  std::vector<struct node> nodes_;

  struct link
  {
    void update ()
    {
      olc::vf2d difference_s = p2_->position_ - p1_->position_;
      if (!p1_->pinned_)
        p1_->force_ += difference_s;
      if (!p2_->pinned_)
        p2_->force_ -= difference_s;
    }

    void show (olc::PixelGameEngine* engine)
    {
      engine->DrawLine (p1_->position_,
                        p2_->position_,
                        olc::BLACK,
                        0xFFFFFFFF);
    }

    node* p1_;
    node* p2_;
  };
  std::vector<struct link> links_;

  float                    speedLimit_;
};

#endif // PGE_399_H
