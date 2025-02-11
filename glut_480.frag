//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;

const float PI = 3.1415926;
const float TAU = PI * 2.0;
const float E = 0.01;

struct Ray
{
  vec3 pos;
  vec3 dir;
};

mat2
rotate2D (float rad)
{
  float c = cos (rad);
  float s = sin (rad);
  return mat2 (c, s, -s, c);
}

vec2
de (vec3 p)
{
  float d = 100.0;
  float a = 0.0;

  p.yz *= rotate2D (PI / 5.0);
  p.y -= 0.5;

  vec3 reaction = vec3 (cos (iTime), 0.0, sin (iTime)) * 3.0;
  p += exp (-length (reaction - p) * 1.0) * normalize (reaction - p);

  float r = atan (p.z, p.x) * 3.0;
  const int ite = 50;
  for (int i = 0; i < ite; i++)
  {
    r += 0.5 / float (ite) * TAU;
    float s = 0.5 + sin (float (i) * 1.618 * TAU) * 0.25;
    s += sin (iTime + float (i)) * 0.1;
    vec2 q = vec2 (length (p.xz) + cos (r) * s - 3.0, p.y + sin (r) * s);
    float dd = length (q) - 0.035;
    a = dd < d ? float (i) : a;
    d = min (d, dd);
  }

  float dd = length (p - reaction) - 0.1;
  a = dd < d ? 0.0 : a;
  d = min (d, dd);

  return vec2 (d, a);
}

void
trace (Ray ray, inout vec3 color, float md)
{
  float ad = 0.0;
  for (float i = 1.0; i > 0.0; i -= 1.0 / 128.0)
  {
    vec2 o = de (ray.pos);
    if (o.x < E)
    {
      color = mix (vec3 (0.1, 0.1, 0.5), vec3 (0.0, 0.0, 1.0), fract (o.y * 1.618));
      color = mix (vec3 (1.0, 1.0, 1.0), color, step (0.05, fract (o.y * 1.618)));
      color = mix (vec3 (0.175, 0.1, 0.1), color, step (0.35, fract (o.y * 1.618 + 0.9)));
      color *= exp (-(1.0 - i) * 15.0);
      return;
    }

    o.x *= 0.6;
    ray.pos += ray.dir * o.x;
    ad += o.x;
    if (ad > md)
      break;
  }
    
  color = mix (vec3 (0.0), vec3 (1.0), ray.dir.y * ray.dir.y);
}

void
main ()
{
  vec2 p = (gl_FragCoord.xy * 2.0 - iResolution.xy) / min (iResolution.x, iResolution.y);
  vec3 color = vec3 (0.0);

  vec3 view = vec3 (0.0, 0.0, 10.0);
  vec3 at = normalize (vec3 (0.0, 0.0, 0.0) - view);
  vec3 right = normalize (cross (at, vec3 (0.0, 1.0, 0.0)));
  vec3 up = cross (right, at);
  float focallength = 3.0;

  Ray ray;
  ray.pos = view;
  ray.dir = normalize (right * p.x + up * p.y + at * focallength);

  trace (ray, color, 20.0);

  color = pow (color, vec3 (0.454545));
  gl_FragColor = vec4 (color, 1.0);
}
