#version 130

// glut_714_common.glsl
const int NUM_OBJECTS = 20;
const float BALL_SIZE = 0.15;
const float MAX_VELOCITY = 4.0;
const float ELASTICITY = 0.5;
const vec2 FRAME_SIZE = vec2(1.2,0.8);
const vec2 GRAVITY = vec2(0.0,-1.0);

const float PI = 3.141592;
const float DEG2RAD = PI / 180.0;
 
vec3
hash3 (uint n)
{
  n = (n << 13U) ^ n;
  n = n * (n * n * 15731U + 789221U) + 1376312589U;
  uvec3 k = n * uvec3(n,n*16807U,n*48271U);
  return vec3( k & uvec3(0x7fffffffU))/float(0x7fffffff);
}

float
cross2 (vec2 a, vec2 b)
{
  return a.x * b.y - a.y * b.x;
}

vec2
cross2 (vec2 a, float b)
{
  return vec2(a.y * b, a.x * -b);
}

vec2
cross2 (float a, vec2 b)
{
  return vec2(b.y * -a, b.x * a);
}

vec2
rotateZ (vec2 v, float a)
{
  lowp vec2 sc = vec2(sin(a),cos(a));
  lowp vec2 ret = v;
  ret.x = v.x * sc.y - v.y * sc.x;
  ret.y = v.x * sc.x + v.y * sc.y;
  return ret;
}

struct Body
{
  vec2 pos;
  vec2 vel;
  float ang;
  float ang_vel;
  float inv_mass;
  float inv_inertia;
};

Body
getBody (sampler2D buf, vec2 ires, int i)
{
  vec4 data0 = texture(buf, (vec2(float(i),0.0) + 0.5) * ires);
  vec4 data1 = texture(buf, (vec2(float(i),1.0) + 0.5) * ires);
    
  Body body;
  body.pos = data0.xy;
  body.vel = data0.zw;
  body.ang = data1.x;
  body.ang_vel = data1.y;
  body.inv_mass = data1.z;
  body.inv_inertia = data1.w;
  return body;
}

void
initBody (int id, inout Body body)
{
  vec3 rnd = hash3(uint(id));
  body.pos = (rnd.xy * 2.0 - 1.0) * 0.4;
  body.vel = cross2(1.0,body.pos) * 2.0;
  body.ang_vel = length(body.pos) * -8.0;
  body.inv_mass = 1.0;
  body.inv_inertia = 1.0 / (0.5 * (1.0/body.inv_mass) * BALL_SIZE * BALL_SIZE);
}

vec2
collisionWithPlane (inout Body b0, vec3 plane)
{
  vec2 normal = normalize(plane.xy);
  float dist = dot(b0.pos,normal) + plane.z;
  float penetration = BALL_SIZE - dist;
  if(penetration > 0.0)
  {
    vec2 r0 = -normal * BALL_SIZE;        

    vec2 vel0 = b0.vel + cross2(b0.ang_vel,r0);
    vec2 rel_vel = vel0;  
        
    float w1 = cross2(r0,normal);

    float a = (1.0 + ELASTICITY) * dot(normal,rel_vel);
    float b = b0.inv_mass + w1 * w1 * b0.inv_inertia;
    float lambda = max(-a / b, 0.0);

    b0.vel += normal * (lambda * b0.inv_mass);
    b0.ang_vel += cross2(r0, normal) * lambda * b0.inv_inertia;

    vel0 = b0.vel + cross2(b0.ang_vel,r0);
    rel_vel = vel0;  

    vec2 tangent = cross2(normal,1.0);
    w1 = cross2(r0,tangent);

    a = (1.0 + ELASTICITY) * dot(tangent,rel_vel);
    b = b0.inv_mass + w1 * w1 * b0.inv_inertia;
    float lambdaF = clamp(-a / b, -lambda, lambda);

    b0.vel += tangent * (lambdaF * b0.inv_mass);
    b0.ang_vel += cross2(r0, tangent) * lambdaF * b0.inv_inertia;
        
    return normal * penetration;
  }

  return vec2(0.0);
}

vec2
collisionWithBody (inout Body b0, in Body b1)
{
  vec2 normal = b0.pos - b1.pos;
  float dist = length(normal);
  float penetration = 2.0 * BALL_SIZE - dist;
  if(penetration > 0.0)
  {
    normal /= dist;

    vec2 r0 = -normal * BALL_SIZE;
    vec2 r1 = normal * BALL_SIZE;
        
    vec2 vel0 = b0.vel + cross2(b0.ang_vel,r0);
    vec2 vel1 = b1.vel + cross2(b1.ang_vel,r1);
    vec2 rel_vel = vel0 - vel1;
        
    float w1 = cross2(r0,normal);
    float w2 = cross2(r1,normal);

    float a = (1.0 + ELASTICITY) * dot(normal,rel_vel);
    float b = b0.inv_mass + b1.inv_mass +
        w1 * w1 * b0.inv_inertia +
        w2 * w2 * b1.inv_inertia;
    float lambda = max(-a / b, 0.0);

    b0.vel += normal * (lambda * b0.inv_mass);
    b0.ang_vel += cross2(r0, normal) * lambda * b0.inv_inertia;
    b1.vel -= normal * (lambda * b1.inv_mass);
    b1.ang_vel -= cross2(r1, normal) * lambda * b1.inv_inertia;

    vel0 = b0.vel + cross2(b0.ang_vel,r0);
    vel1 = b1.vel + cross2(b1.ang_vel,r1);
    rel_vel = vel0 - vel1;  

    vec2 tangent = cross2(normal,1.0);
    w1 = cross2(r0,tangent);
    w2 = cross2(r1,tangent);

    a = (1.0 + ELASTICITY) * dot(tangent,rel_vel);
    b = b0.inv_mass + b1.inv_mass +
        w1 * w1 * b0.inv_inertia +
        w2 * w2 * b1.inv_inertia;
    float lambdaF = clamp(-a / b, -lambda, lambda);

    b0.vel += tangent * (lambdaF * b0.inv_mass);
    b0.ang_vel += cross2(r0, tangent) * lambdaF * b0.inv_inertia;
        
    return normal * penetration * 0.5;
  }

  return vec2(0.0);
}

void
solve (sampler2D data, inout Body b0, int id, vec2 ires)
{
  vec2 displace = vec2(0.0);
    
  for(int i = 0; i < NUM_OBJECTS; i++)
  {
    if(i == id) continue;

    Body b1 = getBody(data, ires, i);
    displace += collisionWithBody(b0,b1);
  }

  displace += collisionWithPlane(b0, vec3(0.0,1.0,FRAME_SIZE.y));
  displace += collisionWithPlane(b0, vec3(0.0,-1.0,FRAME_SIZE.y));
  displace += collisionWithPlane(b0, vec3(1.0,0.0,FRAME_SIZE.x));
  displace += collisionWithPlane(b0, vec3(-1.0,-.0,FRAME_SIZE.x));

  b0.pos += displace;
}

float
isInside (vec2 p, vec2 c)
{
  vec2 d = abs(p-0.5-c) - 0.5;
  return -max(d.x,d.y);
}

void
storeBody (int id, Body b, inout vec4 col, vec2 uv)
{
  col = isInside(uv,vec2(float(id),0)) > 0.0 ? vec4(b.pos,b.vel) : col;
  col = isInside(uv,vec2(float(id),1)) > 0.0 ? vec4(b.ang,b.ang_vel,b.inv_mass,b.inv_inertia) : col;
}
// glut_714_common.glsl

uniform vec2 iResolution;
uniform float iTimeDelta;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

vec2
getForce (vec2 x, vec2 v)
{
  vec2 force = vec2(0.0);

  if(iMouse.z > 0.5)
  {
    vec2 mouse = iMouse.xy / iResolution.xy * 2.0 - 1.0;
    mouse.x *= iResolution.x / iResolution.y;
    vec2 dir = x.xy - mouse;
    float p = length(dir);        
    force += 5.0 * normalize(dir) / p;
  }

  return force;
}

void
main ()
{
  int id = int(gl_FragCoord.x);
  if(id >= NUM_OBJECTS) discard;
  vec2 ires = 1.0 / vec2 (textureSize (iChannel0, 0));

  Body body = getBody(iChannel0, ires, id);
  if(iFrame == 0) initBody(id, body);

  float dt = min(iTimeDelta, 0.07);

  vec2 pvel = body.vel;
  float pang_vel = body.ang_vel;
  vec2 force = getForce(body.pos, body.vel);
  body.vel += (force * body.inv_mass + GRAVITY) * dt;

  float len2 = dot(body.vel,body.vel);
  if(len2 > MAX_VELOCITY * MAX_VELOCITY)
    body.vel *= inversesqrt(len2) * MAX_VELOCITY;

  body.pos += (pvel + body.vel) * 0.5 * dt;
  body.ang += (pang_vel + body.ang_vel) * 0.5 * dt;

  gl_FragColor = vec4(0.0);
  storeBody(id, body, gl_FragColor, gl_FragCoord.xy);
}
