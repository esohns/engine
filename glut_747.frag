#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define AA 1

const float kE = 2.71828;
const float kPi = 3.14159265359;

const int kSteps = 246;
const float kMinD = 1.0;
const float kMaxD = 300.0;
const float kDelD = 0.01;

const vec3 kSunColor = vec3(1.0, 0.7, 0.5);
const vec3 kToSun = vec3(0.4, 0.35, -0.8);

struct Hit
{
  float Dist;
  float MaterialId;
};

vec3
noise (vec2 x)
{
  vec2 f = fract(x);
  vec2 u = f * f * (3.0 - 2.0 * f);

  ivec2 p = ivec2(floor(x));
  float a = texelFetch(iChannel0, (p + ivec2(0, 0)) & 255, 0).x;
  float b = texelFetch(iChannel0, (p + ivec2(1, 0)) & 255, 0).x;
  float c = texelFetch(iChannel0, (p + ivec2(0, 1)) & 255, 0).x;
  float d = texelFetch(iChannel0, (p + ivec2(1, 1)) & 255, 0).x;

  return vec3(a + (b - a) * u.x + (c - a) * u.y + (a - b - c + d) * u.x * u.y,
        6.0 * f * (1.0 - f) * (vec2(b - a, c - a) + (a - b - c + d) * u.yx));
}

float
noise3D (vec3 x)
{
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f * f * (3.0 - 2.0 * f);
    
  ivec3 q = ivec3(p);
  ivec2 uv = q.xy + ivec2(37, 17) * q.z;

  vec2 rg = mix(mix(texelFetch(iChannel1, (uv) & 255, 0),
                    texelFetch(iChannel1, (uv + ivec2(1, 0)) & 255, 0), f.x),
                mix(texelFetch(iChannel1, (uv + ivec2(0, 1)) & 255, 0),
                    texelFetch(iChannel1, (uv + ivec2(1, 1)) & 255, 0), f.x), f.y).yx;
    
  return mix(rg.x, rg.y, f.z);
}

float
noise3DFbm5 (vec3 p)
{
  const int iter = 5;
  float n = 0.0;
  float f = 1.0;
  float a = 1.0;
  for (int i = 0; i < iter; i++)
  {
    n += noise3D(p * f) * a;
    f *= 2.7;
    a *= 0.45;
  }
  return n / float(iter);
}

vec3
Translate (vec3 p, vec3 t)
{
  return p - t;
}

float
Cube (vec3 p, vec3 size)
{
  return length(max(abs(p) - size, 0.0));
}

float
Sphere (vec3 p, float rad)
{
  return length(p) - rad;
}

float
Torus (vec3 p, vec2 t)
{
  vec2 q = vec2(length(p.xz) - t.x, p.y);
  return length(q) - t.y;
}

float
Plane (vec3 p, vec4 n)
{
  return dot(p, n.xyz) + n.w;
}

float
Water (vec3 p)
{
  float t = iTime * 0.75;

  float wg = 0.0;
  float ag = 1.78;
  float fg = 0.15;
  for (int i = 0; i < 3; i++)
  {
    float n = noise((p.xz * fg) + t).x;
    n = -pow(n, 1.6);
    wg += n * ag;
    ag *= 0.25;
    fg *= 2.7;
  }

  float wc = 0.0;
  float ac = 0.5;
  float fc = 0.4;
  for (int i = 0; i < 5; i++)
  {
    float n = noise((p.xz * fc) + (t * 1.15)).x;
    n = (n * 2.0) - 1.0;
    wc += n * ac;
    ac *= 0.23;
    fc *= 3.0;
  }
  wc = abs(wc);

  float w = (wg) + wc;
  return w;
}

Hit
Scene (vec3 p)
{
  Hit hit;

  vec4 wDef = vec4(0.0, 1.0, 0.0, 0.0);
  float w = Plane(p, wDef);
  w += Water(p);

  hit.Dist = w;
  hit.MaterialId = 1.0;

  return hit;
}

vec3
SceneNormal (vec3 p)
{
  vec2 eps = vec2(0.001, 0.0);
  vec3 nor = vec3
    (
        Scene(p + eps.xyy).Dist - Scene(p - eps.xyy).Dist,
        Scene(p + eps.yxy).Dist - Scene(p - eps.yxy).Dist,
        Scene(p + eps.yyx).Dist - Scene(p - eps.yyx).Dist
    );
  return normalize(nor);
}

Hit
Raymarch (vec3 ro, vec3 rd)
{
  float curD = kMinD;
  for (int i = 0; i < kSteps; i++)
  {
    Hit h = Scene(ro + curD * rd);
    float dist = h.Dist;
    if (dist < kDelD)
      return Hit(curD, h.MaterialId);
    curD += dist;
    if (curD >= kMaxD)
      return Hit(kMaxD, -1.0);
  }
  return Hit(kMaxD, -1.0);
}

mat3
LookAt (vec3 origin, vec3 target, float roll)
{
  vec3 rr = vec3(sin(roll), cos(roll), 0.0);
  vec3 ww = normalize(target - origin);
  vec3 uu = normalize(cross(ww, rr));
  vec3 vv = normalize(cross(uu, ww));

  return mat3(uu, vv, ww);
}

vec4
ShadeClouds (vec3 ro, vec3 rd)
{
  float cloudH = 400.0;
    
  float minD = 50.0;
  float maxD = 5000.0;
  float delD = 10.0;
  float curD = minD;
  vec3 cloudCubeLayer = vec3(5000.0, cloudH, 5000.0);
  for (float d = minD; d < maxD; d += delD)
  {
    vec3 p = ro + rd * d;
    float c = Cube(Translate(p, vec3(0.0, 700.0, -500.0)), cloudCubeLayer);
    if (c < delD)
    {
      curD = d;
      break;
    }
  }
    
  if (curD != minD)
  {
    vec3 cP = ro + rd * curD;
    vec3 wind = vec3(iTime * 1.0, iTime * -5.0, iTime * 1.0);

    vec3 p0 = vec3(0.0, 700.0, 0.0);
    vec3 pn = vec3(0.0, 1.0, 0.0);
    float d = dot(pn, (p0 - cP)) / dot(pn, rd);

    float acumDensity = 0.0;
    float acumDist = 0.0;
    int vSteps = 64;
    float stepSize = d / float(vSteps);

    float cloudiness = 0.22;
    float scale = 0.003;

    for (int i = 0; i < vSteps; i++)
    {
      vec3 p = cP + rd * acumDist;
      acumDist += stepSize;
      float curDensity = noise3DFbm5((p + wind) * scale);
      acumDensity += clamp(curDensity - cloudiness, 0.0, 1.0);
    }

    float opacity = 1.0 / pow(kE, acumDensity);
    vec3 col = (kSunColor) + vec3(0.2);

    float fade = 1.0 - clamp(distance(ro, cP) / 4000.0, 0.0, 1.0);
    return vec4(col * 30.0, (1.0 - opacity) * fade);
  }

  return vec4(0.0, 0.0, 0.0, 0.0);
}

vec3
ShadeSky (vec3 ro, vec3 rd, bool doClouds)
{
  vec4 clouds = vec4(0.0);
  if (doClouds)
  {
    clouds = ShadeClouds(ro, rd);
    clouds.xyz *= clouds.w;
  }

  float s = pow(max(dot(normalize(kToSun), rd), 0.0), 1200.0) * 10.0;

  float h = clamp(rd.y, 0.0, 1.0);
  vec3 horizon = vec3(0.8, 0.8, 0.85);
  vec3 sky = vec3(0.0, 0.3, 0.8);
  float skyIntensity = 3.0;

  vec3 composite = (mix(horizon, sky, pow(h, 0.21)) * skyIntensity) + (kSunColor * s);
  composite += clouds.xyz;
  return composite;
}

vec3
ShadeWater (vec3 p, vec3 rd, vec3 ro)
{
  vec3 n = SceneNormal(p);
  vec3 toEye = normalize(ro - p);
  vec3 toSun = vec3(0.1, 0.5, -0.8);
  vec3 h = normalize(normalize(kToSun) - rd);

  float s = pow(max(dot(n, h), 0.0), 300.0);
  vec3 spec = kSunColor * s * 10.0;

  float lamb = max(dot(n, toSun), 0.5);

  float f = max(pow(dot(n, toEye), 0.3), 0.1);
  vec3 fres = 1.0 - (vec3(1.0) * (f + 0.15));
  fres = clamp(fres, vec3(0.0), vec3(1.0));

  vec3 wBaseColor = vec3(0.0, 0.1, 0.3) * 0.5;
  vec3 sN = normalize(n * vec3(1.0, 1.0, 1.0));
  vec3 rDir = normalize(-reflect(toEye, sN));
  vec3 reflec = ShadeSky(p, rDir, true);
  reflec = reflec / (reflec + vec3(1.0));
  reflec *= 5.0;
  vec3 opticsColor = mix(reflec, wBaseColor, clamp((1.0 - fres.x) + 0.05, 0.0, 1.0));

  vec3 wCol = (opticsColor * lamb) + spec;
  wCol += fres * 1.0;

  float distToCam = distance(p, ro) - 100.0;
  return mix(wCol, vec3(0.8, 0.8, 0.85) * 3.0, clamp(distToCam / 400.0, 0.0, 1.0));
}

vec3
Shade (vec3 p, vec3 rd, vec3 ro, Hit hit)
{
  if (hit.Dist >= kMaxD)
    return ShadeSky(ro, rd, true);

  if (int(hit.MaterialId) == 1)
    return ShadeWater(p, rd, ro);
}

void
main ()
{
  vec3 tot = vec3(0.0);
#if AA>1
    for( int m=0; m<AA; m++ )
    for( int n=0; n<AA; n++ )
    {
      vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
      vec2 uv = (-iResolution.xy + 2.0*(gl_FragCoord.xy+o))/iResolution.y;
#else  
  vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
#endif
  vec3 ro = vec3(2.0, 3.0, 30.0);

  mat3 camRot = LookAt(ro, vec3(0.0, 1.0, 0.0), 0.0);
  vec3 rd = normalize(camRot * vec3(uv, 1.6));

  vec3 color;
  Hit hit = Raymarch(ro, rd);
  vec3 p = ro + rd * hit.Dist;
  color = Shade(p, rd, ro, hit);

  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0 / 2.2));
  tot += color;
#if AA>1
    }
    tot /= float(AA*AA);
#endif
  gl_FragColor = vec4(tot, 1.0);
}
