//precision highp float;

uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

varying vec2 vTexCoord;

#define PI 3.14159265359

float a = 1.5;
float b = 2.0;
float m = 1.0;
float n = 2.0;
float p1 = 1.0;
float p2 = 1.0;
float p3 = -1.0;
float p4 = -1.0;

float
Chladni (float a, float b, float m, float n, vec4 p, vec2 z, float t)
{
  float v1 = a * sin (PI * n * z.x + p.x * t) * sin (PI * m * z.y + p.y * t);
  float v2 = b * sin (PI * m * z.x + p.z * t) * sin (PI * n * z.y + p.w * t);
  return v1 + v2;
}

float
map (float value, float min1, float max1, float min2, float max2)
{
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

vec3
cubehelix (vec3 c)
{
  float a = c.y * c.z * (1.0 - c.z);
  float cosh = cos (c.x + PI / 2.0);
  float sinh = sin (c.x + PI / 2.0);
  return vec3 ((c.z + a * (1.78277 * sinh - 0.14861 * cosh)),
               (c.z - a * (0.29227 * cosh + 0.90649 * sinh)),
               (c.z + a * (1.97294 * cosh)));
}

vec3
cubehelixDefault (float t)
{
  return cubehelix (vec3 (mix (300.0 / 180.0 * PI, -240.0 / 180.0 * PI, t), 0.5, t));
}

vec3
cubehelixRainbow (float t)
{
  if (t < 0.0 || t > 1.0)
    t -= floor (t);
  float ts = abs (t - 0.5);
  return cubehelix (vec3 ((360.0 * t - 100.0) / 180.0 * PI, 1.5 - 1.5 * ts, 0.8 - 0.9 * ts));
}

vec3 palette (float t)
{
  vec3 a = vec3 (0.8, 0.5, 0.4);
  vec3 b = vec3 (0.2, 0.4, 0.2);
  vec3 c = vec3 (2.0, 1.0, 1.0);
  vec3 d = vec3 (0.00, 0.25, 0.25);

  return a + b * cos (2.0 * PI * (c * t + d));
}

void
main ()
{
  // Normalized pixel coordinates (from 0 to 1)
  vec2 uv = vTexCoord * 2.0 - 1.0;

  // Define a scale factor
  const float scale = 1.5;

  // Adjust for aspect ratio and scale the coordinates
  uv.x *= scale * iResolution.x / iResolution.y;
  uv.y *= scale;

  vec4 po = vec4 (p1, p2, p3, p4);
  float col = Chladni (a, b, m, n, po, uv, iTime * 0.3);

  float c1 = map (col, -1.0, 1.0, 0.0, 1.0);

  // Output the final color with full opacity
  //gl_FragColor = vec4 (cubehelixDefault (c1), 1.0);
  gl_FragColor = vec4 (cubehelixRainbow (c1), 1.0);
  //gl_FragColor = vec4 (palette (c1), 1.0);
}
