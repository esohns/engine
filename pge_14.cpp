#include "stdafx.h"

#define OLC_PGEX_FUI
#include "pge_14.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

RigidBody::RigidBody (const olc::vf2d& p, int _n, float _len, float _angle, float a, float m, float _e, float _sf, float _df, int _id)
 : position(p), n(_n), len(_len), angle(_angle), angular_velocity(a), mass(m), e(_e), sf(_sf), df(_df), id(_id)
{
  for (int i = 0; i < n; i++)
  {
      model.push_back({ cosf(2.0f * M_PI / n * i), sinf(2.0f * M_PI / n * i) });
  }
  vertices.resize(n);
  prev_vertices.resize(n);

  inv_mass = mass == 0.0f ? 0.0f : 1.0f / mass;
  I = mass * len * len / 12.0f;
  inv_I = I == 0.0f ? 0.0f : 1.0f / I;
}

void
RigidBody::Logic(float dt, bool is_debug)
{
  // Physics Logic
  if (!is_debug)
  {
      position += velocity * dt;
      angle += angular_velocity * dt;
  }

  for (int i = 0; i < n; i++)
  {
    // Rotation
    vertices[i] = {
        model[i].x * cosf(angle) - model[i].y * sinf(angle),
        model[i].x * sinf(angle) + model[i].y * cosf(angle)
    };

    // Scaling
    vertices[i] *= len;

    // Translation
    vertices[i] += position;
  } // end FOR

  if (!first_iter)
  {
    first_iter = true;
    prev_vertices = vertices;
  } // end IF
}

void
RigidBody::ApplyImpulse(const olc::vf2d& impulse, const olc::vf2d& contact)
{
  velocity += impulse * inv_mass * 1.0f; // v = (f/m) * dt
  angular_velocity += contact.cross(impulse) * inv_I * 1.0f; // w = (restoring_torque / I) * dt
}

std::pair<float, olc::vf2d>
RigidBody::SATOverlap (RigidBody& r1, RigidBody& r2)
{
  RigidBody* p1 = &r1, * p2 = &r2;

  float overlap = +INFINITY;
  olc::vf2d overlap_axis;

  bool is_overlap = false;

  for (int n = 0; n < 2; n++)
  {
    if (n == 1)
      std::swap(p1, p2);

    for (int i = 0; i < p1->n; i++)
    {
      int j = (i + 1) % p1->n;

      olc::vf2d edge_normal = (p1->vertices[j] - p1->vertices[i]).perp();

      float min_r1 = +INFINITY, max_r1 = -INFINITY;

      for (const olc::vf2d& p : p1->vertices)
      {
        float q = edge_normal.dot(p);
        min_r1 = std::fmin(min_r1, q);
        max_r1 = std::fmax(max_r1, q);
      } // end FOR

      float min_r2 = +INFINITY, max_r2 = -INFINITY;

      for (const olc::vf2d& p : p2->vertices)
      {
        float q = edge_normal.dot(p);
        min_r2 = std::fmin(min_r2, q);
        max_r2 = std::fmax(max_r2, q);
      } // end FOR

      if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
        return { 0.0f, { 0.0f, 0.0f } };

      float new_overlap = std::fmin(max_r1, max_r2) - std::fmax(min_r1, min_r2);
      if (new_overlap < overlap)
      {
          overlap = new_overlap;
          overlap_axis = edge_normal;
          is_overlap = true;
      } // end IF
    } // end FOR
  } // end FOR

  float overlap_len = overlap_axis.mag();
  overlap_axis /= overlap_len;
  overlap /= overlap_len;

  olc::vf2d displacement = r2.GetPosition() - r1.GetPosition();
  if (displacement.dot(overlap_axis) < 0.0f) overlap_axis *= -1.0f;

  return { overlap, overlap_axis };
}

void
RigidBody::Draw (olc::PixelGameEngine* pge, bool is_fill, float alpha)
{
  // Linearly interpolate position with respect to alpha
  std::vector<olc::vf2d> interpolated_vertices(n);
  for (uint32_t a = 0; a < vertices.size(); a++)
    interpolated_vertices[a] = alpha * prev_vertices[a] + (1.0f - alpha) * vertices[a];

  if (!is_fill)
  {
    for (int i = 0; i < n; i++)
    {
      int j = (i + 1) % n;
      pge->DrawLine(interpolated_vertices[i], interpolated_vertices[j], color * 1.2f);
    } // end FOR
  } // end IF
  else
  {
    for (int i = 0; i < n - 2; i++) {
        int j = (i + 1) % n,
            k = (i + 2) % n;

        pge->FillTriangle(interpolated_vertices[0], interpolated_vertices[j], interpolated_vertices[k], color);
    } // end FOR
  } // end ELSE
  prev_vertices = vertices; // Update vertices
}

bool
RigidBody::IsConstrained(const olc::vf2d& a, const olc::vf2d& b)
{
    return !(position.x + len < a.x || position.y + len < a.y || position.x > b.x + len || position.y > b.y + len);
}

bool
RigidBody::IsContainPoint (const olc::vf2d& p)
{
  for (int i = 0; i < n; i++)
  {
    int j = (i + 1) % n,
        k = (i + 2) % n;

    const olc::vf2d& ab = vertices[j] - vertices[0],
                      bc = vertices[k] - vertices[j],
                      ca = vertices[0] - vertices[k];

    const olc::vf2d ap = vertices[0] - p,
                    bp = vertices[j] - p,
                    cp = vertices[k] - p;

    if (ab.cross(ap) <= 0.0f && bc.cross(bp) <= 0.0f && ca.cross(cp) <= 0.0f) return true;
  } // end FOR

  return false;
}

// Manifold

Edge
Manifold::GetBestEdge(int index, float dir)
{
  RigidBody* r = !index ? a : b;
  const olc::vf2d& n = normal * dir;

  float m_dot = -INFINITY;
  int vertex = -1;

  for (int i = 0; i < r->n; i++)
  {
    float distance = r->GetVertex(i).dot(n);
    if (distance > m_dot)
    {
      m_dot = distance;
      vertex = i;
    } // end IF
  } // end FOR

  olc::vf2d far_vertex = r->GetVertex(vertex),
          right_vertex = r->GetVertex(vertex - 1 < 0 ? r->n - 1 : vertex - 1),
           left_vertex = r->GetVertex((vertex + 1) % r->n);

  olc::vf2d left_edge = (far_vertex - left_vertex),
            right_edge = (far_vertex - right_vertex);

  if (left_edge.dot(n) <= right_edge.dot(n))
    return { left_vertex, far_vertex, left_edge, far_vertex, r->GetPosition() };
  return { right_vertex, far_vertex, right_edge, far_vertex, r->GetPosition() };
}

void
Manifold::GetRefIncEdge()
{
  const auto& edge_a = GetBestEdge(0, 1.0f);
  const auto& edge_b = GetBestEdge(1, -1.0f);

  if (std::fabs(edge_a.edge.dot(normal)) <= std::fabs(edge_b.edge.dot(normal)))
  {
      // Reference edge is A
      ref = edge_a;
      inc = edge_b;
  } // end IF
  else
  {
      // Reference edge is B
      ref = edge_b;
      inc = edge_a;
  } // end ELSE
}

std::vector<olc::vf2d>
Manifold::Clip(const olc::vf2d& v1, const olc::vf2d& v2, const olc::vf2d& n, float o)
{
  std::vector<olc::vf2d> cp;
  float d1 = n.dot(v1) - o, // Signed distance between ref vertex and v1
      d2 = n.dot(v2) - o; // Signed distance between ref vertex and v2

  if (d1 >= 0.0f) cp.push_back(v1);
  if (d2 >= 0.0f) cp.push_back(v2);

  if (d1 * d2 < 0.0f) {
      // Linearly interpolate ref edge
      cp.push_back(v1 + d1 / (d1 - d2) * (v2 - v1));
  }

  return cp;
}

void
Manifold::GetContactPoints()
{
  GetRefIncEdge();
  const olc::vf2d& ref_edge = ref.edge.norm();

  auto cp1 = Clip(inc.v1, inc.v2, ref_edge, ref_edge.dot(ref.v1));
  if (cp1.size() < 2) return;

  auto cp2 = Clip(cp1[0], cp1[1], -ref_edge, -ref_edge.dot(ref.v2));
  if (cp2.size() < 2) return;

  olc::vf2d ref_norm = ref_edge.perp();
  if ((ref.poly_pos - inc.poly_pos).dot(ref_norm) < 0.0f) ref_norm *= -1.0f;

  float d = ref_norm.dot(ref.farthest);

  if (cp2[1].dot(ref_norm) - d < 0.0f) { cp2.erase(cp2.begin() + 1); }
  else if (cp2[0].dot(ref_norm) - d < 0.0f) { cp2.erase(cp2.begin() + 0); }

  points = cp2;
  for (const olc::vf2d& p : cp2)
    overlaps.push_back(p.dot(ref_norm) - d);
}

void
Manifold::ApplyForces(float dt)
{
  if (points.size() == 0)
    return;

  auto VectorProduct = [](float a, const olc::vf2d& v)
  {
      return olc::vf2d(v.y * a, v.x * -a);
  };

  for (const olc::vf2d& p : points)
  {
    const olc::vf2d& ra = (p - a->GetPosition()),
                     rb = (p - b->GetPosition());

    olc::vf2d va = VectorProduct(a->angular_velocity, ra),
              vb = VectorProduct(b->angular_velocity, rb);

    olc::vf2d rv = (b->GetVelocity() - vb) - (a->GetVelocity() - va);

    float rv_normal = rv.dot(normal);
    if (rv_normal > 0.0f)
      return;

    float ra_n = ra.cross(normal),
          rb_n = rb.cross(normal);

    float inv_mass_sum = (
        a->inv_mass + ra_n * ra_n * a->inv_I +
        b->inv_mass + rb_n * rb_n * b->inv_I
        );

    float e = std::fminf(a->e, b->e);
    float j = -(1.0f + e) * rv_normal / inv_mass_sum;
    j /= points.size();

    // Normal resolution
    olc::vf2d impulse = j * normal;
    a->ApplyImpulse(-impulse, ra);
    b->ApplyImpulse(impulse, rb);

    va = VectorProduct(a->angular_velocity, ra);
    vb = VectorProduct(b->angular_velocity, rb);

    rv = (b->GetVelocity() - vb) - (a->GetVelocity() - va);
    rv_normal = rv.dot(normal);

    const olc::vf2d& t = (rv - rv_normal * normal).norm(); // Tangent from triangle law with rv and rv_normal vector
    float jt = -t.dot(rv) / inv_mass_sum;
    jt /= points.size();

    if (std::fabs(jt - 0.0f) <= EPSILON) return;

    olc::vf2d friction_impulse;
    if (std::fabs(jt) <= j * sf) { friction_impulse = jt * t; }
    else { friction_impulse = -j * df * t; }

    a->ApplyImpulse(-friction_impulse, ra);
    b->ApplyImpulse(friction_impulse, rb);
  }
}

void
Manifold::PositionalCorrection()
{
  float overlap_value = 0.0f;
  for (const auto& o : overlaps) overlap_value += o;
  overlap_value /= overlaps.size();

  float p = 1.0f;
  const olc::vf2d& direction = std::fmaxf(overlap_value, 0.0f) * normal * p;

  float res = 1.0f; // Resolution percentage
  if (a->mass > 0.0f && b->mass > 0.0f) res = 0.5f;
  if (a->mass > 0.0f) a->Move(-direction * res);
  if (b->mass > 0.0f) b->Move(direction * res);
}

void
Manifold::Logic(float dt, int iter, bool is_debug)
{
    GetContactPoints();                                   // Get contact points
    if (!is_debug)
    {
        PositionalCorrection();                           // Static resolution
        for (int i = 0; i < iter; i++) ApplyForces(dt);   // Dynamic resolution
    }
}

// Constraint
// Apply restoring force on rope
void
Constraint::ApplyForces (RigidBody& rb, float dt, bool is_input)
{
  const olc::vf2d& direction = rb.GetPosition() - pivot_pos;

  float extension = direction.mag();

  olc::vf2d dir = direction / extension;

  const olc::vf2d& offset = (extension - len) * dir;
  const olc::vf2d& force = -k * offset - b * rb.GetVelocity();

  float inv_mass = is_input ? 1.0f : rb.inv_mass;

  if (!rb.is_input)
    rb.AddVelocity(force * inv_mass * dt);
}

void
Constraint::Update(RigidBody& rb, float dt)
{
  if (rb_id < 0)
    return;
  olc::vf2d direction = rb.GetPosition() - pivot_pos;
  float dir_mag2 = direction.mag2();

  if (dir_mag2 > len * len) { ApplyForces(rb, dt); }
  point_pos = rb.GetPosition();
}

void
Constraint::Draw(olc::PixelGameEngine* pge, olc::Pixel color)
{
  pge->DrawLine(pivot_pos, point_pos, color);
}

// Scene

void
Scene::Update(float dt, bool is_debug)
{
  // Simulation parameters
  float FPS = 60.0f,
        delay = 0.1f;
  float inv_FPS = 1.0f / FPS;

  std::vector<Manifold> manifolds;

  accumulator = std::fmin(accumulator + dt, delay);

  if (accumulator > inv_FPS)
  {
    accumulator -= inv_FPS;

    std::vector<int> shapeID;
    for (auto& c : constraints)
    {
        c.Update(shapes[c.GetID()], inv_FPS);
        shapeID.push_back(c.GetID());
    }

    for (int i = shapes.size() - 1; i >= 0; i--)
    {
      shapes[i].Logic(inv_FPS, is_debug);

      if (std::find(shapeID.begin(), shapeID.end(), i) != shapeID.end())
        continue;

      // Shapes in bounds
      if (!shapes[i].IsConstrained({ 0.0f, 0.0f }, { screen_size.x, screen_size.y }))
        shapes.erase(shapes.begin() + i);
    } // end FOR

    std::vector<Manifold> manifolds;
    for (uint32_t a = 0; a < shapes.size() - 1; a++)
      for (uint32_t b = a + 1; b < shapes.size(); b++)
      {
        const auto& manifold_data = RigidBody::SATOverlap(shapes[a], shapes[b]);
        if (manifold_data.first > 0.0f)
        {
          // Add to vector if overlap > 0
          manifolds.push_back(Manifold(&shapes[a], &shapes[b], manifold_data.first, manifold_data.second));
        } // end IF
      } // end FOR

    for (RigidBody& r : shapes) r.ApplyForce({ 0.0f, g }, inv_FPS);
    for (auto& m : manifolds) { m.Logic(dt, 5, is_debug); }

    for (int i = constraints.size() - 1; i >= 0; i--)
      if (constraints[i].GetID() < 0)
        constraints.erase(constraints.begin() + i);
  } // end IF

  alpha = accumulator / inv_FPS;
}

void
Scene::Draw (olc::PixelGameEngine* pge, bool is_fill)
{
  for (auto& s : shapes)
  {
      s.Draw(pge, true, 0.0f);
      s.Draw(pge, false, 0.0f);
  }
  for (auto& c : constraints)
    c.Draw(pge);
}

void
Scene::AddShape (const olc::vf2d& p, int n_vertices, float len, float angle, float angular_velocity, float mass, olc::Pixel color, float e, float sf, float df)
{
  RigidBody rb(p, n_vertices, len, angle, angular_velocity, mass, e, sf, df, shapes.size());
  rb.SetColor(color);
  shapes.push_back(rb);
}

void
Scene::AddConstraint (const olc::vf2d& p, float len, float k, float b, int index)
{
  Constraint c(p, len, k, b);
  if (index >= 0) c.Attach(index);
  constraints.push_back(c);
}

//////////////////////////////////////////

float
Sign(float value) { return value > 0.0f ? 1.0f : (value < 0.0f ? -1.0f : 0.0f); }

void
RigidBodyDraw::Initialize()
{
  gui.create_window("gui", "Physics Settings", { 0, 0 }, { 200, 125 });
  gui.set_active_window("gui");

  gui.add_int_slider("vertices", "Vertex ", { 80, 5 }, { 100, 10 }, { 3, 10 });
  gui.add_float_slider("angular_velocity", "AngleVel ", { 80, 20 }, { 100, 10 }, { 0.0f, 5.0f });
  gui.add_float_slider("mass", "Mass ", { 80, 35 }, { 100, 10 }, { 0.0f, 100.0f });
  gui.add_float_slider("restitution", "Restitution ", { 80, 50 }, { 100, 10 }, { 0.0f, 2.0f });
  gui.add_float_slider("static_friction", "sFriction ", { 80, 65 }, { 100, 10 }, { 0.0f, 5.0f });
  gui.add_float_slider("dynamic_friction", "dFriction ", { 80, 80 }, { 100, 10 }, { 0.0f, 5.0f });

  gui.find_element("vertices")->set_slider_value(n);
  gui.find_element("vertices")->set_text_color(olc::Pixel(0, 255, 0));
  gui.find_element("angular_velocity")->set_slider_value(angular_velocity);
  gui.find_element("angular_velocity")->set_text_color(olc::Pixel(0, 255, 50));
  gui.find_element("mass")->set_slider_value(mass);
  gui.find_element("mass")->set_text_color(olc::Pixel(0, 255, 100));
  gui.find_element("restitution")->set_slider_value(e);
  gui.find_element("restitution")->set_text_color(olc::Pixel(0, 255, 150));
  gui.find_element("static_friction")->set_slider_value(sf);
  gui.find_element("static_friction")->set_text_color(olc::Pixel(0, 255, 200));
  gui.find_element("dynamic_friction")->set_slider_value(df);
  gui.find_element("dynamic_friction")->set_text_color(olc::Pixel(0, 255, 255));
}

void
RigidBodyDraw::OnMousePress()
{
  model.resize(n);
  vertices.resize(n);

  for (int i = 0; i < n; i++)
    model[i] = { cosf(2.0f * M_PI / n * i + 0.25f * M_PI), sinf(2.0f * M_PI / n * i + 0.25f * M_PI) };
}

void
RigidBodyDraw::OnMouseInput (const olc::vf2d& m_pos, float m_wheel, int key)
{
  pos = m_pos;

  switch (key) {
  case 0: // A
    scale.x = std::fabs(std::fmin(scale.x + m_wheel, 1.0f));
    break;
  case 1: // W
    angle += m_wheel;
    break;
  case 2: // D
    scale.y = std::fabs(std::fmin(scale.y + m_wheel, 1.0f));
    break;
  case 3: // S
    len = std::fmax(len + Sign(m_wheel) * 5.0f, 1.0f);
    break;
  }

  is_preview = true;
}

void
RigidBodyDraw::OnMouseRelease(Scene& scene)
{
  RigidBody rb (pos, n, len, angle, 0.0f, 10.0f);

  for (auto& m : model) m *= scale;
  rb.SetModel(model);
  rb.SetColor(olc::Pixel(rand() % 256, rand() % 256, rand() % 256));

  scene.AddShape(rb);
  index++;

  is_preview = false;
}

void
RigidBodyDraw::Logic()
{
  if (df > sf)
    std::swap(sf, df);
  if (!is_preview)
    return;

  std::vector<olc::vf2d> rb_model = model;
  for (int i = 0; i < n; i++)
  {
    // Model scaling
    rb_model[i] *= scale;

    // Rotation
    vertices[i].x = rb_model[i].x * cosf(angle) - rb_model[i].y * sinf(angle);
    vertices[i].y = rb_model[i].x * sinf(angle) + rb_model[i].y * cosf(angle);

    // Scaling
    vertices[i] *= len;

    // Translation
    vertices[i] += pos;
  } // end FOR
}

void
RigidBodyDraw::PreviewRender (olc::PixelGameEngine* pge)
{
  gui.run();

  n = gui.find_element("vertices")->get_slider_value<int>();
  angular_velocity = gui.find_element("angular_velocity")->get_slider_value<float>();
  mass = gui.find_element("mass")->get_slider_value<float>();
  e = gui.find_element("restitution")->get_slider_value<float>();
  sf = gui.find_element("static_friction")->get_slider_value<float>();
  df = gui.find_element("dynamic_friction")->get_slider_value<float>();

  if (!is_preview)
    return;

  for (int i = 0; i < n; i++)
  {
    int j = (i + 1) % n;
    pge->DrawLine(vertices[i], vertices[j]);
  } // end FOR
}

//////////////////////////////////////////

PGE_14::PGE_14 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 14";
}

PGE_14::~PGE_14 ()
{
}

bool
PGE_14::OnUserCreate ()
{
  rb_draw.Initialize();

  scene.Initialize({ ScreenWidth() * 1.0f, ScreenHeight() * 1.0f });

  scene.AddShape({ ScreenWidth() * 0.5f, ScreenHeight() * 1.25f }, 4, ScreenWidth() * 0.5f, M_PI / 4.0f, 0.0f, 0.0f);
  scene.AddShape({ 0.0f, 0.0f }, 6, 15.0f, 0.0f, 0.0f, 10.0f, olc::GREEN);
  scene.AddShape({ ScreenWidth() * 1.0f, 0.0f }, 3, 20.0f, 0.0f, 0.0f, 2.0f, olc::MAGENTA);

  Constraint c1({ ScreenWidth() * 0.25f, ScreenHeight() * 0.25f }, 100.0f, 0.8f, 0.2f, false);
  c1.Attach(1);
  scene.AddConstraint(c1);

  Constraint c2({ ScreenWidth() * 0.75f, ScreenHeight() * 0.25f }, 50.0f, 0.8f, 0.4f, false);
  c2.Attach(2);
  scene.AddConstraint(c2);

  return true;
}

bool
PGE_14::OnUserUpdate (float fElapsedTime)
{
  const olc::vf2d& m_pos = GetMousePos() * 1.0f;

  // Input
  if (GetKey(olc::Z).bPressed)
  {
    scene.AddShape(
      m_pos,                                                  // Position
      3 + rand() % 3,                                         // Vertices
      10.0f + rand() % 10,                                    // Edge length
      0.0f,                                                   // Initial angle
      0.0f,                                                   // Angular velocity
      Random(2.0f, 10.0f),                                    // Mass
      olc::Pixel(rand() % 256, rand() % 256, rand() % 256),   // Color
      Random(0.2f, 0.6f),                                     // Coefficient of restitution
      0.4f,                                                   // Coefficient of static friction
      0.2f                                                    // Coefficient of dynamic friction
    );
  } // end IF

  bool is_shift = GetKey(olc::SHIFT).bHeld;

  int key = 0;
  if (GetKey(olc::A).bHeld) key = 0;
  else if (GetKey(olc::W).bHeld) key = 1;
  else if (GetKey(olc::D).bHeld) key = 2;
  else if (GetKey(olc::S).bHeld) key = 3;

  if (!is_shift)
  {
    if (GetMouse(1).bPressed)
      rb_draw.OnMousePress();
    if (GetMouse(1).bHeld)
      rb_draw.OnMouseInput(m_pos, Sign(GetMouseWheel()) * 5.0f * fElapsedTime, key);
    if (GetMouse(1).bReleased)
      rb_draw.OnMouseRelease(scene);
  } // end IF

  if (GetMouse(0).bPressed)
  {
    for (auto& c : scene.GetConstraints())
    {
      RigidBody& shape = scene.GetShape(c.GetID());
      if (shape.IsContainPoint(m_pos))
      {
        selected_shape = &shape;
        selected_constraint = &c;
        shape.is_input = true;
      } // end IF
    } // end FOR
  } // end IF

  if (selected_constraint != nullptr)
  {
    if (GetMouse(0).bHeld)
      selected_shape->SetPosition(m_pos);
    else if (GetMouse(0).bReleased)
    {
      selected_shape->is_input = false;
      selected_constraint->ApplyForces(*selected_shape, 1.0f, true);
      selected_shape = nullptr;
      selected_constraint = nullptr;
    } // end ELSE IF
  } // end IF

  // Logic
  rb_draw.Logic();
  for (int i = 0; i < n_iter; i++)
    scene.Update(fElapsedTime, false);

  // Draw
  Clear(olc::BLACK);
  scene.Draw(this, true);
  rb_draw.PreviewRender(this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
