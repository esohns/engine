#ifndef PGE_14_H
#define PGE_14_H

#include <random>
#include <vector>

#include "olcPixelGameEngine.h"
#include "olcPGEX_FrostUI.h"

#include "ace/Basic_Types.h"

#include "defines.h"

constexpr float g = 10.0f;
constexpr float EPSILON = 0.0001f;

// Class Declarations
class RigidBody {
 private:
  olc::vf2d position, velocity;
  std::vector<olc::vf2d> vertices, prev_vertices, model;
  float angle = 0.0f;
  bool first_iter = false;
  int id = 0;

  olc::Pixel color = olc::WHITE;

 public:
  int n = 0; // Vertices
  float angular_velocity = 0.0f;
  float mass = 0.0f, inv_mass = 0.0f;
  float I = 0.0f, inv_I = 0.0f; // Moment of inertia
  float e = 0.1f; // Coefficient of restitution
  float sf = 0.8f, df = 0.4f; // Coefficient of static friction and dynamic friction
  float len = 0.0f;

  bool is_input = false; // Is the rigid body held by mouse

 public:
  RigidBody() {}
  RigidBody(const olc::vf2d& p, int _n, float _len, float _angle, float a, float m, float _e = 0.1f, float _sf = 0.8f, float _df = 0.4f, int _id = 0);

  void Logic(float dt, bool is_debug = false);
  void ApplyImpulse(const olc::vf2d& impulse, const olc::vf2d& contact);

  bool IsConstrained(const olc::vf2d& a, const olc::vf2d& b); // Is the polygon within bounds
  bool IsContainPoint(const olc::vf2d& p); // Is the given point within polygon bounds

  void Draw(olc::PixelGameEngine* pge, bool is_fill = false, float alpha = 0.0f);

  static std::pair<float, olc::vf2d> SATOverlap(RigidBody& a, RigidBody& b);

  void ApplyForce(const olc::vf2d& force, float dt) { velocity += force * inv_mass * dt; }

  void SetModel(const std::vector<olc::vf2d>& m) { model = m; }
  void SetColor(const olc::Pixel& c) { color = c; }

  void Move(const olc::vf2d& o) { position += o; }
  void SetPosition(const olc::vf2d& p) { position = p; }
  const olc::vf2d& GetPosition() const { return position; }

  void AddVelocity(const olc::vf2d& _v) { velocity += _v; }
  void SetVelocity(const olc::vf2d& v) { velocity = v; }
  const olc::vf2d& GetVelocity() const { return velocity; }

  const int& GetID() const { return id; }

  const olc::vf2d& GetVertex(int i) const { return vertices[i]; }
};

struct Edge { olc::vf2d v1, v2, edge, farthest, poly_pos; };

class Manifold
{
 private:
  RigidBody* a = nullptr,
      * b = nullptr;

  float overlap = 0.0f;
  olc::vf2d normal;
  std::vector<olc::vf2d> points; // Contacts
  std::vector<float> overlaps; // Overlaps
  Edge ref, inc;

  float sf = 0.0f, df = 0.0f;

 public:
  Manifold() {}
  Manifold(RigidBody* _a, RigidBody* _b, float _overlap, const olc::vf2d& _normal)
      : a(_a), b(_b), overlap(_overlap), normal(_normal) {
      sf = std::sqrt(a->sf * b->sf);
      df = std::sqrt(a->df * b->df);
  }

  Edge GetBestEdge(int index, float dir);
  void GetRefIncEdge();
  void GetContactPoints();
  std::vector<olc::vf2d> Clip(const olc::vf2d& v1, const olc::vf2d& v2, const olc::vf2d& n, float o);

  void ApplyForces(float dt);
  void PositionalCorrection();

  void Logic(float dt, int iter = 1, bool is_debug = false);
};

class Constraint
{
 private:
  olc::vf2d pivot_pos, point_pos;
  float len = 0.0f, k = 0.8f, b = 0.2f;

  int rb_id = -1;
  bool is_sling = false;

 public:
  Constraint() {}
  Constraint(const olc::vf2d& p, float _len, float _k, float _b, bool _is_sling = false)
      : pivot_pos(p), len(_len), k(_k), b(_b), is_sling(_is_sling) {}

  void ApplyForces(RigidBody& rb, float dt, bool is_input = false);
  void Update(RigidBody& rb, float dt);

  void Draw(olc::PixelGameEngine* pge, olc::Pixel color = olc::WHITE);

  void SetPivotPosition(const olc::vf2d& p) { pivot_pos = p; }
  const olc::vf2d& GetPivotPosition() const { return pivot_pos; }

  void Attach(int id) { rb_id = id; }
  const int& GetID() const { return rb_id; }
  void Reset() { rb_id = -1; }

  const bool& IsSling() const { return is_sling; }
};

class Scene
{
 private:
  std::vector<RigidBody> shapes;
  std::vector<Constraint> constraints;

  float accumulator = 0.0f,
      delay = 0.1f,
      inv_FPS = 1.0f / 60.0f,
      alpha = 0.0f;

  olc::vf2d screen_size;

 public:
  Scene() {}
  void Initialize(const olc::vf2d& _screen_size) { screen_size = _screen_size; }

  void Update(float dt, bool is_debug = false);
  void Draw(olc::PixelGameEngine* pge, bool is_fill = false);

  void AddShape(const olc::vf2d& p, int n_vertices, float len, float angle, float angular_velocity, float mass, olc::Pixel color = olc::WHITE, float e = 0.1f, float sf = 0.8f, float df = 0.4f);
  void AddShape(const RigidBody& rb) { shapes.push_back(rb); }

  void AddConstraint(const olc::vf2d& p, float len, float k, float b, int index);
  void AddConstraint(const Constraint& c) { constraints.push_back(c); }

  std::vector<RigidBody>& GetShapes() { return shapes; }
  std::vector<Constraint>& GetConstraints() { return constraints; }

  RigidBody& GetShape(int index) { return shapes[index]; }
  Constraint& GetConstraint(int index) { return constraints[index]; }
};

class RigidBodyDraw
{
 private:
  olc::vf2d pos, scale = { 1.0f, 1.0f };
  float len = 10.0f, angle = 0.0f;

  std::vector<olc::vf2d> model, vertices;

  olc::FrostUI gui;

  int n = 4;
  float angular_velocity = 0.0f, mass = 1.0f;
  float e = 0.2f, sf = 0.8f, df = 0.4f;

  bool is_preview = false;
 
 public:
  int index = 0;
  void Initialize ();

  void OnMousePress();
  void OnMouseInput(const olc::vf2d&, float, int);
  void OnMouseRelease(Scene&);

  void Logic();

  void PreviewRender(olc::PixelGameEngine*);
};

class PGE_14
 : public olc::PixelGameEngine
{
 public:
  PGE_14 ();
  ~PGE_14 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  Scene scene;
  RigidBodyDraw rb_draw;
  int n_iter = 5;

  RigidBody* selected_shape = nullptr;
  Constraint* selected_constraint = nullptr;

  static float Random (float a, float b)
  {
    std::random_device rd;
    static std::mt19937 m (rd ());
    std::uniform_real_distribution<> dist (a, b);

    return static_cast<float> (dist (m));
  };
};

#endif // PGE_14_H
