#ifndef PGE_32_H
#define PGE_32_H

#include "Box2D/Box2D.h"

#include "olcPixelGameEngine.h"

class PGE_32
 : public b2Draw
 , public olc::PixelGameEngine
{
 public:
  PGE_32 ();
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
  b2World*          world_;
  b2ParticleSystem* particleSystem_;
  b2RevoluteJoint*  joint_;
  float             time_;

  static const b2ParticleColor k_ParticleColors[];
  static const uint32          k_ParticleColorsCount;
};

#endif // PGE_32_H
