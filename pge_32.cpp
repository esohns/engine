#include "stdafx.h"

#include "pge_32.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_2.h"
#include "engine_common.h"

b2ParticleColor PGE_32::k_ParticleColors[] = {
  b2ParticleColor(0xff, 0x00, 0x00, 0xff), // red
  b2ParticleColor(0x00, 0xff, 0x00, 0xff), // green
  b2ParticleColor(0x00, 0x00, 0xff, 0xff), // blue
  b2ParticleColor(0xff, 0x8c, 0x00, 0xff), // orange
  b2ParticleColor(0x00, 0xce, 0xd1, 0xff), // turquoise
  b2ParticleColor(0xff, 0x00, 0xff, 0xff), // magenta
  b2ParticleColor(0xff, 0xd7, 0x00, 0xff), // gold
  b2ParticleColor(0x00, 0xff, 0xff, 0xff), // cyan
};
uint32 PGE_32::k_ParticleColorsCount =
B2_ARRAY_SIZE(PGE_32::k_ParticleColors);

//////////////////////////////////////////

PGE_32::PGE_32 (Mode mode_in)
 : b2Draw ()
 , olc::PixelGameEngine ()
 , mode_ (mode_in)
 , world_ (NULL)
 , particleSystem_ (NULL)
 , ground_ (NULL)
 , dimension_ (0.0f)
 , mouseJoint_ (NULL)
 , mouseWorld_ ()
 , mouseTracing_ (false)
 , mouseTracerPosition_ ()
 , mouseTracerVelocity_ ()
 , joint_ (NULL)
 , time_ (0.0f)
 , bodies_ ()
 , joints_ ()
{
  sAppName = "Example 32";

  switch (mode_)
  {
    case MODE_WAVEMACHINE:
      dimension_ = ENGINE_PGE_32_DEFAULT_DIM_WAVEMACHINE;
      break;
    case MODE_WEB:
      dimension_ = ENGINE_PGE_32_DEFAULT_DIM_WEB;
      ACE_OS::memset (bodies_, 0, sizeof (b2Body*[4]));
      ACE_OS::memset (joints_, 0, sizeof (b2Joint*[8]));
      break;
    default:
      break;
  }
}

PGE_32::~PGE_32 ()
{
  switch (mode_)
  {
    case MODE_WAVEMACHINE:
    {
      if (joint_ && world_)
        world_->DestroyJoint(joint_);
      break;
    }
    case MODE_WEB:
    {
      if (world_)
      {
        for (int i = 0; i < 8; i++)
          world_->DestroyJoint (joints_[i]);
        for (int i = 0; i < 4; i++)
          world_->DestroyBody (bodies_[i]);
      } // end IF
      break;
    }
    default:
      break;
  }

  if (mouseJoint_ && world_)
    world_->DestroyJoint (mouseJoint_);
  if (ground_ && world_)
    world_->DestroyBody (ground_);

  if (particleSystem_ && world_)
    world_->DestroyParticleSystem (particleSystem_);

  delete world_;
}

void
PGE_32::DrawPolygon (const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  float x1, x2, y1, y2;

  for (int32 i = 0; i < vertexCount; ++i)
  {
    if (i == vertexCount - 1)
      break;

    x1 =
      Common_GL_Tools::map (vertices[i].x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
    y1 =
      Common_GL_Tools::map (vertices[i].y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
    x2 =
      Common_GL_Tools::map (vertices[i + 1].x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
    y2 =
      Common_GL_Tools::map (vertices[i + 1].y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x1), static_cast<int32_t> (y1),
                                    static_cast<int32_t> (x2), static_cast<int32_t> (y2),
                                    {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255},
                                    0xFFFFFFFF);
  } // end FOR
  x1 =
    Common_GL_Tools::map (vertices[vertexCount - 1].x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
  y1 =
    Common_GL_Tools::map (vertices[vertexCount - 1].y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
  x2 =
    Common_GL_Tools::map (vertices[0].x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
  y2 =
    Common_GL_Tools::map (vertices[0].y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
  olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x1), static_cast<int32_t> (y1),
                                  static_cast<int32_t> (x2), static_cast<int32_t> (y2),
                                  {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255},
                                  0xFFFFFFFF);
}

void
PGE_32::DrawSolidPolygon (const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  DrawPolygon (vertices, vertexCount, color);
  // *TODO*: flood-fill ?
}

void
PGE_32::DrawCircle (const b2Vec2& center, float32 radius, const b2Color& color)
{
  float x, y;
  x =
    Common_GL_Tools::map (center.x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y =
    Common_GL_Tools::map (center.y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
  olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                    static_cast<int32_t> (radius),
                                    {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255},
                                    0xFF);
}

void
PGE_32::DrawSolidCircle (const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
  float x, y;
  x =
    Common_GL_Tools::map (center.x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y =
    Common_GL_Tools::map (center.y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
  olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                    static_cast<int32_t> (radius),
                                    {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255});
}

void
PGE_32::DrawParticles (const b2Vec2* centers, float32 radius, const b2ParticleColor* colors, int32 count)
{
  float x, y;

  for (int32 i = 0; i < count; ++i)
  {
    x =
      Common_GL_Tools::map (centers[i].x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
    y =
      Common_GL_Tools::map (centers[i].y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
    olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                      static_cast<int32_t> (radius),
                                      colors ? olc::Pixel (static_cast<uint8_t> (colors[i].r * 255.0f), static_cast<uint8_t> (colors[i].g * 255.0f), static_cast<uint8_t> (colors[i].b * 255.0f), 255) : olc::WHITE,
                                      0xFF);
  } // end FOR
}

void
PGE_32::DrawSegment (const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
  float x1, y1, x2, y2;
  x1 =
    Common_GL_Tools::map (p1.x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y1 =
    Common_GL_Tools::map (p1.y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
  x2 =
    Common_GL_Tools::map (p2.x, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y2 =
    Common_GL_Tools::map (p2.y, -dimension_, dimension_, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
  olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x1), static_cast<int32_t> (y1),
                                  static_cast<int32_t> (x2), static_cast<int32_t> (y2),
                                  {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255},
                                  0xFFFFFFFF);
}

void
PGE_32::DrawTransform (const b2Transform& xf)
{
  b2Vec2 p1 = xf.p, p2;
  static float32 k_axisScale_x = 0.4f;
  p2 = p1 + k_axisScale_x * xf.q.GetXAxis ();
  DrawSegment (p1, p2, b2Color (1, 0, 0));

  static float32 k_axisScale_y = 0.4f;
  p2 = p1 + k_axisScale_y * xf.q.GetYAxis ();
  DrawSegment (p1, p2, b2Color (0, 1, 0));
}

bool
PGE_32::OnUserCreate ()
{
  uint32 flags = 0;
  flags += b2Draw::e_shapeBit;
  //flags += b2Draw::e_aabbBit;
  //flags += b2Draw::e_pairBit;
  //flags += b2Draw::e_centerOfMassBit;
  flags += b2Draw::e_particleBit;

  b2Vec2 gravity;
  gravity.Set (0.0f, 10.0f);
  world_ = new b2World (gravity);
  world_->SetDebugDraw (this);

  b2ParticleSystemDef particleSystemDef;
  particleSystem_ = world_->CreateParticleSystem (&particleSystemDef);
  particleSystem_->SetGravityScale (0.4f);
  particleSystem_->SetDensity (1.2f);
  particleSystem_->SetRadius (0.025f);
  particleSystem_->SetDamping (0.2f);

  b2BodyDef bd;
  ground_ = world_->CreateBody (&bd);

  // initialize scene
  switch (mode_)
  {
    case MODE_WAVEMACHINE:
      initializeWaveMachine ();
      break;
    case MODE_WEB:
      flags += b2Draw::e_jointBit;
      initializeWeb ();
      break;
    default:
      break;
  } // end SWITCH
  b2Draw::SetFlags (flags);

  return true;
}

bool
PGE_32::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static float32 time_step_f =
    ENGINE_PGE_32_DEFAULT_HZ > 0.0f ? 1.0f / ENGINE_PGE_32_DEFAULT_HZ : float32(0.0f);

  // handle mouse interaction
  olc::HWButton left_button_s = olc::PixelGameEngine::GetMouse (0);
  if (left_button_s.bPressed)
  {
    mouseTracing_ = true;
    float32 x =
      Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1), -dimension_, dimension_);
    float32 y =
      Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseY ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1), -dimension_, dimension_);
    mouseWorld_.Set (x, y);
    mouseTracerPosition_ = mouseWorld_;
    mouseTracerVelocity_ = b2Vec2_zero;
  } // end IF
  else if (left_button_s.bHeld)
  {
    if (mouseJoint_)
    {
      float32 x =
        Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1), -dimension_, dimension_);
      float32 y =
        Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseY ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1), -dimension_, dimension_);
      mouseJoint_->SetTarget (b2Vec2 (x, y));
      goto continue_;
    } // end IF

    b2AABB aabb;
    b2Vec2 d;
    d.Set (0.001f, 0.001f);
    aabb.lowerBound = mouseWorld_ - d;
    aabb.upperBound = mouseWorld_ + d;

    QueryCallback callback (mouseWorld_);
    world_->QueryAABB (&callback, aabb);

    if (callback.fixture_)
    {
      b2Body* body = callback.fixture_->GetBody ();
      b2MouseJointDef md;
      md.bodyA = ground_;
      md.bodyB = body;
      md.target = mouseWorld_;
      md.maxForce = 1000.0f * body->GetMass();
      mouseJoint_ = (b2MouseJoint*)world_->CreateJoint (&md);
      body->SetAwake (true);
    } // end IF
  } // end IF
  else if (left_button_s.bReleased)
  {
    mouseTracing_ = false;

    if (mouseJoint_)
    {
      world_->DestroyJoint (mouseJoint_); mouseJoint_ = NULL;
    } // end IF
  } // end ELSE IF

  if (mouseTracing_ && !mouseJoint_)
  {
    float32 delay = 0.1f;
    b2Vec2 acceleration = 2 / delay * (1 / delay * (mouseWorld_ - mouseTracerPosition_) - mouseTracerVelocity_);
    mouseTracerVelocity_ += time_step_f * acceleration;
    mouseTracerPosition_ += time_step_f * mouseTracerVelocity_;
    b2CircleShape shape;
    shape.m_p = mouseTracerPosition_;
    shape.m_radius = 2 * 1.0f;
    QueryCallback2 callback (particleSystem_, &shape, mouseTracerVelocity_);
    b2AABB aabb;
    b2Transform xf;
    xf.SetIdentity ();
    shape.ComputeAABB (&aabb, xf, 0);
    world_->QueryAABB (&callback, aabb);
  } // end IF

  if (mouseJoint_)
  {
    b2Vec2 p1 = mouseJoint_->GetAnchorB ();
    b2Vec2 p2 = mouseJoint_->GetTarget ();

    b2Color c;
    c.Set (0.0f, 1.0f, 0.0f);
    DrawCircle (p1, 4.0f, c);
    DrawCircle (p2, 4.0f, c);

    c.Set (0.8f, 0.8f, 0.8f);
    DrawSegment (p1, p2, c);
  } // end IF

continue_:
  if (joint_)
  {
    if (ENGINE_PGE_32_DEFAULT_HZ > 0.0f)
      time_ += 1 / ENGINE_PGE_32_DEFAULT_HZ;
    joint_->SetMotorSpeed (0.05f * cosf (time_) * b2_pi);
  } // end IF

  world_->Step (time_step_f,
                ENGINE_PGE_32_DEFAULT_VEL_ITERATIONS,
                ENGINE_PGE_32_DEFAULT_POS_ITERATIONS);
  world_->DrawDebugData ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_32::initializeWaveMachine ()
{
  {
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.allowSleep = false;
    bd.position.Set (0.0f, 1.0f);
    b2Body* body = world_->CreateBody (&bd);

    b2PolygonShape shape;
    shape.SetAsBox (0.05f, 1.0f, b2Vec2 ( 2.0f,  0.0f), 0.0);
    body->CreateFixture (&shape, 5.0f);
    shape.SetAsBox (0.05f, 1.0f, b2Vec2 (-2.0f,  0.0f), 0.0);
    body->CreateFixture (&shape, 5.0f);
    shape.SetAsBox (2.0f, 0.05f, b2Vec2 ( 0.0f,  1.0f), 0.0);
    body->CreateFixture (&shape, 5.0f);
    shape.SetAsBox (2.0f, 0.05f, b2Vec2 ( 0.0f, -1.0f), 0.0);
    body->CreateFixture (&shape, 5.0f);

    b2RevoluteJointDef jd;
    jd.bodyA = ground_;
    jd.bodyB = body;
    jd.localAnchorA.Set (0.0f, 1.0f);
    jd.localAnchorB.Set (0.0f, 0.0f);
    jd.referenceAngle = 0.0f;
    jd.motorSpeed = 0.05f * b2_pi;
    jd.maxMotorTorque = 1e7f;
    jd.enableMotor = true;
    joint_ = (b2RevoluteJoint*)world_->CreateJoint (&jd);
  }

  {
    b2ParticleGroupDef pd;
    //pd.flags = b2_colorMixingParticle;

    b2PolygonShape shape;
    shape.SetAsBox (0.9f, 0.9f, b2Vec2 (0.0f, 1.0f), 0.0);

    pd.shape = &shape;
    b2ParticleGroup* group = particleSystem_->CreateParticleGroup (pd);

    //b2Assert (group);
    //b2ParticleColor* colorBuffer = particleSystem_->GetColorBuffer ();
    //uint32 particlesPerColor = group->GetParticleCount () / k_ParticleColorsCount;
    //if (!particlesPerColor)
    //  particlesPerColor = 1;
    //for (int32 i = group->GetBufferIndex (); i < group->GetParticleCount () + group->GetBufferIndex (); i++)
	   // colorBuffer[i] = k_ParticleColors[i / particlesPerColor];
  }
}

void
PGE_32::initializeWeb ()
{
  {
    b2EdgeShape shape;
    shape.Set (b2Vec2 (-40.0f, 0.0f), b2Vec2 (40.0f, 0.0f));
    ground_->CreateFixture (&shape, 0.0f);
  }

  {
    b2PolygonShape shape;
    shape.SetAsBox(0.5f, 0.5f);

    b2BodyDef bd;
    bd.type = b2_dynamicBody;

    bd.position.Set (-5.0f, 5.0f);
    bodies_[0] = world_->CreateBody (&bd);
    bodies_[0]->CreateFixture (&shape, 5.0f);

    bd.position.Set (5.0f, 5.0f);
    bodies_[1] = world_->CreateBody (&bd);
    bodies_[1]->CreateFixture (&shape, 5.0f);

    bd.position.Set (5.0f, 15.0f);
    bodies_[2] = world_->CreateBody (&bd);
    bodies_[2]->CreateFixture (&shape, 5.0f);

    bd.position.Set (-5.0f, 15.0f);
    bodies_[3] = world_->CreateBody (&bd);
    bodies_[3]->CreateFixture (&shape, 5.0f);

    b2DistanceJointDef jd;
    b2Vec2 p1, p2, d;

    jd.frequencyHz = 2.0f;
    jd.dampingRatio = 0.0f;

    jd.bodyA = ground_;
    jd.bodyB = bodies_[0];
    jd.localAnchorA.Set (-10.0f, 0.0f);
    jd.localAnchorB.Set (-0.5f, -0.5f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[0] = world_->CreateJoint (&jd);

    jd.bodyA = ground_;
    jd.bodyB = bodies_[1];
    jd.localAnchorA.Set (10.0f, 0.0f);
    jd.localAnchorB.Set (0.5f, -0.5f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[1] = world_->CreateJoint (&jd);

    jd.bodyA = ground_;
    jd.bodyB = bodies_[2];
    jd.localAnchorA.Set (10.0f, 20.0f);
    jd.localAnchorB.Set (0.5f, 0.5f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[2] = world_->CreateJoint (&jd);

    jd.bodyA = ground_;
    jd.bodyB = bodies_[3];
    jd.localAnchorA.Set (-10.0f, 20.0f);
    jd.localAnchorB.Set (-0.5f, 0.5f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[3] = world_->CreateJoint (&jd);

    jd.bodyA = bodies_[0];
    jd.bodyB = bodies_[1];
    jd.localAnchorA.Set (0.5f, 0.0f);
    jd.localAnchorB.Set (-0.5f, 0.0f);;
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[4] = world_->CreateJoint (&jd);

    jd.bodyA = bodies_[1];
    jd.bodyB = bodies_[2];
    jd.localAnchorA.Set (0.0f, 0.5f);
    jd.localAnchorB.Set (0.0f, -0.5f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[5] = world_->CreateJoint (&jd);

    jd.bodyA = bodies_[2];
    jd.bodyB = bodies_[3];
    jd.localAnchorA.Set (-0.5f, 0.0f);
    jd.localAnchorB.Set (0.5f, 0.0f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[6] = world_->CreateJoint (&jd);

    jd.bodyA = bodies_[3];
    jd.bodyB = bodies_[0];
    jd.localAnchorA.Set (0.0f, -0.5f);
    jd.localAnchorB.Set (0.0f, 0.5f);
    p1 = jd.bodyA->GetWorldPoint (jd.localAnchorA);
    p2 = jd.bodyB->GetWorldPoint (jd.localAnchorB);
    d = p2 - p1;
    jd.length = d.Length ();
    joints_[7] = world_->CreateJoint (&jd);
  }
}
