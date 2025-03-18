#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

const float pi = 3.14159;

mat3
xrot (float t)
{
  return mat3(1.0, 0.0, 0.0,
              0.0, cos(t), -sin(t),
              0.0, sin(t), cos(t));
}
mat3
yrot (float t)
{
  return mat3(cos(t), 0.0, -sin(t),
              0.0, 1.0, 0.0,
              sin(t), 0.0, cos(t));
}
mat3
zrot (float t)
{
  return mat3(cos(t), -sin(t), 0.0,
              sin(t), cos(t), 0.0,
              0.0, 0.0, 1.0);
}

float
sdBox (vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float
sdBoxXY (vec3 p, vec2 b)
{
  vec2 d = abs(p.xy) - b;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

#define room (iTime * -0.25)
float mat = 0.0;
vec2 muv = vec2(0.0);
vec3 mpos = vec3(0.0);

float
map (vec3 p)
{
  mat = 1.0;
  float d = 1000.0;

  vec3 h = p;
  h *= zrot(room * 2.0);
  h *= yrot(room * 3.0);

  float rs = 4.0;
  d = min(d, -sdBox(h, vec3(rs)));
  mpos = h;

  float cv = 0.3;
  d = max(d, -sdBoxXY(h, rs * vec2(cv)));
  float o = -sdBoxXY(h.zyx, rs * vec2(cv));
  if (o > d)
  {
    d = o;
    mpos = h.zyx;
  }

  o = -sdBoxXY(h.xzy, rs * vec2(cv));
  if (o > d)
  {
    d = o;
    mpos = h.xzy;
  }

  vec3 q = p - vec3(0.0, 0.0, 3.0);
  q *= zrot(room * 3.14159 * 2.0);
  vec3 ps = vec3(1.0, 1.0, 0.11);
  float frame = sdBox(q, ps + vec3(0.1, 0.1, -0.01));
  if (frame < d)
  {
    d = frame;
    mat = 2.0;
    muv = q.xy / ps.xy;
    mpos = q;
  }

  float portal = sdBox(q, ps);
  if (portal < d)
  {
    d = portal;
    mat = 0.0;
    muv = q.xy / ps.xy;
  }

  return d;
}

vec3
normal (vec3 p)
{
  vec3 o = vec3(0.001, 0.0, 0.0);
  return normalize(vec3(map(p+o.xyy) - map(p-o.xyy),
                        map(p+o.yxy) - map(p-o.yxy),
                        map(p+o.yyx) - map(p-o.yyx)));
}

float
trace (vec3 o, vec3 r)
{
  float t = 0.0;
  for (int i = 0; i < 32; ++i)
  {
    vec3 p = o + r * t;
    float d = map(p);
    t += d;
  }

  return t;
}

mat3
xform (float t)
{
  mat3 xfm = yrot((mod(t, 3.0) + 1.0) * pi * 0.5);
  //xfm *= xrot(t * pi * 0.5);
  return xfm;
}

vec3
campos (float t, float u)
{
  vec3 exit = vec3(0.0, 0.0, 2.99);
  vec3 enter = exit * xform(t);
  vec3 pos = mix(enter, exit, u);
  return pos;
}

mat3
slerp (vec3 a, vec3 b, float t)
{
  vec3 fw = normalize(a);
  vec3 up = vec3(0.0, 1.0, 0.0);
  vec3 left = normalize(cross(fw, up));
  up = normalize(cross(left, fw));
  mat3 xfm = mat3(left, up, fw);
  vec3 k = xfm * b;
  float theta = atan(k.x, k.z) * t;
  fw = vec3(sin(theta), 0.0, cos(theta)) * xfm;
  left = normalize(cross(fw, up));
  up = normalize(cross(left, fw));
  return mat3(left, up, fw);
}

vec3
_texture (sampler2D s, vec3 p, vec3 n)
{
  mat3 t = mat3(texture(s, p.zy).xyz,
                texture(s, p.xz).xyz,
                texture(s, p.xy).xyz);
  return t * t * abs(n);
}

vec3
bump (sampler2D s, vec3 p, vec3 n)
{
  vec2 d = vec2(0.01, 0.0);
  mat3 m = mat3(_texture(s, p - d.xyy, n),
                _texture(s, p - d.yxy, n),
                _texture(s, p - d.yyx, n));
  vec3 gs = vec3(0.299, 0.587, 0.114);
  vec3 v = gs * m;
  vec3 c = dot(_texture(s, p, n), gs) - v;
  return normalize(n + c * 0.5);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  uv = uv * 2.0 - 1.0;
  uv.x *= iResolution.x / iResolution.y;

  float fov = 0.9;
  vec3 sv = normalize(vec3(uv, fov - dot(uv, uv) * 0.2));

  vec3 oo = campos(floor(room), fract(room));
  sv *= zrot(room * 3.14159);

  vec3 or = sv;
  float st = 1.0 - pow(1.0 - fract(room), 2.0);
  vec3 rto = vec3(0.0, 0.0, 1.0);
  vec3 rfrom = -rto * xform(floor(room));
  or = or * slerp(rfrom, rto, st);

  vec3 r = or;
  vec3 o = oo;

  vec3 fc = vec3(0.0);

  for (int i = 0; i < 8; ++i)
  {
    float fi = float(i);

    float t = trace(o, r);
    vec3 w = o + r * t;
    vec3 sn = normal(w);

    if (mat != 0.0)
    {
      vec3 sc = vec3(0.0);
      float rtex = mod(floor(room + fi), 3.0);
      if (rtex == 0.0)
      {
        sn = bump(iChannel0, mpos * 0.125, sn);
        sc = _texture(iChannel0, mpos * 0.25, sn);
      } else if (rtex == 1.0)
      {
        sn = bump(iChannel1, mpos * 0.25, sn);
        sc = _texture(iChannel1, mpos * 0.25, sn);
      } else if (rtex == 2.0)
      {
        sn = bump(iChannel2, mpos * 0.25, sn);
        sc = _texture(iChannel2, mpos * 0.25, sn);
      }

      vec3 lpos = vec3(2.0, 2.0, -2.0);
      vec3 ldir = lpos - w;
      float ldist = length(ldir);
      ldir /= ldist;
      float lprod = max(dot(ldir, sn), 0.0);
      float lpow = 1.0 / (1.0 + ldist * ldist * 0.01);

      float fog = lprod / (1.0 + t * t * 0.001);

      fc += vec3(fog) * sc * lpow;

      break;
    }

    float to = clamp(-sign(fi - 0.5), 0.0, 1.0);

    vec2 pmuv = muv * vec2(-1.0,1.0);
    r = normalize(vec3(pmuv, fov - dot(pmuv, pmuv) * 0.2));
    float speed = pow(fract(room), 4.0);
    r = mix(r, sv, speed * to);
    r = normalize(r);
    vec3 srt = vec3(0.0, 0.0, 1.0);
    vec3 srf = -srt * xform(floor(room + fi + 1.0));
    r = r * slerp(srf, srt, 0.0);
    o = campos(floor(room + fi + 1.0), 0.0);
  }

  gl_FragColor = vec4(fc, 1.0);
}
