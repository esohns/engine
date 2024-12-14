#ifndef PGE_435_H
#define PGE_435_H

#include <chrono>
#include <vector>

#include "olcPixelGameEngine.h"

class PGE_435
 : public olc::PixelGameEngine
{
 public:
  PGE_435 ();
  virtual ~PGE_435 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  bool resize (int32_t, int32_t);
  void disturb (int32_t, int32_t);

  olc::Sprite*            sprite_;
  std::vector<int32_t>    rippleMap_;
  std::vector<int32_t>    lastMap_;
  int32_t                 oldIndex_;
  int32_t                 newIndex_;
  std::vector<olc::Pixel> rippleData_;

#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point lastDrop_;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> lastDrop_;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
};

#endif // PGE_435_H
