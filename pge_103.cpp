#include "stdafx.h"

#include "pge_103.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_103::PGE_103 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 103";
}

bool
PGE_103::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_103::OnUserUpdate (float fElapsedTime)
{
  static bool done_b = false;
  if (done_b) goto end;

  tile ();
  done_b = true;

end:
  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_103::tile ()
{
  int32_t c = Common_Tools::getRandomNumber (30, 80);
  float w = olc::PixelGameEngine::ScreenWidth () / static_cast<float> (c);

  static olc::Pixel palette_a[5] =
    {{0x08, 0x67, 0x88, 255}, {0x07, 0xa0, 0xc3, 255}, {0xf0, 0xc8, 0x08, 255}, {0xdd, 0xdd, 0xdd, 255}, {0xdd, 0x1c, 0x1a, 255}};
  std::vector<olc::Pixel> col1, col2;
  for (int32_t i = 0; i < c; i++)
  {
    int j = Common_Tools::getRandomNumber (0, 4);
    col1.push_back (palette_a[j]);
    j = Common_Tools::getRandomNumber (0, 4);
    col2.push_back (palette_a[j]);
  } // end FOR

  for (int32_t i = 0; i < c; i++)
  {
    int32_t t = i % 2;
    for (int32_t j = 0; j < c; j++)
    {
      int32_t x = static_cast<int32_t> (i * w + w / 2.0f);
      int32_t y = static_cast<int32_t> (j * w + w / 2.0f);
      t = (t == 0) ? 1 : 0;
      form (x, y, w + 1, t, col1[i], col2[j]);
    } // end FOR
  } // end FOR
}

void
PGE_103::form (int32_t x, int32_t y, float s, int32_t t, olc::Pixel& col1, olc::Pixel& col2)
{
  float w = s * 0.7f;
  float h = s;
  if (t == 0)
  {
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - w / 2.0f), static_cast<int32_t> (y - h / 2.0f), static_cast<int32_t> (w), static_cast<int32_t> (h), col1);
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - w / 2.0f), static_cast<int32_t> (y - h / 2.0f), static_cast<int32_t> (w), static_cast<int32_t> (w * 1.1f), {0, 0, 0, 100});
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - w / 2.0f), static_cast<int32_t> (y - h / 2.0f), static_cast<int32_t> (h), static_cast<int32_t> (w), col2);
  } // end IF
  else
  {
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - w / 2.0f), static_cast<int32_t> (y - h / 2.0f), static_cast<int32_t> (h), static_cast<int32_t> (w), col2);
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - w / 2.0f), static_cast<int32_t> (y - h / 2.0f), static_cast<int32_t> (w * 1.1f), static_cast<int32_t> (w), {0, 0, 0, 100});
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - w / 2.0f), static_cast<int32_t> (y - h / 2.0f), static_cast<int32_t> (w), static_cast<int32_t> (h), col1);
  } // end ELSE
}
