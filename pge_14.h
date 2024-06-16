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
class RigidBody
{
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
  RigidBody () {}
  RigidBody (const olc::vf2d&, int, float, float, float, float, float = 0.1f, float = 0.8f, float = 0.4f, int = 0);

  void Logic (float, bool = false);
  void ApplyImpulse (const olc::vf2d&, const olc::vf2d&);

  bool IsConstrained (const olc::vf2d&, const olc::vf2d&); // Is the polygon within bounds
  bool IsContainPoint (const olc::vf2d&); // Is the given point within polygon bounds

  void Draw (olc::PixelGameEngine*, bool = false, float = 0.0f);

  static std::pair<float, olc::vf2d> SATOverlap (RigidBody&, RigidBody&);

  inline void ApplyForce (const olc::vf2d& force, float dt) { velocity += force * inv_mass * dt; }

  inline void SetModel (const std::vector<olc::vf2d>& m) { model = m; }
  inline void SetColor (const olc::Pixel& c) { color = c; }

  inline void Move (const olc::vf2d& o) { position += o; }
  inline void SetPosition (const olc::vf2d& p) { position = p; }
  inline const olc::vf2d& GetPosition () const { return position; }

  inline void AddVelocity (const olc::vf2d& _v) { velocity += _v; }
  inline void SetVelocity (const olc::vf2d& v) { velocity = v; }
  inline const olc::vf2d& GetVelocity () const { return velocity; }

  inline const int& GetID () const { return id; }

  inline const olc::vf2d& GetVertex (int i) const { return vertices[i]; }
};

struct Edge { olc::vf2d v1, v2, edge, farthest, poly_pos; };

class Manifold
{
 private:
  RigidBody* a = nullptr, *b = nullptr;
  float overlap = 0.0f;
  olc::vf2d normal;
  std::vector<olc::vf2d> points; // Contacts
  std::vector<float> overlaps; // Overlaps
  Edge ref, inc;
  float sf = 0.0f, df = 0.0f;

 public:
  Manifold () {}
  Manifold (RigidBody* _a, RigidBody* _b, float _overlap, const olc::vf2d& _normal)
   : a (_a)
   , b (_b)
   , overlap (_overlap)
   , normal (_normal)
  {
    sf = std::sqrt (a->sf * b->sf);
    df = std::sqrt (a->df * b->df);
  }

  Edge GetBestEdge (int, float);
  void GetRefIncEdge ();
  void GetContactPoints ();
  std::vector<olc::vf2d> Clip (const olc::vf2d&, const olc::vf2d&, const olc::vf2d&, float);
  void ApplyForces (float);
  void PositionalCorrection ();
  void Logic (float, int = 1, bool = false);
};

class Constraint
{
 private:
  olc::vf2d pivot_pos, point_pos;
  float len = 0.0f, k = 0.8f, b = 0.2f;
  int rb_id = -1;
  bool is_sling = false;

 public:
  Constraint () {}
  Constraint (const olc::vf2d& p, float _len, float _k, float _b, bool _is_sling = false)
   : pivot_pos (p)
   , len (_len)
   , k (_k)
   , b (_b)
   , is_sling (_is_sling)
  {}

  void ApplyForces (RigidBody&, float, bool = false);
  void Update (RigidBody&, float);
  void Draw (olc::PixelGameEngine*, olc::Pixel = olc::WHITE);
  inline void SetPivotPosition (const olc::vf2d& p) { pivot_pos = p; }
  inline const olc::vf2d& GetPivotPosition () const { return pivot_pos; }
  inline void Attach (int id) { rb_id = id; }
  inline const int& GetID () const { return rb_id; }
  inline void Reset () { rb_id = -1; }
  inline const bool& IsSling () const { return is_sling; }
};

class Scene
{
 private:
  std::vector<RigidBody> shapes;
  std::vector<Constraint> constraints;
  float accumulator = 0.0f, delay = 0.1f, inv_FPS = 1.0f / 60.0f, alpha = 0.0f;
  olc::vf2d screen_size;

 public:
  Scene () {}
  inline void Initialize (const olc::vf2d& _screen_size) { screen_size = _screen_size; }
  void Update (float, bool = false);
  void Draw (olc::PixelGameEngine*, bool = false);
  void AddShape (const olc::vf2d&, int, float, float, float, float, olc::Pixel = olc::WHITE, float = 0.1f, float = 0.8f, float = 0.4f);
  inline void AddShape (const RigidBody& rb) { shapes.push_back (rb); }
  void AddConstraint (const olc::vf2d&, float, float, float, int);
  inline void AddConstraint (const Constraint& c) { constraints.push_back (c); }
  inline std::vector<RigidBody>& GetShapes () { return shapes; }
  inline std::vector<Constraint>& GetConstraints () { return constraints; }
  inline RigidBody& GetShape (int index) { return shapes[index]; }
  inline Constraint& GetConstraint (int index) { return constraints[index]; }
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
  void OnMouseInput (const olc::vf2d&, float, int);
  void OnMouseRelease (Scene&);
  void Logic ();
  void PreviewRender (olc::PixelGameEngine*);
};

class PGE_14
 : public olc::PixelGameEngine
{
 public:
  PGE_14 ();
  inline ~PGE_14 () {}

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
