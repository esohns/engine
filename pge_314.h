#ifndef PGE_314_H
#define PGE_314_H

#include "olcPixelGameEngine.h"

class PGE_314
 : public olc::PixelGameEngine
{
 public:
  PGE_314 ();
  inline virtual ~PGE_314 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int   X_;
  int   Y_;
  float KX_;
  float KY_;
  int   S_;
  int   I_;
  float H_;
  float R_;
  float A_;
  int   Q_;
  int   QMax_;
  float T_;
  float AE_;
  int   KurosawaMode_;
};

#endif // PGE_314_H
