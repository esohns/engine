uniform vec2 iResolution;
uniform float iTime;

#define SHADOWS
#define REFLECTIONS

#define R(p,a) p=cos(a)*p+sin(a)*vec2(-p.y,p.x);

const float PI = 3.141592653589793;
const float threshold = 0.001;
const float maxIters = 50.;
const float ambient = 0.2;
const float maxD = 100.;

vec3 lightPos = vec3(10.);

int m;

float
cube (vec4 cube, vec3 pos)
{
  cube.xyz -= pos;
  return max(max(abs(cube.x) - cube.w, abs(cube.y) - cube.w), abs(cube.z) - cube.w);
}

float
df (vec3 p)
{
  p += vec3(
    sin(p.z * 1.55 + iTime) + sin(p.z * 1.34 + iTime),
    0.,
    sin(p.x * 1.34 + iTime) + sin(p.x * 1.55 + iTime)
  ) * .5;
  vec3 mp = mod(p, 1.);
  mp.y = p.y;
  
  float s1 = cube(
    vec4(0.5, 0.5, 0.5, 0.15),
    vec3(mp.x, mp.y + (sin(p.z * PI) * sin(p.x * PI)) * 0.25, 0.5));
  float s2 = cube(
    vec4(0.5, 0.5, 0.5, 0.15),
    vec3(0.5, mp.y + (sin(p.x * PI) * -sin(p.z * PI)) * 0.25, mp.z));
  m = s1 < s2 ? 0 : 1;
  return min(s1, s2);
}

vec2
rm (vec3 pos, vec3 dir, float threshold, float td)
{
  vec3 startPos = pos;
  vec3 oDir = dir;
  float l, i, tl;
  l = 0.;
  
  for (float i = 0.; i <= 1.; i += 1.0 / maxIters)
  {
    l = df(pos);
    if (abs(l) < threshold)
    {
      break;
    }
    pos += (l * dir * 0.7);
  }
  l = length(startPos - pos);
  return vec2(l < td ? 1.0 : -1.0, min(l, td));
}

float
softShadow (vec3 pos, vec3 l, float r, float f, float td)
{
  float d;
  vec3 p;
  float o = 1.0, maxI = 10., or = r;
  float len;
  for (float i = 10.; i > 1.; i--)
  {
    len = (i - 1.) / maxI;
    p = pos + ((l - pos) * len);
    r = or * len;
    d = clamp(df(p), 0.0, 1.0);
    o -= d < r ? (r - d) / (r * f) : 0.;
    
    if (o < 0.)
      break;
  }
  return o;
}

void
main ()
{
  vec3 camPos = vec3(0., sin(iTime * .3) + 3., iTime);
  vec3 uv = vec3(gl_FragCoord.xy / iResolution.xy, 1.);
  vec3 rayDir = uv;
  rayDir = normalize(rayDir);
  rayDir.yz = R(rayDir.yz, sin(iTime*.25)*.25+1.25);
  rayDir.xz = R(rayDir.xz, sin(iTime*.2));

  float camDist = length(camPos);

  float gd = maxD;
  vec2 march = rm(camPos, rayDir, threshold, gd);

  int lm = m;
  vec3 point = camPos + (rayDir * march.g);
  vec2 e = vec2(0.01, 0.0);
  vec3 n = march.x == 1.0 ?
    (vec3(df(point+e.xyy), df(point+e.yxy), df(point+e.yyx))-df(point)) / e.x : vec3(0., 1., 0.);
  
  vec3 lightDir = normalize(lightPos);
  float intensity = max(dot(n, lightDir), 0.0) * 0.5;
  vec3 lightPos2 = point
  +lightDir;

#ifdef SHADOWS
  intensity *= 3.;
  intensity *= softShadow(point, point +n, 2.0, 8., gd);
#endif

  intensity -= (march.y) * 0.02;
  intensity = march.y == maxD ? 0. : intensity;
  vec4 p = vec4(1.0);
  vec3 c = clamp(lm == 0 ? vec3(sin(point)) * .25 : vec3(cos(point)), 0., 1.);

  c += 1.;
  p.rgb = march.x == -1. ? vec3(0.) : vec3(c * (intensity + ambient));

  gl_FragColor = p;

#ifdef REFLECTIONS
  vec4 p2 = vec4(0., 0., 0., 1.);
  n = normalize(n);

  vec3 refDir = rayDir - 2. * dot(n, rayDir) * n;

  point += refDir * 0.01;
  march = rm(point, refDir, threshold, maxD);
  if (march.y < 0.5)
  {
    point += (refDir * march.y);
    lm = m;
    n = (vec3(df(point+e.xyy), df(point+e.yxy), df(point+e.yyx)) -df(point)) / e.x;
    intensity = max(dot(n, lightDir), 0.0) * 0.5;
    c = clamp(lm == 0 ? vec3(sin(point)) * .25 : vec3(cos(point)), 0., 1.);
    c += 1.;
    p2.rgb = vec3(c * (intensity + ambient));
  }
  gl_FragColor = mix(p, p2, 0.2);
#endif
}
