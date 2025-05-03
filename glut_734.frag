#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

#define Frame float(iFrame)
#define PixelCount iResolution.xy

#define lerp mix
#define rsqrt inversesqrt
#define clamp(x) clamp(x, 0.0, 1.0)

const float Pi = 3.14159265359;

float
Pow2 (float x)
{
  return x * x;
}

float
Pow3 (float x)
{
  return x * x * x;
}

float
Pow4 (float x)
{
  return Pow2(Pow2(x));
}

vec2
AngToVec (float ang)
{
  return vec2(cos(ang), sin(ang));
}

float
SqrLen (float v)
{
  return v * v;
}

float
SqrLen (vec2 v)
{
  return dot(v, v);
}

float
SqrLen (vec3 v)
{
  return dot(v, v);
}

float
SqrLen (vec4 v)
{
  return dot(v, v);
}

float
Pow (float x, float e)
{
  return pow(x, e);
}

vec2
Pow (vec2 x, float e)
{
  return pow(x, vec2(e));
}

vec3
Pow (vec3 x, float e)
{
  return pow(x, vec3(e));
}

vec4
Pow (vec4 x, float e)
{
  return pow(x, vec4(e));
}

float
GammaEncode (float x)
{
  return pow(x, 1.0 / 2.2);
}

vec2
GammaEncode (vec2 x)
{
  return pow(x, vec2(1.0 / 2.2));
}

vec3
GammaEncode (vec3 x)
{
  return pow(x, vec3(1.0 / 2.2));
}

vec4
GammaEncode (vec4 x)
{
  return pow(x, vec4(1.0 / 2.2));
}

float
CheapHash (float v)
{
  return fract(sin(v) * 43758.5453) * 2.0 - 1.0;
}

float
CheapHash (vec2 v)
{
  return CheapHash(v.y + v.x * 12.9898);
}

float
PlotDot (vec2 sp, vec2 dp, float dr)
{
  float v = length(sp - dp);

  float g = 1.0 / (max(0.0, v / dr - 1.0) * 8.0 + 1.0);
  g *= g;

  return g;
}

const float GAng = Pi * (3.0 - sqrt(5.0));
vec2
Sample_Disk_GS (float i, float n, float h0, float h1)
{
  float I = i + h1;

  float rad = sqrt(I / n);

  float theta = GAng * I + h0 * Pi;

  return AngToVec(theta) * rad;
}

void
main ()
{
  vec2 uv0 = gl_FragCoord.xy;
  vec2 uv = floor(uv0.xy);
  vec2 tex = uv0.xy / PixelCount;

  vec2 c = (PixelCount * 0.5);
  vec2 vec = uv - c;

  float rr = PixelCount.y * 0.4;
  float r = PixelCount.y * 0.013;

  vec3 col = vec3(0.0);
  float v = 0.0;

  const float n = 16.0;

  float h0 = Frame * 0.003;
  float h1 = 1.0 - clamp(fract((Frame) * 0.01));

  for (float i = 0.0; i < n; ++i)
  {
    vec2 off = vec2(0.0);

    off = Sample_Disk_GS(i, n, h0, h1);

    vec2 p = c + off * rr;

    float l = clamp((i + h1) / n);
    float l0 = l;

    l = clamp((1.0 - l) * 8.0);
    l = l * l * (3.0 - 2.0 * l);

    l *= 1.0 + Pow2(max(0.0, 0.4 - l0) / 0.4) * 4.0;

    float d = PlotDot(uv0, p, r);
    d *= l;

    v += d;
  }

  col = vec3(0.0, 0.01, 0.1);
  col = lerp(col, (vec3(1.0, 0.15, 0.025)) * 400.0, v);
  col += PlotDot(uv0, c, r * 6.0) * (vec3(1.0, 0.005, 0.001)) * 800.0;

  vec3 col0 = textureLod(iChannel0, tex, 0.0).rgb;

  col = lerp(col0, col, 0.02);

  gl_FragColor = vec4(col, 1.0);
}
