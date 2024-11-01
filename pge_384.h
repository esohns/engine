#ifndef PGE_384_H
#define PGE_384_H

#include <utility>
#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_384
 : public olc::PixelGameEngine
{
 public:
  PGE_384 ();
  inline virtual ~PGE_384 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct parameters
  {
    parameters ()
     : mu_k (4.0f)
     , sigma_k (1.0f)
     , w_k (0.022f)
     , mu_g (0.6f)
     , sigma_g (0.15f)
     , c_rep (1.0f)
     , dt (0.1f)
    {}

    float mu_k;
    float sigma_k;
    float w_k;
    float mu_g;
    float sigma_g;
    float c_rep;
    float dt;
  };

  struct fields
  {
    fields ()
     : R_val (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS)
     , U_val (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS)
     , R_grad (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS * 2)
     , U_grad (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS * 2)
    {}

    std::vector<float> R_val;
    std::vector<float> U_val;
    std::vector<float> R_grad;
    std::vector<float> U_grad;
  };

  std::vector<float> initialize (std::vector<float>&);
  void add_xy (std::vector<float>&, int, float, float, float);
  void compute_fields ();
  std::pair<float, float> repulsion_f (float, float);
  float fast_exp (float);
  std::pair<float, float> peak_f (float, float, float, float = 1.0f);
  float step ();

  struct parameters  parameters_;
  std::vector<float> points_;
  struct fields      fields_;
  int                stepsPerFrame_;
  float              randomColor_;
};

#endif // PGE_384_H
