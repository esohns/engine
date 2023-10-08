#ifndef PGE_199_H
#define PGE_199_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_8.h"

class PGE_199
 : public olc::PixelGameEngine
{
 public:
  PGE_199 ();
  inline virtual ~PGE_199 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class node
  {
   public:
    node (float x, float y)
     : position_ (x, y)
    {}

    inline bool operator== (node& rhs) { return position_ == rhs.position_; }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, ENGINE_PGE_199_DEFAULT_NODE_RADIUS, olc::BLACK);
    }

    olc::vf2d position_;
  };

  class rope
  {
   public:
    rope ()
     : nodes_ ()
    {}

    void addNode (node node)
    {
      nodes_.push_back (node);
    }

    void relax ()
    {
      for (size_t i = 1; i < nodes_.size () - 2; i++)
        for (std::vector<node>::iterator iterator = nodes_.begin ();
             iterator != nodes_.end ();
             ++iterator)
        {
          if (*iterator == nodes_[i])
            continue;
          olc::vf2d diff = nodes_[i].position_ - (*iterator).position_;
          float dist = diff.mag ();
          if (dist < 2.0f * ENGINE_PGE_199_DEFAULT_NODE_RADIUS)
          {
            float rdist = 2.0f * ENGINE_PGE_199_DEFAULT_NODE_RADIUS - dist;
            diff = diff.norm ();
            diff *= rdist;
            nodes_[i].position_ += diff;
          } // end IF
        } // end FOR
    }

    void limitLength ()
    {
      for (size_t i = 1; i < nodes_.size () - 2; i++)
      {
        float dist =
          nodes_[i - 1].position_.dist (nodes_[i].position_) + nodes_[i].position_.dist (nodes_[i + 1].position_);
        if (dist > ENGINE_PGE_199_DEFAULT_LENGTH_LIMIT * 4.0f)
        {
          olc::vf2d pos = nodes_[i - 1].position_ + nodes_[i + 1].position_;
          pos /= 2.0f;
          nodes_[i].position_ = pos;
        } // end IF
      } // end FOR
    }

    olc::vf2d lineBlend (olc::vf2d A, olc::vf2d B, float fac)
    {
      A *= 1.0f - fac;
      B *= fac;
      return A + B;
    }

    void resampleFB ()
    { //resample forward and backward
      float accLen = 0.0f; // accumulated length
      float placedLen = 0.0f;
      //calculate new node
      std::vector<node> Nnodes;
      node& lastNode = nodes_[0];
      Nnodes.push_back (nodes_[nodes_.size () - 1]);
      for (size_t i = nodes_.size () - 1; i >= 1; i--)
      {
        olc::vf2d diff = nodes_[i - 1].position_ - nodes_[i].position_;
        float len = diff.mag ();
        float paccLen = accLen; // previous accLen
        accLen += len;
        if ((accLen - placedLen > ENGINE_PGE_199_DEFAULT_RESAMPLE_LENGTH) && (len > 1.0f))
        {
          int nSec =
            static_cast<int> (std::floor ((accLen - placedLen) / static_cast<float> (ENGINE_PGE_199_DEFAULT_RESAMPLE_LENGTH))); // number of section
          for (int j = 1; j <= nSec; j++)
          {
            olc::vf2d pos =
              lineBlend (nodes_[i].position_, nodes_[i - 1].position_, (placedLen - paccLen + j * ENGINE_PGE_199_DEFAULT_RESAMPLE_LENGTH) / len);
            node node_s (pos.x, pos.y);
            Nnodes.push_back (node_s);
          } // end FOR
          placedLen += nSec * ENGINE_PGE_199_DEFAULT_RESAMPLE_LENGTH;
        } // end IF
      } // end FOR
      Nnodes.pop_back ();
      Nnodes.push_back (lastNode);
      nodes_ = Nnodes;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      static node prev_s (0.0f, 0.0f);
      bool is_first_b = true;
      for (std::vector<node>::iterator iterator = nodes_.begin();
           iterator != nodes_.end();
           ++iterator)
      {
        if (is_first_b)
        {
          is_first_b = false;
          prev_s = *iterator;
        } // end IF
        engine_in->DrawLine (prev_s.position_, (*iterator).position_, olc::BLACK, 0xFFFFFFFF);
        prev_s = *iterator;
      } // end FOR
    }

    std::vector<node> nodes_;
  };

  rope rope_;
};

#endif // PGE_199_H
