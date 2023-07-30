#include "stdafx.h"

#include "pge_71.h"

#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_71::PGE_71 ()
 : olc::PixelGameEngine ()
 , halfWidthHeight_ (0.0f)
 , f_ (0.0f)
 , image_ (NULL)
{
  sAppName = "Example 71";
}

PGE_71::~PGE_71 ()
{
  delete [] image_;
}

bool
PGE_71::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  halfWidthHeight_ = olc::PixelGameEngine::ScreenWidth () / 2.0f;

  image_ = new olc::Pixel[olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ()];
  ACE_OS::memset (image_, 0, sizeof (olc::Pixel) * olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ());

  return true;
}

bool
PGE_71::OnUserUpdate (float fElapsedTime)
{
  int pixels = olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_71_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_71_DEFAULT_ALPHA_DECAY : 0);

  static int frame_counter_i = 0;

  put (static_cast<int32_t> (std::sin (f_) * 2.0f), static_cast<int32_t> (std::cos (f_) * 2.0f));

  uint8_t color_i = 255;
  for (float i = 0.0f; i < static_cast<float> (M_PI) * 2.0f; i += static_cast<float> (M_PI) / 512.0f)
  {
    color_i =
      static_cast<uint8_t> (halfWidthHeight_ + std::cos (-f_ * 2.0f + i * 6.0f) * 30.0f);
    for (int n = 0; n < 320; n += 80)
    {
      int Q = (frame_counter_i % 320 + n) % 320;
      olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (std::sin (i) * Q + halfWidthHeight_),
                                        static_cast<int32_t> (std::cos (i) * Q + halfWidthHeight_),
                                        ENGINE_PGE_71_DEFAULT_RADIUS,
                                        {color_i, color_i, color_i, 255}, 0xFF);
    } // end FOR
  } // end FOR
  olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (halfWidthHeight_),
                                    static_cast<int32_t> (halfWidthHeight_),
                                    ENGINE_PGE_71_DEFAULT_RADIUS,
                                    {color_i, color_i, color_i, 255},
                                    0xFF);

  get ();
  f_ += 0.05f;
  ++frame_counter_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_71::get ()
{
  // sanity check(s)
  ACE_ASSERT (image_);

  ACE_OS::memcpy (image_, olc::PixelGameEngine::GetDrawTarget ()->GetData (), sizeof (olc::Pixel) * olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ());

  // tint
  static float amount_f = ENGINE_PGE_71_DEFAULT_TINT_GREY / 255.0f;
  for (int32_t y = 0; y < olc::PixelGameEngine::ScreenHeight(); ++y)
    for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth(); ++x)
    {
      olc::Pixel* p = &image_[y * olc::PixelGameEngine::ScreenWidth() + x];
      if (p->r || p->g || p->b)
      {
        p->r = static_cast<uint8_t> (p->r * amount_f);
        p->g = static_cast<uint8_t> (p->g * amount_f);
        p->b = static_cast<uint8_t> (p->b * amount_f);
      } // end IF
    } // end FOR
}

void
PGE_71::put (int32_t offsetX_in, int32_t offsetY_in)
{
  // sanity check(s)
  ACE_ASSERT (image_);

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int32_t y = offsetY_in; y < olc::PixelGameEngine::ScreenHeight (); ++y)
    for (int32_t x = offsetX_in; x < olc::PixelGameEngine::ScreenWidth (); ++x)
    {
      if ((x < 0) || (y < 0)) continue;
      p[y * olc::PixelGameEngine::ScreenWidth () + x] =
        image_[(y - offsetY_in) * olc::PixelGameEngine::ScreenWidth () + (x - offsetX_in)];
    } // end FOR
}
