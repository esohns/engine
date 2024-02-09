#ifndef PGE_353_H
#define PGE_353_H

#include <complex>
#include <vector>

#include "olcPixelGameEngine.h"

class PGE_353
 : public olc::PixelGameEngine
{
 public:
  PGE_353 ();
  inline virtual ~PGE_353 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void createInput ();
  void initOutput ();
  std::complex<float> f (std::complex<float>&, float);
  void computeOutput (float);
  olc::Pixel complexToColor (std::complex<float>&);
  void updateScreen ();

  std::vector<std::vector<std::complex<float> > > input_;
  std::vector<std::vector<std::complex<float> > > output_;
  bool                                            showIdentity_;
  float                                           t_;
  float                                           xFactor_;
  float                                           yFactor_;
};

#endif // PGE_353_H
