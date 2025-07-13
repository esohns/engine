#version 130

uniform vec2 iResolution;
uniform float iTime;

uint
hash (uvec2 src)
{
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M; src ^= src>>24u; src *= M;
  h *= M; h ^= src.x; h *= M; h ^= src.y;
  h ^= h>>13u; h *= M; h ^= h>>15u;
  return h;
}

vec3
sRGBToLinear (vec3 rgb)
{
  return mix(pow((rgb + 0.055) * (1.0 / 1.055), vec3(2.4)),
             rgb * (1.0/12.92),
             lessThanEqual(rgb, vec3(0.04045)));
}

vec3
LinearToSRGB (vec3 rgb)
{
  return mix(1.055 * pow(rgb, vec3(1.0 / 2.4)) - 0.055,
             rgb * 12.92,
             lessThanEqual(rgb, vec3(0.0031308)));
}

vec3 a = vec3(0.5, 0.5, 0.5);
vec3 b = vec3(0.5, 0.5, 0.5);
vec3 c = vec3(0.8, 0.8, 0.5);
vec3 d = vec3(0.0, 0.2, 0.5);
float pi = 3.1415926;

#define rot(t) mat2(cos(t), sin(t), -sin(t), cos(t))

float
triangle (float t, float p, float a)
{
  float m = mod(t, p), hp = p * .5;
  float s = step(hp, m);
  return (m * (1. - s) + (p - m) * s) / hp * a;
}

vec3
color (float t)
{
  return a + b * cos(2. * pi * (c * t + d));
}

vec4
truchet (vec2 p, float r, float w)
{
  float hr = r / 2.;
  float hw = w / 2.;

  vec2 b = floor(p / r) * r;
  vec2 c = b + vec2(hr);
  float h = float(hash(uvec2(abs(c * 100.))) % 4u);
  float i = 3. - h;

  vec2 a0 = b + vec2(mod(h, 2.) * r, floor(h / 2.) * r);
  vec2 a1 = b + vec2(mod(i, 2.) * r, floor(i / 2.) * r);

  float d1 = distance(p, a0);
  float d2 = distance(p, a1);

  float d = min(d1, d2);

  vec4 glow = vec4(
    color(triangle(iTime * 0.125, 3., 1.)),
    (1. - (abs(d - hr) - hw) / w) * 0.5
  );

  return mix(vec4(1.), glow, smoothstep(hw, hw + 1.414, abs(d - hr)));
}

void
main ()
{
  vec4 f_color;
  vec2 f_coord = gl_FragCoord.xy;

  vec2 uv = (2.*f_coord - iResolution) / iResolution.y;
  uv *= 900. / 2.;
  uv = rot(iTime * 0.125) * uv;

  vec3 final = vec3(0.);
  for (float i = 3.; i > -0.1; i--)
  {
    float t = mod(iTime, 2.) * 100.;
    vec4 truc = truchet(
      (uv + vec2(sin(iTime * 1.3) * (400. - 400./6. * i ), cos(iTime * 0.7) * (400. - 400./6. * i))),
      80. - i * 20.,
      10. - i * 2.
      ) * (4. - i * 0.8) * 0.25;

    final = final * (1. - truc.a) + clamp(truc.rgb, 0., 1.) * truc.a;
  }
  f_color = vec4(final, 1.);

  gl_FragColor = f_color;
}
