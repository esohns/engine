#ifndef PGE_207_H
#define PGE_207_H

#if defined (FFMPEG_SUPPORT)
#ifdef __cplusplus
extern "C"
{
#include "libavutil/frame.h"
}
#endif /* __cplusplus */
#endif // FFMPEG_SUPPORT

#include "olcPixelGameEngine.h"

class PGE_207
 : public olc::PixelGameEngine
{
 public:
  PGE_207 ();
  virtual ~PGE_207 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
//#if defined (FFMPEG_SUPPORT)
//   uint8_t* imageData_[AV_NUM_DATA_POINTERS];
//#elif defined (IMAGEMAGICK_SUPPORT)
#if defined (IMAGEMAGICK_SUPPORT)
  uint8_t* imageData_;
#endif // FFMPEG_SUPPORT || IMAGEMAGICK_SUPPORT
};

#endif // PGE_207_H
