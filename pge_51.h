#ifndef PGE_51_H
#define PGE_51_H

#include "olcPixelGameEngine.h"

class PGE_51
 : public olc::PixelGameEngine
{
 public:
  PGE_51 ();
  inline virtual ~PGE_51 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  typedef std::vector<olc::vf2d> path_t;
  path_t path_;

  std::vector<olc::vf2d> circlePoints ();
  std::vector<olc::vf2d> complexifyPath (std::vector<olc::vf2d>&);
};

#endif // PGE_51_H
