#ifndef PGE_108_H
#define PGE_108_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "defines_4.h"

class PGE_108
 : public olc::PixelGameEngine
{
 public:
  PGE_108 ();
  inline virtual ~PGE_108 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float X_[ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1];
  float Y_[ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1];
  float Z_[ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1];
  float V_[ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1];
  float dV_[ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1];
  float L_;
  float R_;
  float Lmin_;
  int32_t N_;
  int32_t NN_;
  float KX_;
  float KY_;
  float KZ_;
  float KV_;
  float KdV_;
  int32_t K_;
};

#endif // PGE_108_H
