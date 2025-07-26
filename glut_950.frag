#version 140

// glut_950_common.glsl
//#define LOOP
//#define LOOP2
//#define NIGHT_MODE
//#define CROP

float
easeOutBack (float x, float t)
{
  float c1 = t;
  float c3 = c1 + 1.;

  return 1. + c3 * pow(x - 1., 3.) + c1 * pow(x - 1., 2.);
}

float
easeInOutBack (float x)
{
  float c1 = 1.70158;
  float c2 = c1 * 1.525;

  return x < .5
    ? (pow(2. * x, 2.) * ((c2 + 1.) * 2. * x - c2)) / 2.
    : (pow(2. * x - 2., 2.) * ((c2 + 1.) * (x * 2. - 2.) + c2) + 2.) / 2.;
}

float
easeSnap (float x)
{
  x = pow(x, .75);
  x = easeInOutBack(x);
  return x;
}

float
linearstep (float a, float b, float t)
{
  return clamp((t - a) / (b - a), 0., 1.);
}

#ifdef LOOP
float timeOffset = (92. + 100.) * (3./4.);
float timeGap = 3.;
#else
#ifdef LOOP2
float timeOffset = (92. + 100.);
float timeGap = 4.;
#else
float timeOffset = (92. + 100.);
float timeGap = 4.;
#endif
#endif

float gTime;
float gDuration;
float gSpeed;

void
initTime (float time)
{
  gTime = time;
  gSpeed = 1.;
  gDuration = 14.;

#ifdef LOOP
  gSpeed = 1.5;
  gDuration = (3. * timeGap) / gSpeed; // 6
  gTime /= gDuration;
  gTime = fract(gTime);
  gTime *= gDuration;
  gTime *= gSpeed;
  gTime += .25;
#endif

#ifdef LOOP2
  gSpeed = 1.;
  gDuration = (3. * timeGap) / gSpeed; // 12
  gTime /= gDuration;
  gTime = fract(gTime);
  gTime *= gDuration;
  gTime *= gSpeed;
#endif
}

float
tFloor (float time)
{
  time += timeOffset;
  time -= timeGap / 3.;
  return floor(time / timeGap);
}

float
tFract (float time)
{
  time += timeOffset;
  time -= timeGap / 3.;
  return fract(time / timeGap) * timeGap * .5;
}

vec3
primaryAxis (vec3 p)
{
  vec3 a = abs(p);
  return (1.-step(a.xyz, a.yzx))*step(a.zxy, a.xyz)*sign(p);
}
// glut_950_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

//#define ANIMATE

#define PI 3.14159265359

mat3
scaleM (float s)
{
  return mat3(
    s, 0, 0,
    0, s, 0,
    0, 0, 1
  );
}

mat3
rotM (float a)
{
  return mat3(
    cos(a), sin(a), 0,
    -sin(a), cos(a), 0,
    0, 0, 1
  );
}

mat3
transM (vec2 v)
{
  return mat3(
    1, 0, v.x,
    0, 1, v.y,
    0, 0, 1
  );
}

vec2
mul (vec2 p, mat3 m)
{
   return (vec3(p, 1) * m).xy;
}

void
calcAngleOffset (float tf, float ts, out float angle, out vec2 offset)
{
  float time = tf + ts;
  angle = time;
  offset = vec2(0, time);

#ifndef LOOP
#ifndef LOOP2
  return;
#endif
#endif

#ifdef NIGHT_MODE
  float to = timeOffset / timeGap + 17.;
#else
  float to = timeOffset / timeGap;
#endif

  angle = 0.;
  if (mod(tf, 3.) == 2.)
    angle += mod(tf, 3.) + ts * -2.;
  else
    angle += mod(tf, 3.) + ts;
  angle *= 1.;
  angle += to;

#ifdef NIGHT_MODE
  angle = .0;
#endif

  vec3 ba = max(vec3(0), mod(vec3(tf + 2., tf + 1., tf), 3.) - 1.);
  vec3 bb = max(vec3(0), mod(vec3(tf + 3., tf + 2., tf + 1.), 3.) - 1.);
  vec3 bary = mix(ba, bb, ts);

#ifndef NIGHT_MODE
  vec2 o1 = vec2(0,0);
  vec2 o2 = vec2(-1.,-.3);
  vec2 o3 = vec2(.3,-.333);
#else
  float p3 = PI * 2. / 3.;
  vec2 o1 = vec2(sin(0.),cos(0.)) * .8;
  vec2 o2 = vec2(sin(p3),cos(p3)) * .8;
  vec2 o3 = vec2(sin(p3*2.),cos(p3*2.)) * .8;
#endif

  offset = (bary.x * o1 + bary.y * o2 + bary.z * o3);
    
#ifdef NIGHT_MODE
  offset += 5.0;
  offset += 2.05;
  offset += 2.75;
  offset.y -= .2;
  offset.x += .05;

  offset += 15. + vec2(-.25,0);
#endif

  offset.y += to;
}

mat3
gridTransformation (out float scale)
{
  float tf = tFloor(gTime);
  float ts = easeSnap(linearstep(.3, .8, tFract(gTime)));

  scale = 2.5;
        
  float angle;
  vec2 offset;
  calcAngleOffset(tf, ts, angle, offset);  

  mat3 m = scaleM(scale);
  m *= rotM(PI * -.08 * angle);
  m *= transM(offset * -.78);
  return m;
}

mat3
gridTransformation2 (out float scale)
{
  float tf = tFloor(gTime);
  float ts = easeOutBack(linearstep(.6, .9, tFract(gTime)), 1.70158);

  scale = 3.5;

  float angle;
  vec2 offset;
  calcAngleOffset(tf, ts, angle, offset);  

  mat3 m = scaleM(scale);
  m *= rotM(PI * -.08 * angle * .5);
  m *= rotM(PI * 2./3.);
  m *= transM((offset * -0.78 * .5).yx);

  return m;
}

float
effectMask (vec2 uv)
{
  return sin(length(uv) * 12. + 1.) * .5 + .5;
}

float
vmax (vec3 v)
{
  return max(v.x, max(v.y, v.z));
}

const float s3 = sin(PI / 3.);

vec3
sdTriEdges (vec2 p)
{
  return vec3(
    dot(p, vec2(0,-1)),
    dot(p, vec2(s3, .5)),
    dot(p, vec2(-s3, .5))
  );
}

float
sdTri (vec2 p)
{
  vec3 t = sdTriEdges(p);
  return max(t.x, max(t.y, t.z));
}

float
sdTri (vec3 t)
{
  return max(t.x, max(t.y, t.z));
}

float
sdBorder (vec3 tbRel, vec2 pt1, vec2 pt2)
{
  vec3 axis = primaryAxis(-tbRel);
  bool isEdge = axis.x + axis.y + axis.z < 0.;

  vec2 gA = vec2(0,-1);
  vec2 gB = vec2(s3, .5);
  vec2 gC = vec2(-s3, .5);

  vec2 norA = gC * axis.x + gA * axis.y + gB * axis.z;
  vec2 norB = gB * -axis.x + gC * -axis.y + gA * -axis.z;

  vec2 dir = gA * axis.x + gB * axis.y + gC * axis.z;
  vec2 corner = dir * dot(dir, pt1 - pt2) * 2./3.;

  vec2 ca, cb;
  float side;

  if (isEdge)
  {
    corner = pt2 + corner;
    ca = corner + max(0., dot(corner, -norB)) * norB;
    cb = corner + min(0., dot(corner, -norA)) * norA;
  }
  else
  {
    corner = pt1 - corner;
    ca = corner + max(0., dot(corner, -norA)) * norA;
    cb = corner + min(0., dot(corner, -norB)) * norB;
  }

  side = step(dot(corner, dir * mat2(0,-1,1,0)), 0.);
  corner = mix(ca, cb, side);

  float d = length(corner);

  return d;
}

vec2
hash2 (vec2 p)
{
  return textureLod( iChannel0, (p+0.5)/256.0, 0.0 ).xy;
}

vec2
cellPoint (vec2 n, vec2 f, vec2 cell, bool gaps)
{
  vec2 coord = n + cell;
  vec2 o = hash2( n + cell );
  if (gaps && hash2(o.yx * 10.).y > .5)
    return vec2(1e12);
#ifdef ANIMATE
  o = 0.5 + 0.5*sin( time * PI * 2. + 6.2831*o );
#endif	
  vec2 point_ = cell + o - f;
  return point_;
}

vec4
voronoi (vec2 x, bool gaps)
{
  vec2 n = floor(x);
  vec2 f = fract(x);

  vec2 closestCell, closestPoint;

  const int reach = 3;

  float closestDist = 8.0;
  for( int j = -reach; j <= reach; j++ )
  for( int i = -reach; i <= reach; i++ )
  {
    vec2 cell = vec2(i, j);
    vec2 point_ = cellPoint(n, f, cell, gaps);
    float dist = vmax(sdTriEdges(point_));

    if( dist < closestDist )
    {
      closestDist = dist;
      closestPoint = point_;
      closestCell = cell;
    }
  }

  closestDist = 8.0;
  for( int j = -reach-1; j <= reach+1; j++ )
  for( int i = -reach-1; i <= reach+1; i++ )
  {
    vec2 cell = closestCell + vec2(i, j);
    vec2 point_ = cellPoint(n, f, cell, gaps);

    vec3 triEdges = sdTriEdges(closestPoint - point_);
    float dist = vmax(triEdges);

    if( dist > 0.00001 )
      closestDist = min(closestDist, sdBorder(triEdges, closestPoint, point_));
  }

  return vec4(closestDist, closestCell + n, 0.);
}

void
main ()
{
  initTime(iTime);

  vec2 uv = gl_FragCoord.xy / iResolution;
  uv *= vec2(1,-1);
  uv.y /= 2.;

  if (uv.x > uv.y * -2.)
  {
    gl_FragColor = vec4(0);
    return;
  }

  float scl;
  mat3 m = gridTransformation(scl);

  vec2 uvm = mul(uv, m);

  vec4 v = voronoi(uvm, false);
  float d = v.x / scl;
  vec2 localPt = v.yz;
  vec2 worldPt = mul(localPt, inverse(m));
  vec2 seed = hash2(localPt);
  float id = 0.;
  float tile = mod(localPt.x + localPt.y, 3.);

  if (tile == 0.)
  {
    m = gridTransformation2(scl);
    v = voronoi(mul(uv, m), false);
    d = min(d, v.x / scl);
    localPt = v.yz;
    worldPt = mul(localPt, inverse(m));
    seed = hash2(localPt + seed * 10.);
    id = 1.;
    tile = mod(localPt.x + localPt.y, 3.);
  }

  float mask = effectMask(worldPt);

  mask = texture(iChannel1, uvm * .25).r;

  gl_FragColor = vec4(d, id, tile, mask);
}
