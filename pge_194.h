#ifndef PGE_194_H
#define PGE_194_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_8.h"

class PGE_194
 : public olc::PixelGameEngine
{
 public:
  PGE_194 ();
  virtual ~PGE_194 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct agent
  {
    olc::vf2d position;
    float     heading;
    int       last_option;
  };

  std::vector<agent> agents_;
  bool               highlightAgents_;
  std::vector<float> weights_;
  float*             trail_;

  inline int index (int x, int y) { return x + y * ENGINE_PGE_194_DEFAULT_W; }

  float sense_relative_angle (struct agent&, float);
  void step_sense_and_rotate ();
  void step_move ();
  void step_deposit ();
  void step_diffuse_and_decay ();
  void sim_step ();
  void render ();
  void regenerate (float, float);
  void resetSimulation (float, float);
};

#endif // PGE_194_H
