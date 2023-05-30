#include "stdafx.h"

#include "pge_22.h"

#include "MagickWand/MagickWand.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

//////////////////////////////////////////

PGE_22::PGE_22 ()
 : olc::PixelGameEngine ()
 , circles_ ()
 , spots_ ()
{
  sAppName = "Example 22";
}

bool
PGE_22::OnUserCreate ()
{
  Common_Image_Resolution_t resolution_s;
  uint8_t* data_p = NULL;

  if (!Common_Image_Tools::load (ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_22_DEFAULT_SPOTS_FILE),
                                 ACE_TEXT_ALWAYS_CHAR ("RGB"),
                                 resolution_s,
                                 data_p))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Common_Image_Tools::load (\"%s\"), aborting\n"),
                ACE_TEXT (ENGINE_PGE_22_DEFAULT_SPOTS_FILE)));
    return false;
  } // end IF
  ACE_ASSERT (data_p);
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  for (int y = 0; y < resolution_s.cy; ++y)
    for (int x = 0; x < resolution_s.cx; ++x)
    {
      unsigned int b = data_p[(y * resolution_s.cx + x) * 3];
      b += data_p[((y * resolution_s.cx + x) * 3) + 1];
      b += data_p[((y * resolution_s.cx + x) * 3) + 2];
      if (b)
        spots_.push_back ({x, y});
    } // end FOR
#else
  for (int y = 0; y < resolution_s.height; ++y)
    for (int x = 0; x < resolution_s.width; ++x)
    {
        unsigned int b = data_p[(y * resolution_s.width + x) * 3];
        b += data_p[((y * resolution_s.width + x) * 3) + 1];
        b += data_p[((y * resolution_s.width + x) * 3) + 2];
        if (b)
            spots_.push_back ({x, y});
    } // end FOR
#endif // ACE_WIN32 || ACE_WIN64
  MagickRelinquishMemory (data_p); data_p = NULL;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("loaded image \"%s\": found %u bright spot(s)\n"),
              ACE_TEXT (ENGINE_PGE_22_DEFAULT_SPOTS_FILE),
              spots_.size ()));

  return true;
}

bool
PGE_22::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  //for (int i = 0; i < spots_.size(); i++)
  //  olc::PixelGameEngine::Draw(spots_[i], olc::WHITE);

  int total = 5;
  int count = 0;
  int attempts = 0;

  while (count < total)
  {
    if (makeCircle ())
      count++;
    attempts++;
    if (attempts > 1000)
      break;
  } // end WHILE

  for (int i = 0; i < circles_.size (); i++)
  {
    if (circles_[i].growing_)
    {
      if (circles_[i].edges ())
        circles_[i].growing_ = false;
      else
        for (int j = 0; j < circles_.size (); j++)
          if (circles_[i].position_ != circles_[j].position_)
          {
            ACE_INT32 d =
              static_cast<ACE_INT32> (circles_[i].position_.dist (circles_[j].position_));
            ACE_INT32 distance = circles_[i].radius_ + circles_[j].radius_;
            if (d - 4 < distance)
            {
              circles_[i].growing_ = false;
              break;
            } // end IF
          } // end IF
    } // end IF

    circles_[i].show (this);

    if (circles_[i].growing_)
      circles_[i].grow ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

bool
PGE_22::makeCircle ()
{
  int index = Common_Tools::getRandomNumber (0, static_cast<int> (spots_.size () - 1));

  bool is_valid_b = true;
  for (int i = 0; i < circles_.size (); i++)
  {
    ACE_INT32 d = static_cast<ACE_INT32> (circles_[i].position_.dist (spots_[index]));
    if (d < circles_[i].radius_)
    {
      is_valid_b = false;
      break;
    } // end IF
  } // end FOR
  if (is_valid_b)
  {
    circles_.push_back (circle (spots_[index]));
    return true;
  } // end IF

  return false;
}
