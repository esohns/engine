#version 330
//precision highp float;

uniform vec2 iResolution;
uniform float iTime;
//uniform vec2 iMouse;

varying vec2 vTexCoord;

#define TIME        iTime
#define RESOLUTION  iResolution

#define ROT(a)      mat2(cos(a), sin(a), -sin(a), cos(a))

const float
  pi        = acos(-1.)
, tau       = 2.*pi
, planeDist = .5
, furthest  = 16.
, fadeFrom  = 8.;

const vec2
  pathA = vec2 (.31, .41)
, pathB = vec2 (1.0, sqrt (0.5));

const vec4
  U = vec4 (0, 1, 2, 3);

vec3
aces_approx (vec3 v)
{
  v = max (v, 0.0);
  v *= 0.6;
  float a = 2.51;
  float b = 0.03;
  float c = 2.43;
  float d = 0.59;
  float e = 0.14;

  return clamp ((v * (a * v + b)) / (v * (c * v + d) + e), 0.0, 1.0);
}

vec3
offset (float z)
{
  return vec3 (pathB * sin (pathA * z), z);
}

vec3
doffset (float z)
{
  return vec3 (pathA * pathB * cos (pathA * z), 1.0);
}

vec3
ddoffset (float z)
{
  return vec3 (-pathA * pathA * pathB * sin (pathA * z), 0.0);
}

vec4
alphaBlend (vec4 back, vec4 front)
{
  float w = front.w + back.w * (1.0 - front.w);
  vec3 xyz = (front.xyz * front.w + back.xyz * back.w * (1.0 - front.w)) / w;
  return w > 0.0 ? vec4 (xyz, w) : vec4 (0.0);
}

float
pmin (float a, float b, float k)
{
  float h = clamp (0.5 + 0.5 * (b-a) / k, 0.0, 1.0);
  return mix (b, a, h) - k * h * (1.0 - h);
}

float
pmax (float a, float b, float k)
{
  return -pmin (-a, -b, k);
}

float pabs (float a, float k)
{
  return -pmin (a, -a, k);
}

float
sdOctogon (vec2 p, float r)
{
  const vec3 k = vec3 (-0.9238795325, 0.3826834323, 0.4142135623);
  p = abs (p);
  p -= 2.0 * min (dot (vec2 ( k.x, k.y), p), 0.0) * vec2 ( k.x,k.y);
  p -= 2.0 * min (dot (vec2 (-k.x, k.y), p), 0.0) * vec2 (-k.x,k.y);
  p -= vec2 (clamp (p.x, -k.z * r, k.z * r), r);
  return length (p) * sign (p.y);
}

vec3
palette (float n)
{
  return 0.6 + 0.5 * sin (vec3 (1.9, 1.5, 1.0) + n);
}

vec4
plane (vec3 ro, vec3 rd, vec3 pp, vec3 npp, float pd, vec3 cp, vec3 off, float n)
{
  float aa = 3.0 * pd * distance (pp.xy, npp.xy);
  vec4 col = vec4 (0.0);
  vec2 p2 = pp.xy;
  p2 -= offset (pp.z).xy;
  vec2 doff   = ddoffset (pp.z).xz;
  vec2 ddoff  = doffset (pp.z).xz;
  float dd = dot (doff, ddoff);
  p2 *= ROT (dd* pi * 5.0);

  float d0 = sdOctogon (p2, 0.45) - 0.02;
  float d1 = d0 - 0.01;
  float d2 = length (p2);
  const float colp = pi * 100.0;
  float colaa = aa * 200.0;

  col.xyz = palette (0.5 * n+2.0 * d2) * mix (0.5/(d2 * d2), 1., smoothstep (-0.5 + colaa, 0.5 + colaa, sin (d2 * colp))) / max (3.0 * d2 * d2, 1E-1);
  col.xyz = mix (col.xyz, vec3 (2.0), smoothstep (aa, -aa, d1)); 
  col.w = smoothstep (aa, -aa, -d0);

  return col;
}

vec3
color (vec3 ww, vec3 uu, vec3 vv, vec3 ro, vec2 p)
{
  float lp = length (p);
  vec2 np = p + 1./RESOLUTION.xy;
  float rdd = 2.0 - 0.25;

  vec3 rd =  normalize ( p.x * uu +  p.y * vv + rdd * ww);
  vec3 nrd = normalize (np.x * uu + np.y * vv + rdd * ww);

  float nz = floor (ro.z / planeDist);

  vec4 acol = vec4 (0.0);

  vec3 aro = ro;
  float apd = 0.0;

  for (float i = 1.; i <= furthest; ++i)
  {
    if (acol.w > 0.95)
    {
      // Debug col to see when exiting
      // acol.xyz = palette(i); 
      break;
    }
    float pz = planeDist*nz + planeDist*i;

    float lpd = (pz - aro.z) / rd.z;
    float npd = (pz - aro.z) / nrd.z;
    float cpd = (pz - aro.z) / ww.z;

    {
      vec3 pp = aro + rd*lpd;
      vec3 npp= aro + nrd*npd;
      vec3 cp = aro+ww*cpd;

      apd += lpd;

      vec3 off = offset (pp.z);

      float dz = pp.z - ro.z;
      float fadeIn = smoothstep (planeDist * furthest, planeDist * fadeFrom, dz);
      float fadeOut = smoothstep (0.0, planeDist * 0.1, dz);
      float fadeOutRI = smoothstep (0.0, planeDist * 1.0, dz);

      float ri = mix (1.0, 0.9, fadeOutRI * fadeIn);

      vec4 pcol = plane (ro, rd, pp, npp, apd, cp, off, nz + i);

      pcol.w *= fadeOut * fadeIn;
      acol = alphaBlend (pcol, acol);
      aro = pp;
    }
  }

  return acol.xyz * acol.w;
}

void
main ()
{
  // Map vTexCoord to normalized device coordinates (NDC) [-1, 1]
  vec2 uv = vTexCoord * 2.0 - 1.0;

  // Define a scale factor
  const float scale = 1.0;

  // Adjust for aspect ratio and scale the coordinates
  uv.x *= scale * iResolution.x / iResolution.y;
  uv.y *= scale;

  float tm  = planeDist * TIME * 0.7;

  vec3 ro   = offset (tm);
  vec3 dro  = doffset (tm);
  vec3 ddro = ddoffset (tm);

  vec3 ww = normalize (dro);
  vec3 uu = normalize (cross (U.xyx + ddro, ww));
  vec3 vv = cross (ww, uu);
  
  vec3 col = color (ww, uu, vv, ro, uv);
  col = aces_approx (col);
  col = sqrt (col);

  gl_FragColor = vec4 (col, 1);
}
