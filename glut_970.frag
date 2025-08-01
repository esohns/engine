// glut_970_common.glsl
const float PI = 3.14159265358979323846;

float angles[7];
float edges[7];
vec2 P[7];
vec2 O[8];
int index[8];

vec2 hypTranslate (vec2 z1, vec2 z);

vec2
complexMultiply (vec2 z1, vec2 z2)
{
  return vec2(z1.x * z2.x - z1.y * z2.y, z1.x * z2.y + z1.y * z2.x);
}

vec2
complexConjugate (vec2 z)
{
  return vec2(z.x, -z.y);
}

vec2
complexDivision (vec2 z1, vec2 z2)
{
  float denominator = z2.x * z2.x + z2.y * z2.y;

  return vec2(
    (z1.x * z2.x + z1.y * z2.y) / denominator,
    (z1.y * z2.x - z1.x * z2.y) / denominator
  );
}

float
hypDist (vec2 z1, vec2 z2)
{
  float len1 = length(z1);
  float len2 = length(z2);
  float numerator = length(z1 - z2) + sqrt(len1 * len1 * len2 * len2 - 2.0 * dot(z1, z2) + 1.0);
  float denominator = sqrt((1.0 - len1 * len1) * (1.0 - len2 * len2));

  return 2.0 * log(numerator / denominator);
}

float
hypDist (vec2 z1, vec2 z2, vec2 z)
{
  vec2 w2 = hypTranslate(z1, z2);
  vec2 w = hypTranslate(z1, z);
  w = complexMultiply(normalize(complexConjugate(w2)), w);

  float x = (exp(length(w)) - exp(-length(w))) / 2.0;
  x *= w.y / length(w);

  return log(x + sqrt(1.0 + x * x));
}

float
hypGeodesic (vec2 uv, vec2 z1, vec2 z2)
{
  return smoothstep(0.0, 0.01, hypDist(z1, z2, uv));
}

vec2
hypMid (vec2 z1, vec2 z2)
{
  vec2 w = hypTranslate(z1, z2);
  float r = length(w);
  float t = atan(w.y, w.x);

  float s = r / (1.0 + sqrt(1.0 - r * r));
  vec2 ww = s * vec2(cos(t), sin(t));

  return hypTranslate(-z1, ww);
}

float
hypAngle (vec2 z0, vec2 z1, vec2 z2)
{
  vec2 w1 = hypTranslate(z0, z1);
  vec2 w2 = hypTranslate(z0, z2);
  float angle = atan(w1.y, w1.x) - atan(w2.y, w2.x);

  return mod(angle, 2.0 * PI);
}

vec2
hypReflect (vec2 center, float radius, vec2 z)
{
  vec2 q = z - center;
  q = normalize(q) * (radius * radius / length(q));

  return q + center;
}

vec2
hypReflect (vec2 z1, vec2 z2, vec2 z)
{
  float len1 = length(z1);
  float len2 = length(z2);
  float denominator = z1.x * z2.y - z2.x * z1.y;

  float dd = (z1.y * (1.0 + len2 * len2) - z2.y * (1.0 + len1 * len1)) / denominator;
  float ee = (z2.x * (1.0 + len1 * len1) - z1.x * (1.0 + len2 * len2)) / denominator;

  vec2 center = -vec2(dd, ee) / 2.0;
  float radius = sqrt(length(center) * length(center) - 1.0);

  return hypReflect(center, radius, z);
}

vec2
hypTranslate (vec2 z1, vec2 z)
{
  return complexDivision(z - z1, vec2(1.0, 0.0) - complexMultiply(complexConjugate(z1), z));
}

vec2
hypRotate (float theta, vec2 z)
{
  mat2 rot = mat2(cos(theta), sin(theta), -sin(theta), cos(theta));

  return rot * z;
}

vec2
hypRotate (vec2 z1, float theta, vec2 z)
{
  vec2 w = hypTranslate(z1, z);
  w = hypRotate(theta, w);
  w = hypTranslate(-z1, w);

  return w;
}

vec2
hypRotate3 (vec2 z1, vec2 z)
{
  return hypRotate(z1, 2.0 * PI / 3.0, z);
}

vec2
hypRotate2 (vec2 z1, vec2 z)
{
  return hypRotate(z1, PI, z);
}

vec2
hypTranslate (vec2 z1, vec2 z2, vec2 z3, vec2 z4, vec2 z)
{
  vec2 w2 = hypTranslate(z1, z2);

  vec2 w4 = hypTranslate(z3, z4);

  vec2 rho = complexDivision(w4, w2);
  float angle = atan(rho.y, rho.x);

  vec2 w = hypTranslate(z1, z);
  w = hypRotate(angle, w);
  w = hypTranslate(-z3, w);

  return w;
}

vec2
hypBisectReflect (vec2 z1, vec2 z2, vec2 z)
{
  vec2 m = hypMid(z1, z2);

  vec2 w1 = hypRotate(m, PI / 2.0, z1);
  vec2 w2 = hypRotate(m, PI / 2.0, z2);

  return hypReflect(w1, w2, z);
}

vec4
fixedPoints (vec2 a, vec2 b, vec2 c, vec2 d)
{
  vec2 s = hypTranslate(a, b);

  vec2 t = hypTranslate(c, d);

  vec2 r = complexDivision(t, s);

  vec2 alpha = r - complexMultiply(c, complexConjugate(a));
  vec2 beta = -complexMultiply(r, a) + c;
  vec2 gamma = -complexConjugate(a) + complexMultiply(r, complexConjugate(c));
  vec2 delta = vec2(1.0, 0.0) - complexMultiply(r, complexMultiply(a, complexConjugate(c)));

  vec2 Ddelta = complexMultiply(delta - alpha, delta - alpha) + 4.0 * complexMultiply(gamma, beta);
  float rho = sqrt(length(Ddelta));
  float theta = atan(Ddelta.y, Ddelta.x) / 2.0;

  vec2 z1 = (-(delta - alpha) + vec2(rho * cos(theta), rho * sin(theta))) / 2.0;
  z1 = complexDivision(z1, gamma);

  vec2 z2 = (-(delta - alpha) - vec2(rho * cos(theta), rho * sin(theta))) / 2.0;
  z2 = complexDivision(z2, gamma);

  return vec4(z2, z1);
}

vec3
fold (vec2 uv, vec2 a, vec2 b, vec2 c, vec2 d)
{
  vec4 fixPts = fixedPoints(a, b, c, d);
  vec2 fix1 = fixPts.xy;
  vec2 fix2 = fixPts.zw;

  vec2 aH = hypReflect(fix2, length(fix2 - fix1), a);
  vec2 bH = hypReflect(fix2, length(fix2 - fix1), b);
  vec2 cH = hypReflect(fix2, length(fix2 - fix1), c);
  vec2 uvH = hypReflect(fix2, length(fix2 - fix1), uv);

  float theta = PI / 2.0 - atan(fix2.y, fix2.x);
  mat2 rot = mat2(cos(theta), sin(theta), -sin(theta), cos(theta));

  aH = rot * (aH - fix1);
  bH = rot * (bH - fix1);
  cH = rot * (cH - fix1);
  uvH = rot * (uvH - fix1);

  float angle = atan(uvH.y, uvH.x);
  float center = (length(aH) * length(aH) - length(bH) * length(bH)) / (aH - bH).x / 2.0;
  float radius = length(aH - vec2(center, 0.0));
  float rz = cos(angle) * center + sqrt(radius * radius - center * center * sin(angle) * sin(angle));

  float lnra = log(length(aH));
  float lnrc = log(length(cH));
  float lnuv = log(length(uvH));
  float d1 = lnrc - lnra;
  float d2 = lnuv - lnra;
  float shift = log(rz) - lnra;
  float w = lnra + fract((d2 - shift) / d1) * d1 + shift;

  uvH = mat2(cos(-theta), sin(-theta), -sin(-theta), cos(-theta)) * uvH + fix1;
  uvH = normalize(uvH - fix1) * exp(w) + fix1;
  vec2 uvD = hypReflect(fix2, length(fix2 - fix1), uvH);

  return vec3(uvD, fract((d2 - shift) / d1));
}

void
collectiveRotate (int i)
{
  if (i == 0)
    return;

  vec2 Q = P[i];
  for (int j = 0; j <= i; j++)
    P[j] = hypTranslate(Q, P[j]);

  vec2 w = normalize(P[i - 1]);
  float theta = angles[i] * PI / 180.0 - atan(w.y, w.x);

  for (int j = 0; j <= i; j++)
    P[j] = hypRotate(theta, P[j]);
}

bool coordComputed;

void
coordinates ()
{
  if (coordComputed)
    return;

  P[0] = vec2(0.0, 0.0);

  for (int i = 1; i < 7; i++)
  {
    float x = edges[i-1] / 2.0;
    P[i] = vec2((exp(x) - exp(-x)) / (exp(x) + exp(-x)), 0.0);
    collectiveRotate(i);
  }

  vec2 center = (P[0] + P[1] + P[2] + P[3] + P[4] + P[5] + P[6]) / 7.0;

  for (int i = 0; i < 7; i++)
    P[i] = hypTranslate(center, P[i]);

  coordComputed = true;
}

vec2
o (int i)
{
  return O[index[i < 0 ? 7 : (i % 8)]];
}

int
xedni (int i)
{
  for (int j = 0; j < 8; j++)
    if (index[j] == i)
      return j;
  return -1;
}

vec2
moveO0 ()
{
  vec2 w = hypTranslate(o(0), o(1));
  float angle = mod(atan(w.y, w.x), 2.0 * PI);
  vec2 f = normalize(w);

  for (int i = 0; i < 7; i++)
  {
    int j = xedni(i);
    if (j < 0)
      continue;
    angle = mod(angle + hypAngle(o(j), o(j - 1), o(j + 1)), 2.0 * PI);
    f += vec2(cos(angle), sin(angle));
  }

  O[0] = hypTranslate(-o(0), 0.1 * f);
  return f;
}
// glut_970_common.glsl

uniform vec2 iResolution;
uniform float iTime;

// Choosing among four possible gluing patterns
// 1: a b c d A B C D
// 2: a b A B c d C D
// 3: a b A c B d C D
// 4: a b c A B d C D
#define pattern 4

float rr = sqrt(sqrt(2.)-1.);

void
init ()
{
  for(int i = 0; i < 8; i++)
    O[i] = rr * vec2(cos(float(i) * PI / 4.), sin(float(i) * PI / 4.));
}

float
insideFD (vec2 uv)
{
  float c = 1.;
  for(int i = 0; i < 8; i++)
    c *= hypGeodesic(uv, O[i], O[(i+1)%8]);

  return c;  
}

vec3
hsb2rgb (vec3 c)
{
  vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                            6.0)-3.0)-1.0,
                   0.0,
                   1.0);
  rgb = rgb*rgb*(3.0-2.0*rgb);

  return c.z * mix( vec3(1.0), rgb, c.y);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution-0.5;
  uv *= 2.*vec2(iResolution.x/iResolution.y,1.);
  gl_FragColor=vec4(0., 0., 0., 1.);
  float shade = 1. - smoothstep(0.95, 1.0, length(uv));

  init ();

#if pattern == 1
  for(int i = 0; i < 6; i++)
  {
    uv = fold(uv, O[0], O[1], O[5], O[4]).xy;
    uv = fold(uv, O[1], O[2], O[6], O[5]).xy;
    uv = fold(uv, O[2], O[3], O[7], O[6]).xy;
    uv = fold(uv, O[3], O[4], O[0], O[7]).xy;
  }
#elif pattern == 2
  for(int i = 0; i < 6; i++)
  {
    uv = fold(uv, O[0], O[1], O[3], O[2]).xy;
    uv = fold(uv, O[1], O[2], O[4], O[3]).xy;
    uv = fold(uv, O[4], O[5], O[7], O[6]).xy;
    uv = fold(uv, O[5], O[6], O[0], O[7]).xy;
  }
#elif pattern == 3
  for(int i = 0; i < 6; i++)
  {
    uv = fold(uv, O[0], O[1], O[3], O[2]).xy;
    uv = fold(uv, O[1], O[2], O[5], O[4]).xy;
    uv = fold(uv, O[3], O[4], O[7], O[6]).xy;
    uv = fold(uv, O[5], O[6], O[0], O[7]).xy;
  }
#elif pattern == 4
  for(int i = 0; i < 6; i++)
  {
    uv = fold(uv, O[0], O[1], O[4], O[3]).xy;
    uv = fold(uv, O[1], O[2], O[5], O[4]).xy;
    uv = fold(uv, O[2], O[3], O[7], O[6]).xy;
    uv = fold(uv, O[5], O[6], O[0], O[7]).xy;
  }
#endif

  float angle = atan(uv.y,uv.x)+iTime*PI/2.;
  float radius = length(uv)*2.;
  gl_FragColor.rgb += insideFD(uv)*hsb2rgb(vec3(angle/2./PI,radius,1.0));

  gl_FragColor *= shade;
}
