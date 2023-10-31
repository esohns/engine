#include "stdafx.h"

#include "pge_219.h"

#include "ace/Log_Msg.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_219::PGE_219 ()
 : olc::PixelGameEngine ()
 , curves_ (NULL)
 , columns_ (0)
 , rows_ (0)
 , angle_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 219");
}

PGE_219::~PGE_219 ()
{
  for (int y = 0; y < rows_; y++)
    for (int x = 0; x < columns_; x++)
      delete curves_[y * columns_ + x];
  delete [] curves_;
}

bool
PGE_219::OnUserCreate ()
{
  columns_ =
    static_cast<int> (std::floor (olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_219_DEFAULT_W)) - 1;
  rows_ =
    static_cast<int> (std::floor (olc::PixelGameEngine::ScreenHeight () / ENGINE_PGE_219_DEFAULT_W)) - 1;
  curves_ = new curve*[columns_ * rows_];
  for (int y = 0; y < rows_; y++)
    for (int x = 0; x < columns_; x++)
      curves_[y * columns_ + x] = new curve ();

  return true;
}

bool
PGE_219::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  float d = ENGINE_PGE_219_DEFAULT_W - 0.2f * ENGINE_PGE_219_DEFAULT_W;
  float r = d / 2.0f;

  for (int i = 0; i < columns_; i++)
  {
    int32_t cx = ENGINE_PGE_219_DEFAULT_W + i * ENGINE_PGE_219_DEFAULT_W + ENGINE_PGE_219_DEFAULT_W / 2;
    int32_t cy = ENGINE_PGE_219_DEFAULT_W / 2;
    olc::PixelGameEngine::DrawCircle (cx, cy, static_cast<int32_t> (r), olc::WHITE);

    float x = r * std::cos (angle_ * (i + 1) - static_cast<float> (M_PI_2));
    float y = r * std::sin (angle_ * (i + 1) - static_cast<float> (M_PI_2));
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (cx + x), static_cast<int32_t> (cy + y), 4, olc::WHITE);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (cx + x), 0, static_cast<int32_t> (cx + x), olc::PixelGameEngine::ScreenHeight () - 1,
                                    olc::WHITE, 0xFFFFFFFF);

    for (int j = 0; j < rows_; j++)
      curves_[j * columns_ + i]->setX (cx + x);
  } // end FOR

  for (int j = 0; j < rows_; j++)
  {
    int32_t cx = ENGINE_PGE_219_DEFAULT_W / 2;
    int32_t cy = ENGINE_PGE_219_DEFAULT_W + j * ENGINE_PGE_219_DEFAULT_W + ENGINE_PGE_219_DEFAULT_W / 2;
    olc::PixelGameEngine::DrawCircle (cx, cy, static_cast<int32_t> (r), olc::WHITE);

    float x = r * std::cos (angle_ * (j + 1) - static_cast<float> (M_PI_2));
    float y = r * std::sin (angle_ * (j + 1) - static_cast<float> (M_PI_2));
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (cx + x), static_cast<int32_t> (cy + y), 4, olc::WHITE);
    olc::PixelGameEngine::DrawLine (0, static_cast<int32_t> (cy + y), olc::PixelGameEngine::ScreenWidth () - 1, static_cast<int32_t> (cy + y),
                                    olc::WHITE, 0xFFFFFFFF);

    for (int i = 0; i < columns_; i++)
      curves_[j * columns_ + i]->setY (cy + y);
  } // end FOR

  for (int j = 0; j < rows_; j++)
    for (int i = 0; i < columns_; i++)
    {
      curves_[j * columns_ + i]->addPoint ();
      curves_[j * columns_ + i]->show (this);
    } // end FOR

  angle_ -= 0.01f;

  if (angle_ < 2.0f * static_cast<float> (-M_PI))
  {
    for (int j = 0; j < rows_; j++)
      for (int i = 0; i < columns_; i++)
        curves_[j * columns_ + i]->reset ();

    angle_ = 0.0f;
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
