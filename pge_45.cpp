#include "stdafx.h"

#include "pge_45.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_45::PGE_45 ()
 : olc::PixelGameEngine ()
 , path_ ()
{
  sAppName = "Example 45";
}

bool
PGE_45::OnUserCreate ()
{
  int N = int(std::pow (2, ENGINE_PGE_45_DEFAULT_ORDER));
  int total = N * N;

  for (int i = 0; i < total; i++)
  {
    path_.push_back (hilbert (i));
    int len = olc::PixelGameEngine::ScreenWidth () / N;
    path_[i] *= static_cast<float> (len);
    path_[i] += {len / 2.0f, len / 2.0f};
  } // end FOR

  return true;
}

bool
PGE_45::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static int counter = 0;
  for (int i = 1; i < counter; i++)
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (path_[i].x), static_cast<int32_t> (path_[i].y),
                                    static_cast<int32_t> (path_[i - 1].x), static_cast<int32_t> (path_[i - 1].y),
                                    olc::WHITE, 0xFFFFFFFF);

  counter += 150;
  if (counter >= path_.size ())
    counter = 0;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::vf2d
PGE_45::hilbert (int i)
{
  const olc::vf2d points[4] = {
    olc::vf2d (0.0f, 0.0f),
    olc::vf2d (0.0f, 1.0f),
    olc::vf2d (1.0f, 1.0f),
    olc::vf2d (1.0f, 0.0f)
  };

  int index = i & 3;
  olc::vf2d v = points[index];

  for (int j = 1; j < ENGINE_PGE_45_DEFAULT_ORDER; j++)
  {
    i = i >> 2;
    index = i & 3;
    int len = static_cast<int> (std::pow (2, j));
    if (index == 0)
    {
      float temp = v.x;
      v.x = v.y;
      v.y = temp;
    } // end IF
    else if (index == 1)
    {
      v.y += len;
    } // end ELSE IF
    else if (index == 2)
    {
      v.x += len;
      v.y += len;
    } // end ELSE IF
    else if (index == 3)
    {
      float temp = len - 1 - v.x;
      v.x = len - 1 - v.y;
      v.y = temp;
      v.x += len;
    } // end ELSE IF
  } // end FOR

  return v;
}
