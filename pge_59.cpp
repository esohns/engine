#include "stdafx.h"

#include "pge_59.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_59::PGE_59 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 59";
}

bool
PGE_59::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  reset ();

  return true;
}

bool
PGE_59::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear ({0,0,0, ENGINE_PGE_59_DEFAULT_ALPHA_DECAY});
  int pixels = GetDrawTargetWidth () * GetDrawTargetHeight ();
  olc::Pixel* p = GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_59_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_59_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    reset ();

  for (int i = 0; i < ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS; i++)
  {
    float dx = olc::PixelGameEngine::GetMouseX () - x[i];
    float dy = olc::PixelGameEngine::GetMouseY () - y[i];
    float d = std::sqrt (dx*dx + dy*dy);
    if (d < 1) d = 1;

    float f = std::cos (d * 0.06f) * m[i] / d*2.0f;

    vx[i] = vx[i] * 0.4f - f * dx;
    vy[i] = vy[i] * 0.2f - f * dy;
  } // end FOR

  for (int i = 0; i < ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS; i++)
  {
    x[i] += vx[i];
    y[i] += vy[i];

    if (x[i] < 0) x[i] = static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1);
    else if (x[i] > olc::PixelGameEngine::ScreenWidth () - 1) x[i] = 0.0f;
    if (y[i] < 0) y[i] = static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1);
    else if (y[i] > olc::PixelGameEngine::ScreenHeight () - 1) y[i] = 0.0f;

    olc::Pixel color = {red[i] , green[i] , blue[i] , 30};
    if (m[i] < 0)
    {
      color.r = blue[i]; color.b = 255;
    } // end IF
    else
    {
      color.r = 255; color.g = blue[i]; color.b = red[i];
    } // end ELSE
    if (shape[i] > 2)
    {
      color.r = blue[i]; color.g = green[i]; color.b = 255;
    } // end IF
    else
    {
      color.r = 255; color.g = blue[i]; color.b = red[i];
    } // end ELSE

    if (shape[i] > 2)
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x[i]), static_cast<int32_t> (y[i]), 10, 10, color);
    else if (shape[i] > 1 && shape[i] <=2)
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x[i]), static_cast<int32_t> (y[i]), 2, 2, color);
    else
      olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x[i]), static_cast<int32_t> (y[i]), 5, color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_59::reset ()
{
  std::normal_distribution<float> distribution (0.0f, 1.0f);

  for (int i = 0; i < ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS; i++)
  {
    m[i] = Common_Tools::getRandomNumber (distribution) * 8.0f;
    x[i] = static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1));
    y[i] = static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1));
    blue[i] = Common_Tools::getRandomNumber (0, 255);
    red[i] = Common_Tools::getRandomNumber (0, 255);;
    green[i] = Common_Tools::getRandomNumber (0, 255);
    shape[i] = Common_Tools::getRandomNumber (0, 3);
  } // end FOR
}
