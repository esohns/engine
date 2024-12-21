#ifndef PGE_421_H
#define PGE_421_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_19.h"

class PGE_421
 : public olc::PixelGameEngine
{
 public:
  PGE_421 ();
  virtual ~PGE_421 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class branch
  {
   public:
    branch (branch* par, const olc::vf2d& pos, const olc::vf2d& dir)
     : parent (par)
     , position (pos)
     , direction (dir)
     , originalDirection (dir)
     , count (0)
     , length (ENGINE_PGE_421_DEFAULT_BRANCH_LENGTH)
    {}

    void reset ()
    {
      direction = originalDirection;
      count = 0;
    }

    branch* next ()
    {
      olc::vf2d nextDir = direction * length;
      olc::vf2d nextPos = position + nextDir;
      return new branch (this, nextPos, direction);
    }

    void show (olc::PixelGameEngine* engine)
    {
      if (parent != NULL)
        engine->DrawLine (position, parent->position, olc::DARK_YELLOW, 0xFFFFFFFF);
    }

    branch*   parent;
    olc::vf2d position;
    olc::vf2d direction;
    olc::vf2d originalDirection;
    int       count;
    float     length;
  };
 
  class leaf
  {
   public:
    leaf (olc::vf2d& pos)
     : position (pos)
     , reached (false)
    {}

    void show (olc::PixelGameEngine* engine)
    {
      engine->FillRect (position, { 3, 3 }, olc::GREEN);
    }

    olc::vf2d position;
    bool      reached;
  };

  class tree
  {
   public:
    tree ()
     : branches_ ()
     , leaves_ ()
    {}

    void clear ()
    {
      for (std::vector<branch*>::iterator iterator = branches_.begin ();
           iterator != branches_.end ();
           iterator++)
        delete (*iterator);
      branches_.clear ();
      leaves_.clear ();
    }

    void addLeaves (olc::PixelGameEngine* engine)
    {
      olc::vf2d mouse_position_s (static_cast<float> (engine->GetMouseX ()),
                                  static_cast<float> (engine->GetMouseY ()));

      if (leaves_.empty ())
        branches_.push_back (new branch (NULL, mouse_position_s, olc::vf2d (0.0f, 0.0f)));

      for (int i = 0; i < 10; i++)
      {
        float distance = Common_Tools::getRandomNumber (10.0f, 50.0f);
        olc::vf2d position (Common_Tools::getRandomNumber (0.0f, 1.0f),
                            Common_Tools::getRandomNumber (0.0f, 1.0f));
        position = position.norm ();
        position *= distance;
        position = mouse_position_s + position;
        leaves_.push_back (leaf (position));
      } // end FOR
    }

    void grow ()
    {
      for (std::vector<leaf>::iterator iterator = leaves_.begin ();
           iterator != leaves_.end ();
           iterator++)
      {
        branch* closestBranch = NULL;
        float record = ENGINE_PGE_421_DEFAULT_MAX_DISTANCE;

        for (std::vector<branch*>::iterator iterator_2 = branches_.begin ();
             iterator_2 != branches_.end ();
             iterator_2++)
        {
          float d = (*iterator).position.dist ((*iterator_2)->position);
          if (d < ENGINE_PGE_421_DEFAULT_MIN_DISTANCE)
          {
            (*iterator).reached = true;
            closestBranch = NULL;
            break;
          } // end IF
          else if (d < record)
          {
            closestBranch = (*iterator_2);
            record = d;
          } // end ELSE IF
        } // end FOR

        if (closestBranch != NULL)
        {
          olc::vf2d newDir = (*iterator).position - closestBranch->position;
          newDir = newDir.norm ();
          closestBranch->direction += newDir;
          closestBranch->count++;
        } // end IF
      } // end FOR

      for (std::vector<leaf>::iterator iterator = leaves_.begin ();
           iterator != leaves_.end ();
           )
      {
        if ((*iterator).reached)
        {
          iterator = leaves_.erase (iterator);
          continue;
        } // end IF

        ++iterator;
      } // end FOR

      for (int i = static_cast<int> (branches_.size ()) - 1;
           i >= 0;
           --i)
      {
        branch& branch_s = *branches_[i];
        if (branch_s.count > 0)
        {
          branch_s.direction /= static_cast<float> (branch_s.count + 1);
          branches_.push_back (branch_s.next ());
          branch_s.reset ();
        } // end IF
      } // end FOR
    }

    void show (olc::PixelGameEngine* engine)
    {
      for (std::vector<leaf>::iterator iterator = leaves_.begin ();
           iterator != leaves_.end ();
           iterator++)
        (*iterator).show (engine);

      for (std::vector<branch*>::iterator iterator = branches_.begin ();
           iterator != branches_.end ();
           iterator++)
        (*iterator)->show (engine);
    }

    std::vector<branch*> branches_;
    std::vector<leaf>    leaves_;
  };

  tree tree_;
};

#endif // PGE_421_H
