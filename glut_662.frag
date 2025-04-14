#version 150

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define inf                 1.0
#define MaxIters            500
#define AA                  3
#define PI                  3.141592653
#define L2(x)               dot(x, x)
#define Hyperbolic          -1.0
#define Euclidean           0.
#define Spherical           1.
#define MAX_TRACE_STEPS     500
#define MIN_TRACE_DIST      0.001
#define MAX_TRACE_DIST      100.0
#define PRECISION           0.0001
#define EdgeSize            0.005

#define moveVertexToInf

mat4 cartan;

// -1 for hyerbolic, 0 for Euclidean, 1 for spherial
vec4 vertexType;

vec2 euclideanVertex;

bool hasEuclideanVertex;

float
dihedral (float x)
{
  return x == inf ? inf : cos(PI / x);
}

vec2 cmul (vec2 p, vec2 q) { return vec2(p.x*q.x-p.y*q.y, p.x*q.y+p.y*q.x); }
vec2 cdiv(vec2 z, vec2 w) { return vec2(z.x * w.x + z.y * w.y, -z.x * w.y + z.y * w.x) / dot(w, w); }

struct Ball
{
  vec2 cen;
  float r;
  bool isplane;
  bool invert;
};

Ball[4] realBalls, virtualBalls;

Ball defaultBall() { return Ball(vec2(0), 1., false, false); }

Ball from_line(vec2 n) { return Ball(n, 0., true, false); }

Ball from_sphere(vec2 cen, float r) { return Ball(cen, r, false, false); }

void invertBall(inout Ball B) { B.invert = !B.invert; }

float
sdBall (vec2 p, Ball B)
{
  if (B.isplane)
    return dot(p, B.cen);
  float k = length(p - B.cen) - B.r;
  return B.invert ? k : -k;
}

bool insideBall (vec2 p, Ball B) { return sdBall(p, B) > 0.; }

void
checkCartan (mat4 M)
{
  const float e = 0.001;
  float det;
  det = determinant(mat3(M[1].yzw, M[2].yzw, M[3].yzw));
  vertexType.x = step(-e, det) + step(e, det) - 1.;
    
  det = determinant(mat3(M[0].xzw, M[2].xzw, M[3].xzw));
  vertexType.y = step(-e, det) + step(e, det) - 1.;

  det = determinant(mat3(M[0].xyw, M[1].xyw, M[3].xyw));
  vertexType.z = step(-e, det) + step(e, det) - 1.;

  det = determinant(mat3(M[0].xyz, M[1].xyz, M[2].xyz));
  vertexType.w = step(-e, det) + step(e, det) - 1.;
}

bool
try_reflect (inout vec2 p, Ball B, inout int count)
{
  if (insideBall(p, B))
    return true;
  vec2 cen = B.cen;
  float r = B.r;
  if (B.isplane)
  {
    p = p - 2. * dot(p, cen) * cen;
  }
  else
  {
    p = (r * r) / L2(p - cen) * (p - cen) + cen;
  }
  count += 1;
  return false;
}

vec2
getIntersection (Ball B1, Ball B2, Ball B3)
{
  vec2 d = vec2(-B3.cen.y, B3.cen.x);
  float r1 = B1.r, r2 = B2.r;
  float k = (L2(B1.cen) - L2(B2.cen) - r1 * r1 + r2 * r2) / (2. * dot(d, B1.cen - B2.cen));
  return k * d;
}

Ball
solveBall (Ball B1, Ball B2, Ball B3)
{
  vec2 d = vec2(-B3.cen.y, B3.cen.x);
  float r1 = B1.r, r2 = B2.r;
  float k = (L2(B1.cen) - L2(B2.cen) - r1 * r1 + r2 * r2) / (2. * dot(B1.cen - B2.cen, d));
  vec2 a = k * d;
  float r = sqrt(L2(a - B1.cen) - r1 * r1);
  return from_sphere(a, r);
}

float
sdistanceToBall (vec2 p, Ball B)
{
  if (B.isplane)
  {
    float k = dot(vec3(p, 1), vec3(B.cen, B.r));
    return k;
  }

  float k = length(p - B.cen) - B.r;
  return B.invert ? -k : k;
}

void
initLevel2Graph (float a0, float a1, float a2, float a3, float a4, float a5)
{
  Ball B0, B1, B2, B3;
  float c01 = dihedral(a0);
  float c02 = dihedral(a1);
  float c03 = dihedral(a2);
  float c12 = dihedral(a3);
  float c13 = dihedral(a4);
  float c23 = dihedral(a5);

  cartan = mat4(  1, -c01, -c02, -c03,
                -c01,    1, -c12, -c13,
                -c02, -c12,    1, -c23,
                -c03, -c13, -c23,    1);

  checkCartan(cartan);

  float s23 = sqrt(1. - c23 * c23);
  B2 = from_line(vec2(1, 0));
  B3 = from_line(vec2(-c23, s23));

  float k1 = c12;
  float k2 = (c13 + c23 * c12) / s23;
  float r = 1. / sqrt(k1 * k1 + k2 * k2 - 1.);
  B1 = from_sphere(vec2(k1 * r, k2 * r), r);
  invertBall(B1);

  k1 = c02;
  k2 = (c03 + c23 * c02) / s23;
  float a = k1 * k1 + k2 * k2 - 1.;
  float b = dot(vec3(k1, k2, c01), vec3(B1.cen, B1.r));
  float c = L2(B1.cen) - B1.r * B1.r;
  r = b / a - sqrt(b * b - a * c) / a;
  B0 = from_sphere(vec2(k1 * r, k2 * r), r);
  invertBall(B0);

  virtualBalls = Ball[4] (B0, B1, B2, B3);

  for (int k = 0; k < 4; k++)
    realBalls[k] = defaultBall();
  invertBall(realBalls[0]);

  if (vertexType.y == Hyperbolic)
  {
    float r = sqrt(L2(B0.cen) - B0.r * B0.r);
    realBalls[1] = from_sphere(vec2(0), r);
  }
  if (vertexType.z == Hyperbolic)
    realBalls[2] = solveBall(B0, B1, B3);
  if (vertexType.w == Hyperbolic)
    realBalls[3] = solveBall(B0, B1, B2);
    
  if (vertexType.y == Euclidean)
  {
    hasEuclideanVertex = true;
    euclideanVertex = vec2(0);
    return;
  }

  if (vertexType.z == Euclidean)
  {
    hasEuclideanVertex = true;
    euclideanVertex = getIntersection(B0, B1, B3);
    return;
  }

  if (vertexType.w == Euclidean)
  {
    hasEuclideanVertex = true;
    euclideanVertex = getIntersection(B0, B1, B2);
    return;
  }
}

vec2
applyMobius(vec2 p)
{
  if (hasEuclideanVertex)
  {
      vec2 A = euclideanVertex;
      vec2 B = vec2(0, 0);
      vec2 C = vec2(1, 0);
      vec2 D = vec2(4, 0);
      p = cdiv(cmul(p, A) + B, cmul(C, p) + D);
  }
  return p;
}

void
fold (inout vec2 p, inout int count, inout int index)
{
#ifdef moveVertexToInf
  p = applyMobius(p);
#endif

  for (int i = 0; i < MaxIters; i++)
  {
    bool found = true;
    for (int k = 0; k < 4; k++)
      found = try_reflect(p, virtualBalls[k], count) && found;
    if (found)
      break;
  }
  for (int k = 0; k < 4; k++)
    if (vertexType[k] == Hyperbolic && insideBall(p, realBalls[k]))
    {
      index = k;
      break;
    }
}

float
dist2Mirrors (vec2 p, inout float lw)
{
  float d = 1e30;
  for (int i = 0; i < 4; i++)
    d = min(d, abs(sdBall(p, virtualBalls[i])));
  if (L2(p) > 1.)
    lw *= 3. * L2(p);
  return d;
}

//#define useMouseInvert
//#define config      initLevel2Graph(4., 4., 4., 4., 4., 4.)
#define config        initLevel2Graph(4., 2., inf, 4., 3., 3.)

const float sphereSize = 2.25;

float sdSphere (vec3 p) { return length(p - vec3(0, 0, sphereSize)) - sphereSize; }

float sdPlane (vec3 p) { return p.z; }

vec2 rot2d (vec2 p, float a) { return p * cos(a) + vec2(-p.y, p.x) * sin(a); }

vec3
colormap (int index, float t)
{
  float c = float(index) + 1.;
  return .5 + .45 * cos(PI * pow(t, 0.3) * 1.6 * c + vec3(0, 1, 2));
}

vec3
getFloorColor (inout vec2 p)
{
  int index = -1;
  int count = 0;
  fold(p, count, index);
  float lw = EdgeSize;
  float dist = dist2Mirrors(p, lw);
  float t = clamp(float(count + 1) / float(MaxIters + 1), 0., 1.);
  vec3 col = colormap(index, t);
  float aa = fwidth(dist) / 2.;
  col = mix(col, vec3(0), 1. - smoothstep(lw - aa, lw + aa, dist));
  return col;
}

vec2
sphereToPlane (vec3 p)
{
  float k = 2. * sphereSize / (2. * sphereSize - p.z);
  return k * p.xy;
}

vec2
map (vec3 p)
{
  float d1 = sdSphere(p);
  float d2 = sdPlane(p);
  float id = (d1 < d2) ? 0.: 1.;
  return vec2(min(d1, d2), id);
}

vec3
getNormal (vec3 p)
{
  const vec2 e = vec2(0.001, 0.);
  return normalize(
      vec3(map(p + e.xyy).x - map(p  - e.xyy).x,
           map(p + e.yxy).x - map(p  - e.yxy).x,
           map(p + e.yyx).x - map(p  - e.yyx).x
          )
      );
}

vec2
raymarch (vec3 ro, vec3 rd)
{
  float t = MIN_TRACE_DIST;
  vec2 h;
  for(int i=0; i<MAX_TRACE_STEPS; i++)
  {
    h = map(ro + t * rd);
    if (h.x < PRECISION * (t*0.125+1.))
      return vec2(t, h.y);

    if (t > MAX_TRACE_DIST)
      break;

    t += h.x;
  }

  return vec2(-1.0);
}

float
calcOcclusion (vec3 p, vec3 n)
{
  float occ = 0.0;
  float sca = 1.0;
  for (int i = 0; i < 5; i++)
  {
    float h = 0.01 + 0.15 * float(i) / 4.0;
    float d = map(p + h * n).x;
    occ += (h - d) * sca;
    sca *= 0.85;
  }

  return clamp(1.0 - occ, 0.2, 1.0);
}

float
softShadow (vec3 ro, vec3 rd, float tmin, float tmax, float k)
{
  float res = 1.0;
  float t = tmin;
  for (int i = 0; i < 30; i++)
  {
    float h = map(ro + rd * t).x;
    res = min(res, k * h / t);
    t += clamp(h, 0.001, 0.1);
    if (h < 0.0001 || t > tmax)
      break;
  }

  return clamp(res, 0., 1.0);
}

vec3
getColor (vec3 ro, vec3 rd, vec3 pos, vec3 nor, vec3 lp, vec3 basecol)
{
  vec3 col = vec3(0.0);
  vec3 ld = lp - pos;
  float lDist = max(length(ld), .001);
  ld /= lDist;
  float ao = calcOcclusion(pos, nor);
  float sh = softShadow(pos+0.001*nor, ld, 0.01, lDist, 32.);
  sh = min(sh + ao * 0.2, 1.);

  float diff = clamp(dot(nor, ld), 0., 1.);
  float atten = 1. / (1. + lDist * 0.01 + lDist * lDist * 0.002);
  float spec = pow(max( dot( reflect(-ld, nor), -rd ), 0.0 ), 20.);
  float fres = clamp(1.0 + dot(rd, nor), 0.0, 1.0);
  col += basecol * (diff + .5 * ao);
  col += basecol * vec3(.8, 1., 0.3) * spec * 8.;
  col += basecol * vec3(1.2, 1., .8) * fres * fres * 6.;
  col *= atten * sh * ao;
  return col;
}

void
main ()
{
  vec3 scenecol = vec3(0.);

  config;

  vec3 camera = vec3(0.1, 10., 10.);
  vec3 lp =  vec3(1., 0., 20.);
  camera.xy = rot2d(camera.xy, iTime*0.3);
  vec3 lookat  = vec3(0.);
  vec3 up = vec3(0, 0, 1);
  vec3 forward = normalize(lookat - camera);
  vec3 right = normalize(cross(forward, up));
  up = normalize(cross(right, forward));

  for(int ii=0; ii<AA; ii++)
    for(int jj=0; jj<AA; jj++)
    {
      vec2 o = vec2(float(ii), float(jj)) / float(AA);
      vec2 uv = (2. * gl_FragCoord.xy + o - iResolution.xy) / iResolution.y;
      vec3 rd = normalize(uv.x * right + uv.y * up + 1.5 * forward);
      vec2 res = raymarch(camera, rd);
      float t = res.x;
      float id = res.y;
      vec3 pos = camera + t * rd;

      vec3 col;
      vec2 p0;
      vec3 nor;

      if (id == 0.)
      {
        nor = normalize(pos - vec3(0, 0, sphereSize));
        p0 = sphereToPlane(pos);
      }
      else if (id == 1.)
      {
        nor = vec3(0, 0, 1);
        p0 = pos.xy;
      }

#ifdef useMouseInvert      
      vec2 mouse = (iMouse.xy-2.*iResolution.xy)/iResolution.y;
      if (iMouse.z == 0.)
        mouse = vec2(1.5, 1.5);
      mouse = rot2d(mouse, iTime*0.1);
      float k = 1./L2(mouse);
      vec2 invCtr = k * mouse;
      float tt = (k - 1.) / L2(p0 -invCtr);
      p0 = tt*p0 + (1. - tt)*invCtr;
      p0.x = -p0.x;
#endif
      vec3 basecol = getFloorColor(p0);
      col = getColor(camera, rd, pos, nor, lp, basecol);
      col *= max(0.0, min(1.1, 170. / dot(pos,pos)) - 0.1);
      scenecol += col;
    }
  scenecol /= (float(AA) * float(AA));
  gl_FragColor = vec4(sqrt(clamp(scenecol, 0.0, 1.0)), 1.0);
}
