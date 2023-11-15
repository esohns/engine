#include "stdafx.h"

#include "pge_235.h"

#include <vector>

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_235::PGE_235 ()
 : olc::PixelGameEngine ()
 , centerX_ (0)
 , dots_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 235");
}

bool
PGE_235::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  centerX_ = olc::PixelGameEngine::ScreenWidth () / 2;

  for (int i = 0; i < ENGINE_PGE_235_DEFAULT_NUMBER_OF_DOTS; i++)
  {
    dots_.push_back (dot ());
    dots_[i].setup (i);
  } // end FOR

  return true;
}

bool
PGE_235::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth() * olc::PixelGameEngine::GetDrawTargetHeight();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget()->GetData();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_235_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_235_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < static_cast<int> (dots_.size ()); i++)
  {
    int modIndex = mod (i - ENGINE_PGE_235_DEFAULT_MODULO, ENGINE_PGE_235_DEFAULT_MODULO);
    if (dots_[i].radius_ > dots_[modIndex].radius_)
    {
      dots_[i].amplitude_ = dots_[modIndex].amplitude_;
      dots_[i].size_ = dots_[modIndex].size_;
      dots_[i].col_ = dots_[modIndex].col_;
    } // end IF
    else if (Common_Tools::testRandomProbability (0.05f))
    {
      dots_[i].amplitude_ = Common_Tools::getRandomNumber (0.0f, 1.0f);
      dots_[i].size_ = Common_Tools::getRandomNumber (0.0f, 1.0f);
      static olc::Pixel colors_a[7] = {olc::Pixel (0x23, 0xE5, 0xCA), olc::Pixel (0x10, 0x0D, 0xE6), olc::Pixel (0xF9, 0x43, 0x25),
                                       olc::Pixel (0xDF, 0x62, 0x8A), olc::Pixel (0xE2, 0x65, 0x8B), olc::Pixel (0xF0, 0xBB, 0x73),
                                       olc::Pixel (0x45, 0x3C, 0xA5)};
      int index_i = Common_Tools::getRandomNumber (0, 6);
      dots_[i].col_ = colors_a[index_i];
    } // end ELSE IF

    dots_[i].update (centerX_, frame_count_i);
    dots_[i].draw (this);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

int
PGE_235::mod (int x, int m)
{
  return (x % m + m) % m;
}
