#ifndef PGE_206_H
#define PGE_206_H

#include "olcPixelGameEngine.h"

#include "defines_9.h"

class PGE_206
 : public olc::PixelGameEngine
{
 public:
  PGE_206 ();
  inline virtual ~PGE_206 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float CX_[ENGINE_PGE_206_DEFAULT_NMAX];
  float CY_[ENGINE_PGE_206_DEFAULT_NMAX];
  int   LR_[ENGINE_PGE_206_DEFAULT_NMAX];
  int   BN_[ENGINE_PGE_206_DEFAULT_NMAX];
  float Rmax_[ENGINE_PGE_206_DEFAULT_NMAX];
  float Aone_[ENGINE_PGE_206_DEFAULT_NMAX];
  float Azero_[ENGINE_PGE_206_DEFAULT_NMAX];
  float R_;
  int   I_;
  int   II_;
  float L_;
  int   OKNG_;
  float NX_;
  float NY_;
  float NRmax_;
  float RND_;
  int   NN_;
  float KY_;
  float KX_;
  float KA_;
  float DX_;
  float DY_;
};

#endif // PGE_206_H
