#ifndef PGE_66_H
#define PGE_66_H

#include <vector>

#include "glm/glm.hpp"

#include "olcPixelGameEngine.h"

class PGE_66
 : public olc::PixelGameEngine
{
 public:
  PGE_66 ();
  inline virtual ~PGE_66 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<glm::vec3> points_;
};

#endif // PGE_66_H
