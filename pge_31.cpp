#include "stdafx.h"

#include "pge_31.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines.h"
#include "engine_common.h"

PGE_31::PGE_31 ()
 : olc::PixelGameEngine ()
 , x_ (ENGINE_PGE_31_DEFAULT_NOISE_X)
 , y_ (ENGINE_PGE_31_DEFAULT_NOISE_Y)
 , z_ (ENGINE_PGE_31_DEFAULT_NOISE_Z)
 , module_ ()
{
  sAppName = "Example 31";
}

bool
PGE_31::OnUserCreate ()
{
  return true;
}

bool
PGE_31::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  std::vector<olc::vi2d> points_a;
  for (float a = 0.0f; a < static_cast<float> (M_PI * 2.0); a += 0.05f)
  {
    float offset =
      Common_GL_Tools::map (static_cast<float> (module_.GetValue (x_, y_, z_)), -1.0f, 1.0f, -25.0f, 25.0f);
    float r = ENGINE_PGE_31_DEFAULT_RADIUS + offset;
    float x = r * std::cosf (a);
    float y = r * std::sinf (a);
    points_a.push_back ({(olc::PixelGameEngine::ScreenWidth ()  / 2) + static_cast<int32_t> (x),
                         (olc::PixelGameEngine::ScreenHeight () / 2) + static_cast<int32_t> (y)});
    x_ += 0.05;
  } // end FOR
  y_ += 0.01;

  std::vector<olc::vi2d>::const_iterator iterator;
  for (iterator = points_a.begin ();
       iterator != points_a.end ();
       ++iterator)
  {
    if (std::next (iterator) == points_a.end ())
      break;
    olc::PixelGameEngine::DrawLine (*iterator, *std::next (iterator), olc::WHITE, 0xFFFFFFFF);
  } // end FOR
  iterator = std::prev (points_a.end ());
  olc::PixelGameEngine::DrawLine (*iterator, points_a[0], olc::WHITE, 0xFFFFFFFF);

  // flood-fill from the center
  floodFill ({olc::PixelGameEngine::ScreenWidth ()  / 2,
              olc::PixelGameEngine::ScreenHeight () / 2});

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_31::floodFill (const olc::vi2d& seed_in)
{
  std::vector<olc::vi2d> queue;
  queue.push_back (seed_in);

  // Color the pixel with the new color
  olc::PixelGameEngine::Draw (seed_in, olc::WHITE);

  olc::Sprite* sprite_p = olc::PixelGameEngine::GetDrawTarget ();
  while (queue.size () > 0)
  {
    olc::vi2d point_s = queue[queue.size () - 1];
    queue.pop_back ();

    olc::vi2d point_temp = point_s;
    point_temp.x++;
    if (sprite_p->GetPixel (point_temp) != olc::WHITE)
    {
      olc::PixelGameEngine::Draw (point_temp, olc::WHITE);
      queue.push_back (point_temp);
    } // end IF

    point_temp = point_s;
    point_temp.x--;
    if (sprite_p->GetPixel (point_temp) != olc::WHITE)
    {
      olc::PixelGameEngine::Draw (point_temp, olc::WHITE);
      queue.push_back (point_temp);
    } // end IF

    point_temp = point_s;
    point_temp.y++;
    if (sprite_p->GetPixel (point_temp) != olc::WHITE)
    {
      olc::PixelGameEngine::Draw (point_temp, olc::WHITE);
      queue.push_back (point_temp);
    } // end IF

    point_temp = point_s;
    point_temp.y--;
    if (sprite_p->GetPixel (point_temp) != olc::WHITE)
    {
      olc::PixelGameEngine::Draw (point_temp, olc::WHITE);
      queue.push_back (point_temp);
    } // end IF
  } // end WHILE
}
