#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define EPSILON 0.001
#define DIST_MAX 50.0
#define ITER_MAX 200u

#define pi 3.1415926
#define tau (pi+pi)

mat3
yaw_pitch_roll (float yaw, float pitch, float roll)
{
  mat3 R = mat3(vec3(cos(yaw), sin(yaw), 0.0), vec3(-sin(yaw), cos(yaw), 0.0), vec3(0.0, 0.0, 1.0));
  mat3 S = mat3(vec3(1.0, 0.0, 0.0), vec3(0.0, cos(pitch), sin(pitch)), vec3(0.0, -sin(pitch), cos(pitch)));
  mat3 T = mat3(vec3(cos(roll), 0.0, sin(roll)), vec3(0.0, 1.0, 0.0), vec3(-sin(roll), 0.0, cos(roll)));

  return R * S * T;
}

float
halfspace (vec3 n, vec3 p)
{
  return dot(n,p);
}

float
rhombic_dodecahedron (vec3 position, float halfSize)
{
  halfSize *= 0.5;
  position = abs(position);
  position.xy -= vec2(-1.0, 1.0) * min(position.y - position.x, 0.0);
  position.yz -= vec2(1.0, -1.0) * min(position.y - position.z, 0.0);
  position.y -= halfSize;
  position.xz = abs(position.xz);
  position.xz -= vec2(-1.0, 1.0) * min(position.z - position.x, 0.0);
  float h = position.y + position.z - halfSize;
  vec3 q = position;
  q.yz -= 0.5 * h;
  q -= vec3(2.0, 1.0, -1.0) / 3.0 * max(q.x - q.z, 0.0);
  q = clamp(q, 0.0, halfSize);
  float d = length(position - q);

  return h < 0.0 ? -d : d;
}

float
box (vec3 p, vec3 b)
{
  vec3 q = abs(p) - b;

  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float
pyramid (vec3 position, float halfWidth, float halfHeight)
{
  position.z += halfHeight;
  position.xy = abs(position.xy);
  if (position.x > position.y)
    position.xy = position.yx;

  vec3 d1 = vec3(max(position.x - halfWidth, 0.0), position.z, max(position.y - halfWidth, 0.0));

  vec3 q = position;
  float k = halfWidth * halfWidth + 4.0 * halfHeight * halfHeight;
  float h = dot(q.zy - vec2(0.0, halfWidth), vec2(halfWidth, 2.0 * halfHeight)) / k;
  q.zy -= vec2(halfWidth, 2.0 * halfHeight) * h;
  q -= vec3(k, -halfWidth * halfWidth, 2.0 * halfHeight * halfWidth) * max(q.x - q.y, 0.0) / (k + halfWidth * halfWidth);
  vec3 d2 = position - clamp(q, vec3(0.0), vec3(halfWidth, halfWidth, 2.0 * halfHeight));
  float d = sqrt(min(dot(d1, d1), dot(d2, d2)));

  return max(h, -position.z) < 0.0 ? -d : d;
}

float
box_with_roofs (vec3 p, float h)
{
  float d = DIST_MAX;
  d = min(d, box(p, vec3(0.5)));

  if(true)
  {
    p = abs(p);
    vec3 o = vec3(0,-1,+1);
    vec3 s = p * mat3(o.xyz,o.yxz,o.yzx);

    if(s.x < 0.0 && s.z > 0.0) p.yzx = p.xyz;
    if(s.y < 0.0 && s.z < 0.0) p.zxy = p.xyz;
  }

  d = min(d, pyramid(p-vec3(0,0,0.5+0.25*h), 0.5, 0.25*h));

  return d;
}

float
line (vec3 p, vec3 a, vec3 b)
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp(dot(pa,ba)/dot(ba,ba), 0.0, 1.0);

  return length(pa - ba*h);
}

float
rhombic_dodecahedron_mesh (vec3 p)
{
  p = abs(p - 0.5) + 0.5;
  p.xy = p.x < p.y ? p.xy : p.yx;
  p.yz = p.y < p.z ? p.yz : p.zy;

  return line(p, vec3(1,1,0.5), vec3(0.5,0.5,1));
}

vec4
combine (vec4 a, vec4 b)
{
  return a.w < b.w ? a : b;
}

vec4
intersect (vec4 a, vec4 b)
{
  return a.w > b.w ? a : b;
}

vec4
cut (vec4 shape, float cut)
{
  return shape.w > cut ? shape : vec4(shape.xyz, cut);
}

vec3
offset (vec3 o, vec3 p)
{
  vec3 l = vec3(1);
  float s = 2.0;

  return p - o - s*clamp(round((p - o)/s),-l,l);
}

vec4
map (vec3 p)
{
  float h = 0.5-0.5*cos(tau*iTime*0.1);

  return cut(combine(combine(vec4(vec3(0,0.3,1), box_with_roofs(offset(vec3(0,0,0), p),h)),
                             vec4(vec3(1,0.55,0),box_with_roofs(offset(vec3(1,1,0), p),h))),
                     combine(vec4(vec3(1,0.05,0),box_with_roofs(offset(vec3(0,1,1), p),h)),
                             vec4(vec3(0.15,1,0),box_with_roofs(offset(vec3(1,0,1), p),h)))),
             halfspace(normalize(vec3(1,1,1)),p+1.0/6.0));
}

vec3
normal (vec3 p)
{
  const float h = EPSILON;
  const vec2 k = vec2(1,-1);

  return normalize(k.xyy * map(p + k.xyy*h).w + k.yyx * map(p + k.yyx*h).w + k.yxy * map(p + k.yxy*h).w + k.xxx * map(p + k.xxx*h).w);
}

vec4
trace (vec3 ro, vec3 rd, float t0, float t1)
{
  uint i;
  vec4 h = vec4(0);
  float t;
  for(t = t0, i = 0u; t < t1 && i < ITER_MAX; i++)
  {
    h = map(ro + rd * t);
    if(h.w < EPSILON)
      return vec4(h.xyz,t);
    t += h.w;
  }

  return vec4(h.xyz,t);
}

float
shadow (vec3 ro, vec3 rd, float t0, float t1)
{
  return trace(ro, rd, t0, t1).w < t1 ? 0.0 : 1.0;
}

float
softshadow (vec3 ro, in vec3 rd, float t0, float t1, float k)
{
  float res = 1.0;
  float ph = 1e20;
  uint i;
  float t;
  for(t = t0, i = 0u; t < t1 && i < ITER_MAX; i++)
  {
    float h = map(ro + rd*t).w;
    if( h < EPSILON )
      return 0.0;
    float y = h*h/(2.0*ph);
    float d = sqrt(h*h-y*y);
    res = min( res, k*d/max(0.0,t-y) );
    ph = h;
    t += h;
  }

  return res;
}

float
calcAO (vec3 pos, vec3 nor)
{
  float occ = 0.0;
  float sca = 1.0;
  for( int i=0; i<5; i++ )
  {
    float h = 0.01 + 0.12*float(i)/4.0;
    float d = map( pos + h*nor ).w;
    occ += (h-d)*sca;
    sca *= 0.95;
    if( occ>0.35 )
      break;
  }

  return clamp( 1.0 - 3.0*occ, 0.0, 1.0 ) ;
}

void
main ()
{
  vec4 o;
  vec2 I = gl_FragCoord.xy;

  bool demo = all(lessThan(iMouse.xy, vec2(10.0)));
  vec2 R = iResolution;
  I = (I + I - R) / R.y;
  float yaw = tau * float(iMouse.x) / float(R.x);
  float pitch = pi - pi * float(iMouse.y) / float(R.y);
  yaw = !demo ? yaw : tau * (fract(iTime * 0.02)-0.25);
  pitch = !demo ? pitch : 1.5 * pi / 2.0;

  vec3 ro = vec3(0,0,-12.5);
  vec3 rd = normalize(vec3(I.xy, 2.5));

  mat3 M = yaw_pitch_roll(yaw, pitch, 0.0);
  ro = M * ro;
  rd = M * rd;

  vec3 lightdir = M * normalize(vec3(-.35, 1,-1));

  vec4 result = trace(ro, rd, 0.0, DIST_MAX);
  vec3 color = result.xyz;
  float dist = result.w;
  if(dist < DIST_MAX)
  {
    vec3 dst = ro + rd * dist;
    vec3 n = normal(dst);

    vec3 ambient = vec3(0.1);
    float brightness = max(dot(lightdir, n), 0.0);
    if(brightness > 0.0)
      brightness *= shadow(ro + rd * dist + n * 0.01, lightdir, 0.0, DIST_MAX);
    color *= (ambient * calcAO(dst, n)) + brightness*3.0;

    if(brightness > 0.0)
    {
      float specular = pow(max(0.0, dot(n, normalize(-rd + lightdir))), 250.0);
      color += specular;
    }

    vec3 fog_color = vec3(0);
    color = mix(fog_color, vec3(color), exp(-pow(dist/20.0, 2.0))); // fog
  }
  else
    color *= 0.0;

  color = tanh(color);
  color = sqrt(color);

  o = vec4(color, 1);

  gl_FragColor = o;
}
