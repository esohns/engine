#ifndef PGE_409_H
#define PGE_409_H

#include "olcPixelGameEngine.h"

class PGE_409
 : public olc::PixelGameEngine
{
 public:
  PGE_409 ();
  inline virtual ~PGE_409 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void setting ();
  void hen ();
  void kan ();

  float Zx[4];
  float Wx[4];
  float Zy[4];
  float Wy[4];
  olc::vf2d NAKA;
  olc::vf2d HOPPY;
  olc::vf2d A;
  olc::vf2d B;
  olc::vf2d C;
  olc::vf2d D_2;
  olc::vf2d E;
  olc::vf2d F;
  olc::vf2d LU;
  olc::vf2d RU;
  olc::vf2d LD;
  olc::vf2d RD;
  olc::vf2d K;

  float CX[4];
  float CY[4];
  float R[4];
  float D[4];
  int I;
  int II;
  int P;
  int Q;
  float X;
  float Y;
};

#endif // PGE_409_H
  