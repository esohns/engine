#include "stdafx.h"

#include "pge_207.h"

#if defined (IMAGEMAGICK_SUPPORT)
#if defined (IMAGEMAGICK_IS_GRAPHICSMAGICK)
#include "wand/wand_api.h"
#else
#include "MagickWand/MagickWand.h"
#endif // IMAGEMAGICK_IS_GRAPHICSMAGICK
#endif // IMAGEMAGICK_SUPPORT

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_207::PGE_207 ()
 : olc::PixelGameEngine ()
//#if defined (FFMPEG_SUPPORT)
//  , imageData_ ()
//#endif // FFMPEG_SUPPORT
 , imageData_ (NULL)
{
//#if defined (FFMPEG_SUPPORT)
//  ACE_OS::memset (imageData_, 0, sizeof (uint8_t*) * AV_NUM_DATA_POINTERS);
//#endif // FFMPEG_SUPPORT

  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 207");
}

PGE_207::~PGE_207 ()
{
#if defined (IMAGEMAGICK_SUPPORT)
  MagickRelinquishMemory (imageData_);
#endif // IMAGEMAGICK_SUPPORT
//#if defined (FFMPEG_SUPPORT)
  //delete [] imageData_[0];
//#endif // FFMPEG_SUPPORT
}

bool
PGE_207::OnUserCreate ()
{
  Common_Image_Resolution_t resolution_s;
#if defined (IMAGEMAGICK_SUPPORT)
  if (!Common_Image_Tools::load (ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_207_DEFAULT_IMAGE_FILE),
                                 ACE_TEXT_ALWAYS_CHAR ("RGBA"),
                                 resolution_s,
                                 imageData_))
#endif // IMAGEMAGICK_SUPPORT
//#if defined (FFMPEG_SUPPORT)
  //enum AVPixelFormat pixel_format_e;
  //if (!Common_Image_Tools::load (ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_207_DEFAULT_IMAGE_FILE),
  //                               resolution_s,
  //                               pixel_format_e,
  //                               imageData_))
//#endif // FFMPEG_SUPPORT
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to load image (was: \"%s\"), aborting\n"),
                ACE_TEXT (ENGINE_PGE_207_DEFAULT_IMAGE_FILE)));
    return false;
  } // end IF

  return true;
}

bool
PGE_207::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  static float hDivisionSize =
    olc::PixelGameEngine::ScreenHeight () / static_cast<float> (ENGINE_PGE_207_DEFAULT_H_DIVISIONS);
  static float max_amplitude_f = hDivisionSize / 2.0f;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float frequency_f =
    Common_GL_Tools::map (std::sin (frame_count_i / 60.0f), -1.0f, 1.0f, 20.0f, 200.0f);
  float phase_f = frame_count_i / 2.0f;
  for (int32_t hDiv = 0; hDiv < ENGINE_PGE_207_DEFAULT_H_DIVISIONS; hDiv++)
  {
    float y = (hDivisionSize / 2.0f) + hDiv * hDivisionSize;
    olc::vf2d prev_s (-1.0f, y);
    for (int32_t x = 0; x < ENGINE_PGE_207_DEFAULT_WIDTH; x++)
    {
      float angle_f =
        Common_GL_Tools::map (static_cast<float> (x), 0.0f, static_cast<float> (ENGINE_PGE_207_DEFAULT_WIDTH), 0.0f, static_cast<float> (M_PI) * 2.0f);
      float sin_value_f = std::sin (phase_f + angle_f * frequency_f);

      int32_t index_i = static_cast<int32_t> (std::floor (y)) * ENGINE_PGE_207_DEFAULT_WIDTH + x;
//#if defined (FFMPEG_SUPPORT)
      //float value_f = static_cast<float> (imageData_[0][index_i * 4 + 0] + // r
      //                                    imageData_[0][index_i * 4 + 1] + // g
      //                                    imageData_[0][index_i * 4 + 2]); // b
//#endif // FFMPEG_SUPPORT
      float value_f = static_cast<float> (imageData_[index_i * 4 + 0] + // r
                                          imageData_[index_i * 4 + 1] + // g
                                          imageData_[index_i * 4 + 2]); // b
      value_f /= 3.0f;
      float amplitude_f =
        Common_GL_Tools::map (value_f, 0.0f, 255.0f, max_amplitude_f, 0.0f);
      olc::vf2d point_s (static_cast<float> (x), y + sin_value_f * amplitude_f);
      olc::PixelGameEngine::DrawLine (prev_s, point_s, olc::WHITE, 0xFFFFFFFF);
      prev_s = point_s;
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
