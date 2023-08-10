#include "stdafx.h"

#include "pge_96.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_96::PGE_96 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , paths_ ()
 , pointIdx_ (0)
 , pointsPerFrame_ (1.0f)
{
  sAppName = "Example 96";
}

bool
PGE_96::OnUserCreate ()
{
  std::vector<olc::vi2d> visited_a;
  olc::vi2d point_s (0, 0);
  visited_a.push_back (point_s);
  findPaths (point_s, visited_a);

  rsfc (ENGINE_PGE_96_DEFAULT_MARGIN, ENGINE_PGE_96_DEFAULT_MARGIN, olc::PixelGameEngine::ScreenWidth () - 2.0f * ENGINE_PGE_96_DEFAULT_MARGIN, true, true, 0);

  return true;
}

bool
PGE_96::OnUserUpdate (float fElapsedTime)
{
  static bool done_b = false;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  { // reset, choose another (random) path
    done_b = false;
    points_.clear ();
    pointIdx_ = 0;
    pointsPerFrame_ = 1.0f;

    rsfc (ENGINE_PGE_96_DEFAULT_MARGIN, ENGINE_PGE_96_DEFAULT_MARGIN, olc::PixelGameEngine::ScreenWidth() - 2.0f * ENGINE_PGE_96_DEFAULT_MARGIN, true, true, 0);

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  if (done_b) goto end;

  for (int k = 0; k < static_cast<int32_t> (pointsPerFrame_); k++)
  {
    olc::PixelGameEngine::DrawLine (points_[pointIdx_], points_[pointIdx_ + 1],
                                    rainbow (pointIdx_ / static_cast<float> (points_.size ())),
                                    0xFFFFFFFF);

    pointIdx_++;
    if (pointIdx_ == points_.size () - 1)
    {
      done_b = true;
      break;
    }
  } // end FOR

  pointsPerFrame_ += 0.01f;

end:
  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<olc::vi2d>
PGE_96::possibleNeighbors (olc::vi2d& p)
{
  std::vector<olc::vi2d> possibilities;
  if (p.x % 2 == 0 && p.y < ENGINE_PGE_96_DEFAULT_N - 1) possibilities.push_back ({p.x, p.y + 1});
  if (p.x % 2 == 1 && p.y > 0) possibilities.push_back ({p.x, p.y - 1});
  if (p.y % 2 == 0 && p.x < ENGINE_PGE_96_DEFAULT_N - 1) possibilities.push_back ({p.x + 1, p.y});
  if (p.y % 2 == 1 && p.x > 0) possibilities.push_back ({p.x - 1, p.y});
  return possibilities;
}

bool
PGE_96::inArray (olc::vi2d& p, std::vector<olc::vi2d>& arr)
{
  for (std::vector<olc::vi2d>::iterator iterator = arr.begin ();
       iterator != arr.end ();
       ++iterator)
    if ((*iterator).x == p.x && (*iterator).y == p.y) return true;
  return false;
}

void
PGE_96::findPaths (olc::vi2d& p, std::vector<olc::vi2d>& visited)
{
  std::vector<olc::vi2d> neighbors = possibleNeighbors (p);
  if (neighbors.empty ())
  {
    if (visited.size () == (ENGINE_PGE_96_DEFAULT_N * ENGINE_PGE_96_DEFAULT_N)) paths_.push_back (visited);
    return;
  } // end IF

  for (std::vector<olc::vi2d>::iterator iterator = neighbors.begin ();
       iterator != neighbors.end ();
       ++iterator)
  {
    if (!inArray (*iterator, visited))
    {
      std::vector<olc::vi2d> visited_a = visited;
      visited_a.push_back (*iterator);
      findPaths (*iterator, visited_a);
    } // end IF
  } // end FOR
}

void
PGE_96::rsfc (float x0, float y0, float s, bool topToBottom, bool leftToRight, int32_t depth)
{
  if (depth == ENGINE_PGE_96_DEFAULT_MAX_DEPTH)
  {
    points_.push_back ({x0 + s / 2.0f, y0 + s / 2.0f});
    return;
  } // end IF

  float newS = s / static_cast<float> (ENGINE_PGE_96_DEFAULT_N);
  int32_t idx1 = topToBottom ? 0 : 1;
  int32_t idx2 = leftToRight ? 0 : 1;
  ACE_ASSERT (!paths_.empty ());
  int32_t i = Common_Tools::getRandomNumber (0, static_cast<int32_t> (paths_.size () - 1));

  for (std::vector<olc::vi2d>::iterator iterator = paths_[i].begin ();
       iterator != paths_[i].end ();
       ++iterator)
  {
    float x = leftToRight ? (*iterator).x * newS : (ENGINE_PGE_96_DEFAULT_N - (*iterator).x - 1) * newS;
    float y = topToBottom ? (*iterator).y * newS : (ENGINE_PGE_96_DEFAULT_N - (*iterator).y - 1) * newS;
    rsfc (x0 + x, y0 + y, newS, (*iterator).x % 2 == idx1, (*iterator).y % 2 == idx2, depth + 1);
  } // end FOR
}

olc::Pixel
PGE_96::rainbow (float t)
{
  static Common_GL_Color_t palette[ENGINE_PGE_96_DEFAULT_NUMBER_OF_COLORS] =
    {{0xef, 0x56, 0x2f}, {0xf9, 0xd5, 0x31}, {0xa7, 0xcc, 0x51}, {0x4b, 0xaf, 0xdd}, {0xbd, 0x4e, 0xe5}};
  int32_t i = static_cast<int32_t> (std::floor (ENGINE_PGE_96_DEFAULT_NUMBER_OF_COLORS * t));
  float int_f;
  float amt = std::modf (ENGINE_PGE_96_DEFAULT_NUMBER_OF_COLORS * t, &int_f);
  Common_GL_Color_t lerped_color =
    Common_GL_Tools::lerpRGB (palette[i % ENGINE_PGE_96_DEFAULT_NUMBER_OF_COLORS], palette[(i + 1) % ENGINE_PGE_96_DEFAULT_NUMBER_OF_COLORS], amt);
  olc::Pixel result = {lerped_color.r, lerped_color.g, lerped_color.b, 255};
  return result;
}
