#include "stdafx.h"

#include "pge_94.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_94::PGE_94 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , paths_ ()
 , pointIdx_ (0)
 , pointsPerFrame_ (1.0f)
{
  sAppName = "Example 94";
}

bool
PGE_94::OnUserCreate ()
{
  std::vector<olc::vi2d> visited_a;
  olc::vi2d point_s (0, 0);
  visited_a.push_back (point_s);
  findPaths (point_s, visited_a);

  rsfc (0.0f, 0.0f, 1.0f, true, true, 0);
  float delta = 1.0f - points_[points_.size () - 1].x;
  points_.push_back ({1.0f - delta, 1.0f + delta});

  return true;
}

bool
PGE_94::OnUserUpdate (float fElapsedTime)
{
  static bool done_b = false;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  { // reset, choose another (random) path
    done_b = false;
    points_.clear ();
    pointIdx_ = 0;
    pointsPerFrame_ = 1.0f;

    rsfc (0.0f, 0.0f, 1.0f, true, true, 0);
    float delta = 1.0f - points_[points_.size () - 1].x;
    points_.push_back ({1.0f - delta, 1.0f + delta});

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  float r0 = 50.0f, rDelta = 200.0f;

  if (done_b) goto end;

  for (int k = 0; k < static_cast<int32_t> (pointsPerFrame_); k++)
  {
    olc::vf2d p1 = points_[pointIdx_], p2 = points_[pointIdx_ + 1];

    float r1 = p1.x * rDelta + r0, r2 = p2.x * rDelta + r0;
    float theta1 = p1.y * static_cast<float> (M_PI), theta2 = p2.y * static_cast<float> (M_PI);
    int32_t x1 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth() / 2.0f + r1 * std::cos (theta1));
    int32_t y1 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / 2.0f + r1 * std::sin (theta1));
    int32_t x2 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth() / 2.0f + r2 * std::cos (theta2));
    int32_t y2 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / 2.0f + r2 * std::sin (theta2));

    olc::vf2d center = {olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight() / 2.0f};
    olc::Pixel color = rainbow (pointIdx_ / (2.0f * points_.size ()));
    if (p1.y == p2.y)
      olc::PixelGameEngine::DrawLine (x1, y1, x2, y2, color, 0xFFFFFFFF);
    else
    {
      if (theta1 > theta2)
      {
        float temp = theta1;
        theta1 = theta2;
        theta2 = temp;
      } // end IF
      draw_circle_arc (center, (r1 + r2) / 2.0f, theta1, theta2, color);
      //arc (olc::PixelGameEngine::ScreenWidth () / 2, olc::PixelGameEngine::ScreenHeight () / 2, 2 * r1, 2 * r2, theta1, theta2);
    } // end ELSE

    r1 = (1.0f - p1.x) * rDelta + r0, r2 = (1.0f - p2.x) * rDelta + r0;
    theta1 = p1.y * static_cast<float> (M_PI) + static_cast<float> (M_PI), theta2 = p2.y * static_cast<float> (M_PI) + static_cast<float> (M_PI);
    x1 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () / 2.0f + r1 * std::cos (theta1));
    y1 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / 2.0f + r1 * std::sin (theta1));
    x2 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () / 2.0f + r2 * std::cos (theta2));
    y2 =
      static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / 2.0f + r2 * std::sin (theta2));

    color = rainbow (1.0f / 2.0f + pointIdx_ / (2.0f * points_.size ()));
    if (p1.y == p2.y)
      olc::PixelGameEngine::DrawLine (x1, y1, x2, y2, color, 0xFFFFFFFF);
    else
    {
      if (theta1 > theta2)
      {
        float temp = theta1;
        theta1 = theta2;
        theta2 = temp;
      } // end IF
      draw_circle_arc (center, (r1 + r2) / 2.0f, theta1, theta2, color);
      //arc (olc::PixelGameEngine::ScreenWidth () / 2, olc::PixelGameEngine::ScreenHeight () / 2, 2 * r1, 2 * r2, theta1, theta2);
    } // end ELSE

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
PGE_94::possibleNeighbors (olc::vi2d& p)
{
  std::vector<olc::vi2d> possibilities;
  if (p.x % 2 == 0 && p.y < ENGINE_PGE_94_DEFAULT_N - 1) possibilities.push_back ({p.x, p.y + 1});
  if (p.x % 2 == 1 && p.y > 0) possibilities.push_back ({p.x, p.y - 1});
  if (p.y % 2 == 0 && p.x < ENGINE_PGE_94_DEFAULT_N - 1) possibilities.push_back ({p.x + 1, p.y});
  if (p.y % 2 == 1 && p.x > 0) possibilities.push_back ({p.x - 1, p.y});
  return possibilities;
}

bool
PGE_94::inArray (olc::vi2d& p, std::vector<olc::vi2d>& arr)
{
  for (std::vector<olc::vi2d>::iterator iterator = arr.begin ();
       iterator != arr.end ();
       ++iterator)
    if ((*iterator).x == p.x && (*iterator).y == p.y) return true;
  return false;
}

void
PGE_94::findPaths (olc::vi2d& p, std::vector<olc::vi2d>& visited)
{
  std::vector<olc::vi2d> neighbors = possibleNeighbors (p);
  if (neighbors.empty ())
  {
    if (visited.size () == (ENGINE_PGE_94_DEFAULT_N * ENGINE_PGE_94_DEFAULT_N)) paths_.push_back (visited);
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
PGE_94::rsfc (float x0, float y0, float s, bool topToBottom, bool leftToRight, int32_t depth)
{
  if (depth == ENGINE_PGE_94_DEFAULT_MAX_DEPTH)
  {
    points_.push_back ({x0 + s / 2.0f, y0 + s / 2.0f});
    return;
  } // end IF

  float newS = s / static_cast<float> (ENGINE_PGE_94_DEFAULT_N);
  int32_t idx1 = topToBottom ? 0 : 1;
  int32_t idx2 = leftToRight ? 0 : 1;
  ACE_ASSERT (!paths_.empty ());
  int32_t i = Common_Tools::getRandomNumber (0, static_cast<int32_t> (paths_.size () - 1));

  for (std::vector<olc::vi2d>::iterator iterator = paths_[i].begin ();
       iterator != paths_[i].end ();
       ++iterator)
  {
    float x = leftToRight ? (*iterator).x * newS : (ENGINE_PGE_94_DEFAULT_N - (*iterator).x - 1) * newS;
    float y = topToBottom ? (*iterator).y * newS : (ENGINE_PGE_94_DEFAULT_N - (*iterator).y - 1) * newS;
    rsfc (x0 + x, y0 + y, newS, (*iterator).x % 2 == idx1, (*iterator).y % 2 == idx2, depth + 1);
  } // end FOR
}

olc::Pixel
PGE_94::rainbow (float t)
{
  static Common_GL_Color_t palette[ENGINE_PGE_94_DEFAULT_NUMBER_OF_COLORS] =
    {{0xef, 0x56, 0x2f, 0xff}, {0xf9, 0xd5, 0x31, 0xff}, {0xa7, 0xcc, 0x51, 0xff}, {0x4b, 0xaf, 0xdd, 0xff}, {0xbd, 0x4e, 0xe5, 0xff}};
  int32_t i = static_cast<int32_t> (std::floor (ENGINE_PGE_94_DEFAULT_NUMBER_OF_COLORS * t));
  float int_f;
  float amt = std::modf (ENGINE_PGE_94_DEFAULT_NUMBER_OF_COLORS * t, &int_f);
  Common_GL_Color_t lerped_color =
    Common_GL_Tools::lerpRGB (palette[i % ENGINE_PGE_94_DEFAULT_NUMBER_OF_COLORS], palette[(i + 1) % ENGINE_PGE_94_DEFAULT_NUMBER_OF_COLORS], amt);
  olc::Pixel result = {lerped_color.r, lerped_color.g, lerped_color.b, 255};
  return result;
}

void
PGE_94::draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color)
{
  std::vector<olc::vf2d> points_arc;

  for (int i = 0; i <= ENGINE_PGE_94_DEFAULT_ARC_RESOLUTION + 1; ++i)
  {
    float angle_point =
      (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_94_DEFAULT_ARC_RESOLUTION));
    olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
    point_s *= radius;
    point_s += center;
    points_arc.push_back (point_s);
  } // end FOR

  for (int i = 0; i <= ENGINE_PGE_94_DEFAULT_ARC_RESOLUTION; i++)
    olc::PixelGameEngine::DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
}
