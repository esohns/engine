#ifndef PGE_32_H
#define PGE_32_H

#include "Box2D/Box2D.h"

#include "olcPixelGameEngine.h"

class PGE_32
 : public b2Draw
 , public olc::PixelGameEngine
{
 public:
  enum Mode
  {
    MODE_WAVEMACHINE = 0,
    MODE_WEB
  };

  PGE_32 (Mode);
  virtual ~PGE_32 ();

  // implement b2Draw
  virtual void DrawPolygon (const b2Vec2*, int32, const b2Color&);
  virtual void DrawSolidPolygon (const b2Vec2*, int32, const b2Color&);
  virtual void DrawCircle (const b2Vec2&, float32, const b2Color&);
  virtual void DrawSolidCircle (const b2Vec2&, float32, const b2Vec2&, const b2Color&);
  virtual void DrawParticles (const b2Vec2*, float32, const b2ParticleColor*, int32);
  virtual void DrawSegment (const b2Vec2&, const b2Vec2&, const b2Color&);
  virtual void DrawTransform (const b2Transform&);

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class QueryCallback : public b2QueryCallback
  {
   public:
    QueryCallback (const b2Vec2& point)
    {
      point_ = point;
      fixture_ = NULL;
    }

    bool ReportFixture (b2Fixture* fixture)
    {
      b2Body* body = fixture->GetBody ();
      if (body->GetType () == b2_dynamicBody)
      {
        bool inside = fixture->TestPoint (point_);
        if (inside)
        {
          fixture_ = fixture;
          return false;
        } // end IF
      } // end IF
      return true;
    }

    b2Vec2     point_;
    b2Fixture* fixture_;
  };

  class QueryCallback2 : public b2QueryCallback
  {
   public:
    QueryCallback2 (b2ParticleSystem* particleSystem,
                    const b2Shape* shape,
                    const b2Vec2& velocity)
    {
      particleSystem_ = particleSystem;
      shape_ = shape;
      velocity_ = velocity;
    }

    bool ReportFixture (b2Fixture* fixture)
    {
      B2_NOT_USED (fixture);
      return false;
    }

    bool ReportParticle (const b2ParticleSystem* particleSystem, int32 index)
    {
      if (particleSystem != particleSystem_)
        return false;

      b2Transform xf;
      xf.SetIdentity ();
      b2Vec2 p = particleSystem_->GetPositionBuffer ()[index];
      if (shape_->TestPoint (xf, p))
      {
        b2Vec2& v = particleSystem_->GetVelocityBuffer ()[index];
        v = velocity_;
      } // end IF

      return true;
    }

    b2ParticleSystem* particleSystem_;
    const b2Shape* shape_;
    b2Vec2 velocity_;
  };

  Mode              mode_;

  b2World*          world_;
  b2ParticleSystem* particleSystem_;
  b2Body*           ground_;
  float             dimension_;

  // mouse
  b2MouseJoint*     mouseJoint_;
  b2Vec2            mouseWorld_;
  bool              mouseTracing_;
  b2Vec2            mouseTracerPosition_;
  b2Vec2            mouseTracerVelocity_;

  // wave machine
  b2RevoluteJoint*  joint_;
  float32           time_;
  static b2ParticleColor k_ParticleColors[];
  static uint32          k_ParticleColorsCount;

  // web
  b2Body*           bodies_[4];
  b2Joint*          joints_[8];

  void initializeWaveMachine ();
  void initializeWeb ();
};

#endif // PGE_32_H
