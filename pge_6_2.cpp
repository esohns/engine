#include "stdafx.h"

#include "pge_6_2.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_6_2::PGE_6_2 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 6.2";
}

PGE_6_2::~PGE_6_2 ()
{
  delete [] buffer_1;
  delete [] buffer_2;
  delete [] cooling_buffer;
}

void
PGE_6_2::cool ()
{
  double value_d = 0.0;
  double x_offset_d = 0.0;

  for (int x = 0; x < ScreenWidth (); ++x)
  {
    x_offset_d += step;
    double y_offset_d = y;
    for (int y = 0; y < ScreenHeight (); ++y)
    {
      y_offset_d += step;
      value_d = (module.GetValue (x_offset_d, y_offset_d, 0.0) + 1.0) / 2.0;
      double brightness_d = pow (value_d, 3.0) * 255.0;
      cooling_buffer[y * ScreenWidth() + x] = (ACE_UINT8)brightness_d;
    } // end FOR
  } // end FOR
  y += step;
}

void
PGE_6_2::convect ()
{
  ACE_OS::memcpy (buffer_1, buffer_1 + ScreenWidth (), (ScreenHeight () - 1) * ScreenWidth () * sizeof (ACE_UINT8));
}

void
PGE_6_2::fire ()
{
  for (int x = 0; x < ScreenWidth (); ++x)
    for (int j = 0; j < ENGINE_PGE_6_2_DEFAULT_NUMBER_OF_FIRE_LINES; ++j)
    {
      int y = ScreenHeight() - (j + 1);
      buffer_1[y * ScreenWidth() + x] = 0xFF;
    } // end FOR
}

void
PGE_6_2::blur (int offset_in)
{
  ACE_UINT32 rgb_combined_i = 0;

  //     [M]
  // [L]     [R]
  //     [M]
  // middle column
  rgb_combined_i += getColor (offset_in - ScreenWidth ()); // x, y - 1
  rgb_combined_i += getColor (offset_in + ScreenWidth ()); // x, y + 1
  // left column
  rgb_combined_i += getColor (offset_in - 1);              // x - 1, y
  // right column
  rgb_combined_i += getColor (offset_in + 1);              // x + 1, y
  // average the colors
  rgb_combined_i = rgb_combined_i / 4; // divide by 4

  buffer_2[offset_in] = (ACE_UINT8)rgb_combined_i;
}

ACE_UINT8
PGE_6_2::getColor (int offset_in)
{
  if ((offset_in < 0) || (offset_in >= ScreenHeight () * ScreenWidth ()))
    return 0;

  return buffer_1[offset_in];
}

bool
PGE_6_2::OnUserCreate ()
{
  srand (time (NULL));

  buffer_1 = new ACE_UINT8[ScreenHeight () * ScreenWidth ()];
  ACE_OS::memset (buffer_1, 0, sizeof (ACE_UINT8) * ScreenHeight () * ScreenWidth ());
  buffer_2 = new ACE_UINT8[ScreenHeight () * ScreenWidth ()];
  ACE_OS::memset (buffer_2, 0, sizeof (ACE_UINT8) * ScreenHeight () * ScreenWidth ());
  cooling_buffer = new ACE_UINT8[ScreenHeight() * ScreenWidth()];
  ACE_OS::memset (cooling_buffer, 0, sizeof(ACE_UINT8) * ScreenHeight() * ScreenWidth());

  module.SetNoiseQuality (noise::QUALITY_FAST);

  return true;
}

bool
PGE_6_2::OnUserUpdate (float fElapsedTime)
{
  fire ();
  for (int y = 0; y < ScreenHeight (); ++y)
    for (int x = 0; x < ScreenWidth (); ++x)
      blur (y * ScreenWidth () + x);
  cool ();
  for (int y = 0; y < ScreenHeight (); ++y)
    for (int x = 0; x < ScreenWidth (); ++x)
    {
      ACE_UINT8 value_i = buffer_2[y * ScreenWidth() + x];
      ACE_UINT8 cooling_value_i = cooling_buffer[y * ScreenWidth() + x];
      buffer_2[y * ScreenWidth () + x] =
        (value_i >= cooling_value_i ? value_i - cooling_value_i : 0);
    } // end FOR

  for (int y = 0; y < ScreenHeight (); y++)
    for (int x = 0; x < ScreenWidth (); x++)
      Draw (x, y, olc::Pixel (buffer_2[y * ScreenWidth () + x],
                              buffer_2[y * ScreenWidth () + x],
                              buffer_2[y * ScreenWidth () + x],
                              255));

  ACE_UINT8* buffer_temp = buffer_2;
  buffer_2 = buffer_1;
  buffer_1 = buffer_temp;

  convect ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
