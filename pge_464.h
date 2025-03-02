#ifndef PGE_464_H
#define PGE_464_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_464
 : public olc::PixelGameEngine
{
 public:
  PGE_464 ();
  inline virtual ~PGE_464 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void initializeHairs ();
  void moveHairs (olc::vf2d&);

  class spine
  {
   public:
    spine ()
     : n (0)
     , separation (0.0f)
     , bendMax (0.0f)
     , nodes ()
    {}

    spine (int n_in, float separation_in, float bendMax_in)
     : n (n_in)
     , separation (separation_in)
     , bendMax (bendMax_in)
     , nodes ()
    {
      for (int i = 0; i < n_in; i++)
        nodes.push_back (olc::vf2d (i * separation_in, 0.0f));
    }

    void relaxRight (int i, olc::vf2d& u)
    {
      if (i + 1 < n)
      {
        olc::vf2d v = nodes[i + 1] - nodes[i];
        float dot = u.dot (v);
        float det = u.x * v.y - u.y * v.x;
        float ang = std::atan2 (det, dot);
        if (ang < -bendMax)
        {
          float x_rotated_f = v.x * std::cos (-ang - bendMax) - v.y * std::sin (-ang - bendMax);
          float y_rotated_f = v.x * std::sin (-ang - bendMax) + v.y * std::cos (-ang - bendMax);
          v = { x_rotated_f, y_rotated_f };
        } // end IF
        else if (ang > bendMax)
        {
          float x_rotated_f = v.x * std::cos (-ang + bendMax) - v.y * std::sin (-ang + bendMax);
          float y_rotated_f = v.x * std::sin (-ang + bendMax) + v.y * std::cos (-ang + bendMax);
          v = { x_rotated_f, y_rotated_f };
        } // end ELSE IF
        v = v.norm (); v *= separation;
        nodes[i + 1] = nodes[i] + v;
        relaxRight (i + 1, v);
      } // end IF
    }

    void relaxLeft (int i, olc::vf2d& u)
    {
      if (i > 0)
      {
        olc::vf2d v = nodes[i - 1] - nodes[i];
        float dot = u.dot (v);
        float det = u.x * v.y - u.y * v.x;
        float ang = std::atan2 (det, dot);
        if (ang < -bendMax)
        {
          float x_rotated_f = v.x * std::cos (-ang - bendMax) - v.y * std::sin (-ang - bendMax);
          float y_rotated_f = v.x * std::sin (-ang - bendMax) + v.y * std::cos (-ang - bendMax);
          v = { x_rotated_f, y_rotated_f };
        } // end IF
        else if (ang > bendMax)
        {
          float x_rotated_f = v.x * std::cos (-ang + bendMax) - v.y * std::sin (-ang + bendMax);
          float y_rotated_f = v.x * std::sin (-ang + bendMax) + v.y * std::cos (-ang + bendMax);
          v = { x_rotated_f, y_rotated_f };
        } // end ELSE IF
        v = v.norm (); v *= separation;
        nodes[i - 1] = nodes[i] + v;
        relaxLeft (i - 1, v);
      } // end IF
    }

    void setNode (int i, olc::vf2d& p)
    {
      olc::vf2d u;
      if (i > 0)
      {
        if (i + 1 < n)
          u = nodes[i + 1] - nodes[i - 1];
        else
          u = nodes[i] - nodes[i - 1];
      } // end IF
      else
        u = nodes[i + 1] - nodes[i];
      nodes[i] = p;
      relaxRight (i,  u);
      //relaxLeft  (i, -u);
    }

    void show (olc::PixelGameEngine* engine)
    {
      for (int i = 0; i < n; i++)
      {
        olc::vf2d p = nodes[i];
        if (i > 0)
        {
          olc::vf2d q = nodes[i - 1];
          engine->DrawLine (p, q, olc::BLACK, 0xFFFFFFFF);
        } // end IF
      } // end FOR
    }

    int                    n;
    float                  separation;
    float                  bendMax;
    std::vector<olc::vf2d> nodes;
  };

  struct hair
  {
    olc::vf2d      p;
    olc::vf2d      q;
    PGE_464::spine spine;
  };

  void drawArrow (olc::vf2d&, olc::vf2d&, float);

  noise::module::Perlin    noise_;
  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1_;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1_;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  float                    D_;
  std::vector<struct hair> hairs_;
};

#endif // PGE_464_H
