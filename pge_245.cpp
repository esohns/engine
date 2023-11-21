#include "stdafx.h"

#include "pge_245.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_11.h"

float
scaling (float x)
{
  x = 0.87f - x;
  return std::exp (-7.6f * x) - std::exp (-5.0f * x) * (std::cos (10.05f * static_cast<float> (M_PI) * x) / 5.0f + 1.0f);
}

//function scaling1(x) {
//  return 0.5 - 0.5 * cos(PI * x)
//}
//
//function scaling2(x) {
//  return 3.7626 / PI * (sin(PI * x) + 1 / 3 * sin(3 * PI * x) + 1 / 5 * sin(5 * PI * x) + 1 / 7 * sin(7 * PI * x) + 1 / 9 * sin(9 * PI * x))
//}
//
//function scaling3(x) {
//  return 1 - exp(-10 * x) * cos(6 * PI * x)
//}
//
//function scaling4(x) {
//  x = 0.87 - x;
//  return exp(-7.6 * x) - exp(-5 * x) * (cos(10.05 * PI * x) / 5 + 1);
//}
//
//function scaling5(x) {
//  return x;
//}

//////////////////////////////////////////

PGE_245::PGE_245 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , newPoints_ ()
 , g_ ()
 , ds_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 245");
}

bool
PGE_245::OnUserCreate ()
{
  points_ = genPoints ();
  newPoints_ = genPoints ();
  
  g_ =
    gradient (ENGINE_PGE_245_DEFAULT_W + 2, ENGINE_PGE_245_DEFAULT_H + 2,
              olc::Pixel (34, 193, 195), olc::Pixel (253, 187, 45), olc::Pixel (151, 68, 238), olc::Pixel (255, 97, 198));

  for (int i = 0; i < ENGINE_PGE_245_DEFAULT_H + 2; i++)
  {
    std::vector<std::pair<olc::vf2d, olc::vi2d> > ds_row;
    for (int j = 0; j < ENGINE_PGE_245_DEFAULT_W + 2; j++)
      ds_row.push_back (std::make_pair (newPoints_[i][j] - points_[i][j],
                                        olc::vi2d (0, Common_Tools::getRandomNumber (0, ENGINE_PGE_245_DEFAULT_STEPS / 2))));
    ds_.push_back (ds_row);
  } // end FOR

  return true;
}

bool
PGE_245::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < static_cast<int> (ds_.size ()); i++)
    for (int j = 0; j < static_cast<int> (ds_[0].size ()); j++)
    {
      if (ds_[i][j].second.x == ds_[i][j].second.y)
      { // If a point has reached the end of its path
        newPoints_[i][j] = genPoint (j, i); // Generate a new point
        ds_[i][j].first = newPoints_[i][j] - points_[i][j];	// Update the direction matrix to get dx, dy, and a new path timer
        ds_[i][j].second.x = 0;
        ds_[i][j].second.y = Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_STEPS / 2, ENGINE_PGE_245_DEFAULT_STEPS);
      } // end IF
      else
      { // If a point needs to move
        float factor =
          (ds_[i][j].second.y - ds_[i][j].second.x) / static_cast<float> (ds_[i][j].second.y); // (steps-step)/steps
        factor = scaling (factor);
        points_[i][j].x = newPoints_[i][j].x - factor * ds_[i][j].first.x;
        points_[i][j].y = newPoints_[i][j].y - factor * ds_[i][j].first.y;
        ds_[i][j].second.x++;
      } // end ELSE
    } // end FOR

  drawMesh (points_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<std::vector<olc::vf2d> >
PGE_245::genPoints ()
{
  std::vector<std::vector<olc::vf2d> > circles;
  for (int i = 0; i < ENGINE_PGE_245_DEFAULT_H + 2; i++)
  {
    std::vector<olc::vf2d> temp;
    for (int j = 0; j < ENGINE_PGE_245_DEFAULT_W + 2; j++)
    {
      olc::vf2d temp_2 (0.0f, 0.0f);
      temp.push_back (temp_2);
    } // end FOR
    circles.push_back (temp);
  } // end FOR

  for (int i = 0; i < static_cast<int> (circles.size ()); i++)
    for (int j = 0; j < static_cast<int> (circles[0].size ()); j++)
      circles[i][j] = genPoint (j, i);

  return circles;
}

olc::vf2d
PGE_245::genPoint (int x, int y)
{
  olc::vf2d point_s;

  // Generate four corners for grid
  if (x == 0 && y == 0) return {0.0f, 0.0f};
  else if (x == ENGINE_PGE_245_DEFAULT_W + 1 && y == 0) return {static_cast<float> (ENGINE_PGE_245_DEFAULT_WIDTH), 0.0f};
  else if (x == 0 && y == ENGINE_PGE_245_DEFAULT_H + 1) return {0.0f, static_cast<float> (ENGINE_PGE_245_DEFAULT_HEIGHT)};
  else if (x == ENGINE_PGE_245_DEFAULT_W + 1 && y == ENGINE_PGE_245_DEFAULT_H + 1) return {static_cast<float> (ENGINE_PGE_245_DEFAULT_WIDTH), static_cast<float> (ENGINE_PGE_245_DEFAULT_HEIGHT)};
  // Generate edges for grid
  else if (y == 0) return {(x - 1) * ENGINE_PGE_245_DEFAULT_GRID_SIZE + Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_GRID_SIZE * (1.0f - ENGINE_PGE_245_DEFAULT_DZ), ENGINE_PGE_245_DEFAULT_GRID_SIZE * ENGINE_PGE_245_DEFAULT_DZ), 0.0f};
  else if (y == ENGINE_PGE_245_DEFAULT_H + 1) return {(x - 1) * ENGINE_PGE_245_DEFAULT_GRID_SIZE + Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_GRID_SIZE * (1.0f - ENGINE_PGE_245_DEFAULT_DZ), ENGINE_PGE_245_DEFAULT_GRID_SIZE * ENGINE_PGE_245_DEFAULT_DZ), static_cast<float> (ENGINE_PGE_245_DEFAULT_HEIGHT)};
  else if (x == 0) return {0.0f, (y - 1) * ENGINE_PGE_245_DEFAULT_GRID_SIZE + Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_GRID_SIZE * ENGINE_PGE_245_DEFAULT_DZ, ENGINE_PGE_245_DEFAULT_GRID_SIZE * (1.0f - ENGINE_PGE_245_DEFAULT_DZ))};
  else if (x == ENGINE_PGE_245_DEFAULT_W + 1) return {static_cast<float> (ENGINE_PGE_245_DEFAULT_WIDTH), (y - 1) * ENGINE_PGE_245_DEFAULT_GRID_SIZE + Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_GRID_SIZE * ENGINE_PGE_245_DEFAULT_DZ, ENGINE_PGE_245_DEFAULT_GRID_SIZE * (1.0f - ENGINE_PGE_245_DEFAULT_DZ))};
  // Generate centers for grid
  else
  {
    int x1 = (x - 1) * ENGINE_PGE_245_DEFAULT_GRID_SIZE;
    int y1 = (y - 1) * ENGINE_PGE_245_DEFAULT_GRID_SIZE;
    point_s.x = x1 + Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_GRID_SIZE * ENGINE_PGE_245_DEFAULT_DZ, ENGINE_PGE_245_DEFAULT_GRID_SIZE * (1.0f - ENGINE_PGE_245_DEFAULT_DZ));
    point_s.y = y1 + Common_Tools::getRandomNumber (ENGINE_PGE_245_DEFAULT_GRID_SIZE * ENGINE_PGE_245_DEFAULT_DZ, ENGINE_PGE_245_DEFAULT_GRID_SIZE * (1.0f - ENGINE_PGE_245_DEFAULT_DZ));
  } // end ELSE

  return point_s;
}

std::vector<std::vector<olc::Pixel> >
PGE_245::gradient (int w, int h, olc::Pixel& c1, olc::Pixel& c2, olc::Pixel& c3, olc::Pixel& c4)
{
  std::vector<std::vector<olc::Pixel> > grid;

  for (int i = 0; i < h; i++)
  {
    std::vector<olc::Pixel> temp;
    for (int j = 0; j < w; j++)
      temp.push_back (olc::BLACK);
    grid.push_back (temp);
  } // end FOR

  // c1 c2
  // c3 c4
  grid[0][0] = c1;
  grid[0][w - 1] = c2;
  grid[h - 1][0] = c3;
  grid[h - 1][w - 1] = c4;

  // Start with top and bottom rows
  for (int x = 0; x < w; x++)
  {
    grid[0][x] = olc::PixelLerp (c1, c2, x / static_cast<float> (w));
    grid[h - 1][x] = olc::PixelLerp (c3, c4, x / static_cast<float> (w));
  } // end FOR

  // Fill in center
  for (int x = 0; x < w; x++)
  {
    olc::Pixel c1 = grid[0][x];
    olc::Pixel c2 = grid[h - 1][x];
    for (int y = 1; y < h - 1; y++)
      grid[y][x] = olc::PixelLerp (c1, c2, y / static_cast<float> (h - 1));
  } // end FOR

  return grid;
}

void
PGE_245::drawMesh (std::vector<std::vector<olc::vf2d> >& circles)
{
  // Pick the widest angle, bisect it
  for (int i = 0; i < ENGINE_PGE_245_DEFAULT_W + 1; i++)
    for (int j = 0; j < ENGINE_PGE_245_DEFAULT_H + 1; j++)
    {
      // p1 -> p2
      //  ^     |
      //  |     V
      // p3 <- p4
      olc::vf2d& v1 = circles[j][i];
      olc::vf2d& v2 = circles[j][i + 1];
      olc::vf2d& v3 = circles[j + 1][i];
      olc::vf2d& v4 = circles[j + 1][i + 1];

      olc::vf2d v12 = (v2 - v1).norm ();
      olc::vf2d v21 = (v1 - v2).norm ();
      olc::vf2d v24 = (v4 - v2).norm ();
      olc::vf2d v42 = (v2 - v4).norm ();
      olc::vf2d v43 = (v3 - v4).norm ();
      olc::vf2d v34 = (v4 - v3).norm ();
      olc::vf2d v31 = (v1 - v3).norm ();
      olc::vf2d v13 = (v3 - v1).norm ();

      // angles between vectors
      float angle1 = std::acos (v24.dot (v21));
      float angle2 = std::acos (v43.dot (v42));
      float angle3 = std::acos (v31.dot (v34));
      float angle4 = std::acos (v12.dot (v13));

      olc::Pixel& c = g_[j][i];

      float m = std::max (angle1, std::max (angle2, std::max (angle3, angle4)));
      if (angle1 < 0.0f || angle3 < 0.0f || m == angle1 || m == angle3)
      {
        olc::PixelGameEngine::FillTriangle (v1, v2, v3, c);
        c.a = 240;
        olc::PixelGameEngine::FillTriangle (v4, v2, v3, c);
      } // end IF
      else
      {
        olc::PixelGameEngine::FillTriangle (v1, v2, v4, c);
        c.a = 240;
        olc::PixelGameEngine::FillTriangle (v1, v3, v4, c);
      } // end ELSE
      c.a = 255;
    } // end FOR
}
