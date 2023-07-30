#ifndef PGE_71_H
#define PGE_71_H

#include "olcPixelGameEngine.h"

class PGE_71
 : public olc::PixelGameEngine
{
 public:
  PGE_71 ();
  virtual ~PGE_71 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float       halfWidthHeight_;
  float       f_;
  olc::Pixel* image_;

  void get ();
  void put (int32_t, int32_t);
};

#endif // PGE_71_H
