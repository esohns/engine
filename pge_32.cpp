#include "stdafx.h"

#include "pge_32.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines.h"
#include "engine_common.h"

const b2ParticleColor PGE_32::k_ParticleColors[] = {
  b2ParticleColor(0xff, 0x00, 0x00, 0xff), // red
  b2ParticleColor(0x00, 0xff, 0x00, 0xff), // green
  b2ParticleColor(0x00, 0x00, 0xff, 0xff), // blue
  b2ParticleColor(0xff, 0x8c, 0x00, 0xff), // orange
  b2ParticleColor(0x00, 0xce, 0xd1, 0xff), // turquoise
  b2ParticleColor(0xff, 0x00, 0xff, 0xff), // magenta
  b2ParticleColor(0xff, 0xd7, 0x00, 0xff), // gold
  b2ParticleColor(0x00, 0xff, 0xff, 0xff), // cyan
};
const uint32 PGE_32::k_ParticleColorsCount =
B2_ARRAY_SIZE(PGE_32::k_ParticleColors);

//////////////////////////////////////////

PGE_32::PGE_32 ()
 : b2Draw ()
 , olc::PixelGameEngine ()
 , world_ (NULL)
 , particleSystem_ (NULL)
 , joint_ (NULL)
 , time_ (0.0f)
{
  sAppName = "Example 32";
}

PGE_32::~PGE_32 ()
{
  if (joint_)
    world_->DestroyJoint (joint_);
  if (particleSystem_)
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

    x1 = Common_GL_Tools::map (vertices[i].x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
    y1 = Common_GL_Tools::map (vertices[i].y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
    x2 = Common_GL_Tools::map (vertices[i + 1].x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
    y2 = Common_GL_Tools::map (vertices[i + 1].y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x1), static_cast<int32_t> (y1),
                                    static_cast<int32_t> (x2), static_cast<int32_t> (y2),
                                    {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255},
                                    0xFFFFFFFF);
  } // end FOR
  x1 = Common_GL_Tools::map (vertices[vertexCount - 1].x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
  y1 = Common_GL_Tools::map (vertices[vertexCount - 1].y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
  x2 = Common_GL_Tools::map (vertices[0].x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
  y2 = Common_GL_Tools::map (vertices[0].y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
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
  x = Common_GL_Tools::map (center.x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y = Common_GL_Tools::map (center.y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
  olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                    static_cast<int32_t> (radius),
                                    {static_cast<uint8_t> (color.r * 255.0f), static_cast<uint8_t> (color.g * 255.0f), static_cast<uint8_t> (color.b * 255.0f), 255},
                                    0xFF);
}

void
PGE_32::DrawSolidCircle (const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
  float x, y;
  x = Common_GL_Tools::map (center.x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y = Common_GL_Tools::map (center.y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
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
    x = Common_GL_Tools::map (centers[i].x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
    y = Common_GL_Tools::map (centers[i].y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
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
  x1 = Common_GL_Tools::map (p1.x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y1 = Common_GL_Tools::map (p1.y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
  x2 = Common_GL_Tools::map (p2.x, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth() - 1));
  y2 = Common_GL_Tools::map (p2.y, -ENGINE_PGE_32_DEFAULT_DIM, ENGINE_PGE_32_DEFAULT_DIM, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight() - 1));
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
  //flags += b2Draw::e_jointBit;
  //flags += b2Draw::e_aabbBit;
  //flags += b2Draw::e_pairBit;
  //flags += b2Draw::e_centerOfMassBit;
  flags += b2Draw::e_particleBit;
  b2Draw::SetFlags (flags);

  b2Vec2 gravity;
  gravity.Set (0.0f, 10.0f);
  world_ = new b2World (gravity);
  world_->SetDebugDraw (this);

  b2ParticleSystemDef particleSystemDef;
  particleSystem_ = world_->CreateParticleSystem (&particleSystemDef);
  particleSystem_->SetGravityScale (0.4f);
  particleSystem_->SetDensity (1.2f);

  // initialize scene
  // ground
  //b2BodyDef groundBodyDef;
  //groundBodyDef.position.Set (0, -10);

  //b2Body* ground = world_->CreateBody (&groundBodyDef);

  //b2PolygonShape groundBox;
  //groundBox.SetAsBox (100, 10);

  //ground->CreateFixture (&groundBox, 0);

  //// dynamic body
  //b2BodyDef bodyDef;
  //bodyDef.type = b2_dynamicBody;
  //bodyDef.position.Set (0.0f, 40.0f);

  //b2Body* body = world_->CreateBody (&bodyDef);

  //b2PolygonShape dynamicBox;
  //dynamicBox.SetAsBox (1.0f, 1.0f);

  //b2FixtureDef fixtureDef;
  //fixtureDef.shape = &dynamicBox;
  //fixtureDef.density = 1.0f;
  //fixtureDef.friction = 0.3f;
  //body->CreateFixture (&fixtureDef);

  b2Body* ground = NULL;
  {
    b2BodyDef bd;
    ground = world_->CreateBody (&bd);
  }

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
    jd.bodyA = ground;
    jd.bodyB = body;
    jd.localAnchorA.Set (0.0f, 1.0f);
    jd.localAnchorB.Set (0.0f, 0.0f);
    jd.referenceAngle = 0.0f;
    jd.motorSpeed = 0.05f * b2_pi;
    jd.maxMotorTorque = 1e7f;
    jd.enableMotor = true;
    joint_ = (b2RevoluteJoint*)world_->CreateJoint (&jd);
  }

  particleSystem_->SetRadius (0.025f);
  particleSystem_->SetDamping (0.2f);

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

  return true;
}

bool
PGE_32::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static float32 time_step_f =
    ENGINE_PGE_32_DEFAULT_HZ > 0.0f ? 1.0f / ENGINE_PGE_32_DEFAULT_HZ : float32(0.0f);

  if (ENGINE_PGE_32_DEFAULT_HZ > 0)
    time_ += 1 / ENGINE_PGE_32_DEFAULT_HZ;
  joint_->SetMotorSpeed (0.05f * cosf (time_) * b2_pi);

  world_->Step (time_step_f,
                ENGINE_PGE_32_DEFAULT_VEL_ITERATIONS,
                ENGINE_PGE_32_DEFAULT_POS_ITERATIONS);
  world_->DrawDebugData ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
