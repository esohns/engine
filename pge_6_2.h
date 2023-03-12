#ifndef PGE_6_2_H
#define PGE_6_2_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_6_2
 : public olc::PixelGameEngine
{
 public:
  PGE_6_2 ();
  ~PGE_6_2 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void cool ();
  void convect ();
  void fire ();
  ACE_UINT8 getColor (int); // offset
  void blur (int); // offset

#define RGB_R(x) uint8_t(x >> 24)
#define RGB_G(x) uint8_t((x & 0xFFFFFF) >> 16)
#define RGB_B(x) uint8_t((x & 0xFFFF) >> 8)
#define RGB_A(x) uint8_t(x & 0xFF)
#define BRIGHTNESS(x) ((RGB_R(x) + RGB_G (x) + RGB_B (x)) / 3);

  ACE_UINT8* buffer_1 = NULL;
  ACE_UINT8* buffer_2 = NULL;
  ACE_UINT8* cooling_buffer = NULL;

  // noise
  double                y = 0.0;
  double                step = 0.02;
  noise::module::Perlin module;
};

#endif // PGE_6_2_H
