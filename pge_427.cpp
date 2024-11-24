#include "stdafx.h"

#include "pge_427.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "engine_common.h"

PGE_427::PGE_427 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , fill_ (false)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 427");
}

bool
PGE_427::OnUserCreate ()
{
  return true;
}

bool
PGE_427::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_427_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_427_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    particles_.push_back (particle (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), static_cast<float> (olc::PixelGameEngine::GetMouseY ()),
                                    ENGINE_PGE_427_DEFAULT_MAX_LEVEL));
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
    fill_ = !fill_;

  for (int i = static_cast<int> (particles_.size ()) - 1; i > -1; --i)
  {
    particle& particle_r = particles_[i];
    particle_r.move (particles_);

    if (particle_r.velocity.mag () < 0.1f)
    {
      std::vector<particle>::iterator iterator = particles_.begin ();
      std::advance (iterator, i);
      particles_.erase (iterator);
    } // end IF
  } // end FOR

  float r, g, b;
  std::vector<olc::vf2d> points_a;
  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    points_a.push_back ((*iterator).position);
  std::vector<int> data = triangulate (points_a);
  for (int i = 0; i < static_cast<int> (data.size ()); i += 3)
  {
    particle& p1 = particles_[data[i]];
    particle& p2 = particles_[data[i + 1]];
    particle& p3 = particles_[data[i + 2]];

    if (p1.position.dist (p2.position) > ENGINE_PGE_427_DEFAULT_DISTANCE_THRESHOLD)
      continue;
    if (p2.position.dist (p3.position) > ENGINE_PGE_427_DEFAULT_DISTANCE_THRESHOLD)
      continue;
    if (p1.position.dist (p3.position) > ENGINE_PGE_427_DEFAULT_DISTANCE_THRESHOLD)
      continue;

    Common_Image_Tools::HSVToRGB (165.0f + p1.life * 1.5f, 1.0f, 1.0f, r, g, b);
    if (fill_)
      olc::PixelGameEngine::FillTriangle (p1.position, p2.position, p3.position, olc::PixelF (r, g, b));
    else
      olc::PixelGameEngine::DrawTriangle (p1.position, p2.position, p3.position, olc::PixelF (r, g, b));
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

struct PGE_427::triangle
PGE_427::superTriangle (std::vector<olc::vf2d>& vertices_in)
{
  struct triangle result;

  float xmin = std::numeric_limits<float>::infinity ();
  float ymin = std::numeric_limits<float>::infinity ();
  float xmax = -std::numeric_limits<float>::infinity ();
  float ymax = -std::numeric_limits<float>::infinity ();
  float dx, dy, dmax, xmid, ymid;

  for (std::vector<olc::vf2d>::iterator iterator = vertices_in.begin ();
       iterator != vertices_in.end ();
       ++iterator)
  {
    if ((*iterator).x < xmin) xmin = (*iterator).x;
    if ((*iterator).x > xmax) xmax = (*iterator).x;
    if ((*iterator).y < ymin) ymin = (*iterator).y;
    if ((*iterator).y > ymax) ymax = (*iterator).y;
  } // end FOR

  dx = xmax - xmin;
  dy = ymax - ymin;
  dmax = std::max (dx, dy);
  xmid = xmin + dx * 0.5f;
  ymid = ymin + dy * 0.5f;

  result.p1.x = xmid - 20.0f * dmax;
  result.p1.y = ymid - dmax;
  result.p2.x = xmid;
  result.p2.y = ymid + 20.0f * dmax;
  result.p3.x = xmid + 20.0f * dmax;
  result.p3.y = ymid - dmax;

  return result;
}

struct PGE_427::circum_circle
PGE_427::circumCircle (std::vector<olc::vf2d>& vertices_in, int i, int j, int k)
{
  struct circum_circle result;

  float x1 = vertices_in[i].x;
  float y1 = vertices_in[i].y;
  float x2 = vertices_in[j].x;
  float y2 = vertices_in[j].y;
  float x3 = vertices_in[k].x;
  float y3 = vertices_in[k].y;
  float fabsy1y2 = std::abs (y1 - y2);
  float fabsy2y3 = std::abs (y2 - y3);

  float xc, yc, m1, m2, mx1, mx2, my1, my2, dx, dy;

  /* Check for coincident points */
  if (fabsy1y2 < ENGINE_PGE_427_DEFAULT_EPSILON && fabsy2y3 < ENGINE_PGE_427_DEFAULT_EPSILON)
    return result;

  if (fabsy1y2 < ENGINE_PGE_427_DEFAULT_EPSILON)
  {
    m2 = -((x3 - x2) / (y3 - y2));
    mx2 = (x2 + x3) / 2.0f;
    my2 = (y2 + y3) / 2.0f;
    xc = (x2 + x1) / 2.0f;
    yc = m2 * (xc - mx2) + my2;
  } // end IF
  else if (fabsy2y3 < ENGINE_PGE_427_DEFAULT_EPSILON)
  {
    m1 = -((x2 - x1) / (y2 - y1));
    mx1 = (x1 + x2) / 2.0f;
    my1 = (y1 + y2) / 2.0f;
    xc = (x3 + x2) / 2.0f;
    yc = m1 * (xc - mx1) + my1;
  } // end ELSE IF
  else
  {
    m1 = -((x2 - x1) / (y2 - y1));
    m2 = -((x3 - x2) / (y3 - y2));
    mx1 = (x1 + x2) / 2.0f;
    mx2 = (x2 + x3) / 2.0f;
    my1 = (y1 + y2) / 2.0f;
    my2 = (y2 + y3) / 2.0f;
    xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
    yc = (fabsy1y2 > fabsy2y3) ? m1 * (xc - mx1) + my1
                               : m2 * (xc - mx2) + my2;
  } // end ELSE

  dx = x2 - xc;
  dy = y2 - yc;

  result.i = i; result.j = j; result.k = k;
  result.center.x = xc; result.center.y = yc;
  result.radius = dx * dx + dy * dy;
  result.is_valid = true;

  return result;
}

void
PGE_427::deDuplicate (std::vector<std::pair<int, int> >& edges_in)
{
  int i, j, a, b, m, n;
  for (j = static_cast<int> (edges_in.size ()); j;)
  {
    b = edges_in[--j].first;
    a = edges_in[j].second;

    for (i = j; i;)
    {
      n = edges_in[--i].first;
      m = edges_in[i].second;

      if ((a == m && b == n) || (a == n && b == m))
      {
        std::vector<std::pair<int, int> >::iterator iterator = edges_in.begin ();
        std::advance (iterator, j);
        edges_in.erase (iterator);

        iterator = edges_in.begin ();
        std::advance (iterator, i);
        edges_in.erase (iterator);

        if (j > edges_in.size ())
          --j;

        break;
      } // end IF
    } // end FOR
  } // end FOR
}

std::vector<int>
PGE_427::triangulate (std::vector<olc::vf2d>& vertices_in)
{
  std::vector<int> result;

  int n = static_cast<int> (vertices_in.size ());
  if (n < 3)
    return result;

  std::vector<int> indices (n, 0);
  for (int i = n; i--; )
    indices[i] = i;
  std::sort (indices.begin (), indices.end (),
             [&vertices_in] (int i, int j) { return vertices_in[j].x > vertices_in[i].x; });

  struct triangle triangle_s = superTriangle (vertices_in);
  vertices_in.push_back (triangle_s.p1);
  vertices_in.push_back (triangle_s.p2);
  vertices_in.push_back (triangle_s.p3);

  struct circum_circle circum_circle_s = circumCircle (vertices_in, n + 0, n + 1, n + 2);
  if (!circum_circle_s.is_valid)
    return result;

  std::vector<struct circum_circle> open_a;
  open_a.push_back (circum_circle_s);
  std::vector<struct circum_circle> closed_a;
  std::vector<std::pair<int, int> > edges_a;

  /* Incrementally add each vertex to the mesh. */
  for (int i = static_cast<int> (indices.size ()); i--; edges_a.clear ())
  {
    int c = indices[i];

    /* For each open triangle, check to see if the current point is
     * inside it's circumcircle. If it is, remove the triangle and add
     * it's edges to an edge list. */
    for (int j = static_cast<int> (open_a.size ()); j--;)
    {
      /* If this point is to the right of this triangle's circumcircle,
       * then this triangle should never get checked again. Remove it
       * from the open list, add it to the closed list, and skip. */
      float dx = vertices_in[c].x - open_a[j].center.x;
      if (dx > 0.0f && dx * dx > open_a[j].radius)
      {
        closed_a.push_back (open_a[j]);

        std::vector<struct circum_circle>::iterator iterator = open_a.begin ();
        std::advance (iterator, j);
        open_a.erase (iterator);
        continue;
      } // end IF

      /* If we're outside the circumcircle, skip this triangle. */
      float dy = vertices_in[c].y - open_a[j].center.y;
      if (dx * dx + dy * dy - open_a[j].radius > ENGINE_PGE_427_DEFAULT_EPSILON)
        continue;

      /* Remove the triangle and add it's edges to the edge list. */
      edges_a.push_back (std::make_pair (open_a[j].i, open_a[j].j));
      edges_a.push_back (std::make_pair (open_a[j].j, open_a[j].k));
      edges_a.push_back (std::make_pair (open_a[j].k, open_a[j].i));

      std::vector<struct circum_circle>::iterator iterator = open_a.begin ();
      std::advance (iterator, j);
      open_a.erase (iterator);
    } // end FOR

    /* Remove any doubled edges. */
    deDuplicate (edges_a);

    /* Add a new triangle for each edge. */
    for (int j = static_cast<int> (edges_a.size ()); j;)
    {
      int b = edges_a[--j].first;
      int a = edges_a[j].second;
      open_a.push_back (circumCircle (vertices_in, a, b, c));
    } // end FOR
  } // end FOR

  /* Copy any remaining open triangles to the closed list, and then
   * remove any triangles that share a vertex with the supertriangle,
   * building a list of triplets that represent triangles. */
  for (int i = static_cast<int> (open_a.size ()); i--;)
    closed_a.push_back (open_a[i]);

  for (int i = static_cast<int> (closed_a.size ()); i--;)
    if (closed_a[i].i < n && closed_a[i].j < n && closed_a[i].k < n)
    {
      result.push_back (closed_a[i].i);
      result.push_back (closed_a[i].j);
      result.push_back (closed_a[i].k);
    } // end IF

  return result;
}
