#include "stdafx.h"

#include "pge_89.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_89::PGE_89 ()
 : olc::PixelGameEngine ()
 , b1_ (ENGINE_PGE_89_DEFAULT_WIDTH, ENGINE_PGE_89_DEFAULT_HEIGHT)
 , b2_ (ENGINE_PGE_89_DEFAULT_WIDTH, ENGINE_PGE_89_DEFAULT_HEIGHT)
 , yPos1_ (0)
 , yPos2_ (ENGINE_PGE_89_DEFAULT_HEIGHT)
 , isFirst_ (true)
 , palette_ ()
{
  sAppName = "Example 89";
}

bool
PGE_89::OnUserCreate ()
{
  for (int i = 1; i <= 30; i++)
    palette_[i] = {255, static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)), 255, 255};
  palette_[15] = palette_[16] = {0x0A, 0xED, 0xFF, 255};

  return true;
}

bool
PGE_89::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);

  static int frame_count_i = 1;

  if (isFirst_)
  {
    olc::PixelGameEngine::SetDrawTarget (&b1_);
    drawRows (28, 0);
    olc::PixelGameEngine::SetDrawTarget (&b2_);
    drawRows (28, 1);
    isFirst_ = false;
  } // end IF

  --yPos1_;
  --yPos2_;

  olc::PixelGameEngine::SetDrawTarget (nullptr);
  olc::PixelGameEngine::DrawSprite (0, yPos1_, &b1_, 1U, 0U);
  olc::PixelGameEngine::DrawSprite (0, yPos2_, &b2_, 1U, 0U);

  if (yPos1_ <= -ENGINE_PGE_89_DEFAULT_HEIGHT)
  {
    olc::PixelGameEngine::SetDrawTarget (&b1_);
    olc::PixelGameEngine::Clear (olc::BLACK);

    drawRows (28, frame_count_i);
    yPos1_ = ENGINE_PGE_89_DEFAULT_HEIGHT;
  } // end IF
  if (yPos2_ <= -ENGINE_PGE_89_DEFAULT_HEIGHT)
  {
    olc::PixelGameEngine::SetDrawTarget (&b2_);
    olc::PixelGameEngine::Clear (olc::BLACK);

    drawRows (28, frame_count_i);
    yPos2_ = ENGINE_PGE_89_DEFAULT_HEIGHT;
  } // end IF

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_89::drawRows (int32_t numRows_in, int frameCount_in)
{
  for (int i = 0; i < numRows_in; i++)
    drawRow (i, frameCount_in);
}

void
PGE_89::drawRow (int32_t numRow_in, int frameCount_in)
{
  numRow_in *= 20;
  int colNum = 1;
  while (colNum <= 30)
  {
    if ((Common_Tools::getRandomNumber (0.0f, 1.0f) < 0.5f) || (frameCount_in == 0))
      drawVertical (colNum * 20, numRow_in, palette_[colNum]);
    else
    {
      bool which = Common_Tools::getRandomNumber (0.0f, 1.0f) < 0.5f;
      if (colNum != 30)
      {
        drawCrossing (colNum * 20, numRow_in, which, palette_[colNum], palette_[colNum + 1]);
        olc::Pixel hold_s = palette_[colNum];
        palette_[colNum] = palette_[colNum + 1];
        palette_[colNum + 1] = hold_s;
      } // end IF
      if (colNum == 30)
        drawVertical (colNum * 20, numRow_in, palette_[colNum]);
      colNum++;
    } // end ELSE
    colNum++;
  } // end WHILE
}

void
PGE_89::drawVertical (int32_t x, int32_t y, olc::Pixel& c)
{
  olc::PixelGameEngine::DrawLine (x, y, x, y + 20, c, 0xFFFFFFFF);
}

void
PGE_89::drawCrossing (int32_t x, int32_t y, bool positive, olc::Pixel& c1, olc::Pixel& c2)
{
  if (positive)
  {
    olc::PixelGameEngine::DrawLine (x, y, x + 20, y + 20, c1, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x + 20, y, x + 15, y + 5, c2, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x + 5, y + 15, x, y + 20, c2, 0xFFFFFFFF);
  } // end IF
  else
  {
    olc::PixelGameEngine::DrawLine (x + 20, y, x, y + 20, c2, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x, y, x + 5, y + 5, c1, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x + 15, y + 15, x + 20, y + 20, c1, 0xFFFFFFFF);
  } // end ELSE
}
