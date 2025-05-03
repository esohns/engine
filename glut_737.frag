#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 20.

float objID = 0.;

vec3
tpl (sampler2D t, in vec3 p, in vec3 n)
{
  n = max(abs(n), 0.001);
  n /= (n.x + n.y + n.z);
  p = (texture(t, p.yz) * n.x + texture(t, p.zx) * n.y + texture(t, p.xy) * n.z).xyz;
  return p * p;
}

vec3
camPath (float t)
{
  return vec3(-sin(t / 2.), sin(t / 2.) * .5 + 1.57, t);
}

float
map (vec3 p)
{
  p.xy -= camPath(p.z).xy;

  vec2 a = sin(vec2(1.57, 0) + p.z * 1.57 / 10.);
  p.xy = mat2(a, -a.y, a.x) * p.xy;

  vec3 q = abs(fract(p + vec3(.5, 0, .5)) - .5);
  float joins = (q.x + q.y + q.z) - .16;

  p = abs(fract(p + vec3(0, .5, 0)) - .5);
  joins = min(joins, (p.x + p.y + p.z) - .16);

  p = abs(p - (p.x + p.y + p.z) * .3333);
  float lat = (p.x + p.y + p.z) * .5 - 0.034;

  float bolts = max(lat - .015, -(joins - .48));

  objID = step(joins, lat) + step(bolts, lat) * 2.;

  return min(min(joins, lat), bolts);
}

float
cao (vec3 p, vec3 n)
{
  float sca = 1., occ = 0.;
  for (float i = 0.; i < 5.; i++)
  {
    
    float hr = .01 + i * .35 / 4.;
    float dd = map(n * hr + p);
    occ += (hr - dd) * sca;
    sca *= 0.7;
  }
  return clamp(1.0 - occ, 0., 1.);
}

vec3
nr (vec3 p, inout float edge)
{
  vec2 e = vec2(.003, 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p) * 2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge / e.x * 2.));

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0, h;
  for (int i = 0; i < 128; i++)
  {
    h = map(ro + rd * t);
    if (abs(h) < 0.001 * (t * .125 + 1.) || t > FAR)
      break;
    t += h * .85;
  }

  return min(t, FAR);
}

float
sha (vec3 ro, vec3 rd, float start, float end, float k)
{
  float shade = 1.0;
  const int maxIterationsShad = 20;

  float dist = start;
  float stepDist = end / float(maxIterationsShad);

  for (int i = 0; i < maxIterationsShad; i++)
  {
    float h = map(ro + rd * dist);

    shade = min(shade, smoothstep(0.0, 1.0, k * h / dist));

    dist += clamp(h, 0.005, 0.16);
        
    if (abs(h) < 0.001 || dist > end)
      break;
  }

  return max(shade, 0.);
}

vec3
db (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.001, 0);

  mat3 m = mat3(tpl(tx, p - e.xyy, n), tpl(tx, p - e.yxy, n), tpl(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114) * m;
  g = (g - dot(tpl(tx, p, n), vec3(0.299, 0.587, 0.114))) / e.x;
  g -= n * dot(n, g);

  return normalize(n + g * bf);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p);
  p -= ip;
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p * p * (3. - 2. * p);
  h = mix(fract(sin(h) * 43758.5453), fract(sin(h + s.x) * 43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z);
}

vec3
eMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd;

  rd.xy -= iTime * .25;
  rd *= 3.;

  float c = n3D(rd) * .57 + n3D(rd * 2.) * .28 + n3D(rd * 4.) * .15;
  c = smoothstep(0.4, 1., c);

  vec3 col = vec3(min(c * 1.5, 1.), pow(c, 2.5), pow(c, 12.));

  return mix(col, col.yzx, sRd * .25 + .25);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy * .5) / iResolution.y;

  float speed = 1.;
  vec3 o = camPath(iTime * speed);
  vec3 lk = camPath(iTime * speed + .1);
  vec3 l = camPath(iTime * speed + 1.);

  vec3 loffs = vec3(0, 1, 0);
  vec2 a = sin(vec2(1.57, 0) - l.z * 1.57 / 10.);
  loffs.xy = mat2(a, -a.y, a.x) * loffs.xy;
  l += loffs;

  float FOV = 3.14159 / 3.;
  vec3 fwd = normalize(lk - o);
  vec3 rgt = normalize(vec3(fwd.z, 0, -fwd.x));
  vec3 up = cross(fwd, rgt);

  vec3 r = normalize(fwd + FOV * (u.x * rgt + u.y * up));

  float t = trace(o, r);

  float sObjID = objID;

  vec3 col = vec3(0);
  if (t < FAR)
  {
    vec3 p = o + r * t;

    float ed;
    vec3 n = nr(p, ed);

    float sz = 2. / 1.;
    n = db(iChannel0, p * sz, n, .01 / (1. + t / FAR));

    l -= p;
    float d = max(length(l), 0.001);
    l /= d;

    float at = 1. / (1. + d * .25 + d * d * .05);

    float ao = cao(p, n);
    float sh = sha(p, l, 0.04, d, 8.);

    float di = max(dot(l, n), 0.);
    float sp = pow(max(dot(reflect(r, n), l), 0.0), 16.);

    vec3 tx = tpl(iChannel0, p * sz, n);
    if (sObjID > 1.5)
      tx *= vec3(1.5);
    else if (sObjID > .5)
      tx *= vec3(.5, .7, 1);

    col = tx * (di * sh + vec3(.75, .75, 1) * .25 + vec3(.5, .7, 1) * sp * sh * 3.);

    col *= 1. - ed * .7; // Dark edges
    //col += ed*.5; // Light edges

    vec3 em = eMap(reflect(r, n), n);
    col += col * em * 2.;

    col *= ao * at;
  }
  if (t >= FAR)
    l = normalize(l - o - r * FAR);

  vec3 bg = mix(vec3(.5, .7, 1), vec3(1, .5, .6), l.y * .5 + .5);
  col = mix(clamp(col, 0., 1.), bg, smoothstep(0., FAR - 2., t));

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
