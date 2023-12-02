#ifndef PGE_12_H
#define PGE_12_H

#include <complex>
#include <vector>

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_12
 : public olc::PixelGameEngine
{
 public:
  PGE_12 ();
  ~PGE_12 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct t_fourier
  {
    std::complex<float> com {0.0f, 0.0f};
    size_t nFreq = 0;
    float fAmp = 0.0f;
    float fPhase = 0.0f;
  };

  void getSignalData (const std::string&);
  std::vector<struct t_fourier> computeDFT (const std::vector<std::complex<float> >&);
  void createDFTFromPicture ();
  olc::vf2d drawEpicycle (olc::vf2d, float, const std::vector<struct t_fourier>&);

  float fDelta = 0.0f;  //change in time
  float fTick = 0.0f;
  int nPic = 0; //initial picture
  std::string sPictures[5] = {"peace.dat","plant.dat","hydra.dat","fouriertransform.dat","note.dat"};

  olc::vf2d y_circle_orig;  //initial circle origin
  olc::vf2d x_circle_orig;  //initial circle origin
  std::vector<olc::vf2d> vPreviousPath; //store previously drawn path
  std::vector<olc::vf2d> vCurrentPath; //store active path

  std::vector<struct t_fourier> vFourierX;    //Fourier X
  std::vector<std::complex<float> > vSignalX; //Series X
  std::vector<struct t_fourier> vFourierY;    //Fourier Y
  std::vector<std::complex<float> > vSignalY; //Series Y
};

#endif // PGE_12_H
