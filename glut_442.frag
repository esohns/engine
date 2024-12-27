//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;

varying vec2 vTexCoord;

#define PI 3.14159265359

float
funPattern (float x, float y, float t)
{
  float a = atan (y, x);
  float b = asin (sin (a - t + 3.0 * log (x * x + y * y)));
  float k = pow (2.0 * PI, 1.0 - sqrt (sin (6.0 * a) * sin (6.0 * a) + b * b));
  return 2.0 * b * cos (k) - sin (k) / (1.0 / sin (12.0 * a));
}

// Compute radius based on pattern value, I don't really need this :P
// float funRadius(float patternValue, float diameter) {
//    return diameter * abs(2.0 / (1.0 + exp(-5.0 * patternValue)) - 1.0);
//}

// Convert hue to RGB (HSB to RGB helper function)
vec3
hsb2rgb (float h, float s, float b)
{
  vec3 c = vec3 (h, s, b);
  float k = mod (c.x * 6.0, 6.0);
  float f = k - floor (k);
  float p = c.z * (1.0 - c.y);
  float q = c.z * (1.0 - f * c.y);
  float t = c.z * (1.0 - (1.0 - f) * c.y);

  if (k < 1.0) return vec3 (c.z, t, p);
  if (k < 2.0) return vec3 (q, c.z, p);
  if (k < 3.0) return vec3 (p, c.z, t);
  if (k < 4.0) return vec3 (p, q, c.z);
  if (k < 5.0) return vec3 (t, p, c.z);
  return vec3 (c.z, p, q);
}

// https://github.com/d3/d3-color
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

// https://github.com/d3/d3-scale-chromatic
vec3
cubehelixDefault (float t)
{
  return cubehelix (vec3 (mix (300.0 / 180.0 * PI, -240.0 / 180.0 * PI, t), 0.5, t));
}

// https://github.com/d3/d3-scale-chromatic
vec3
cubehelixRainbow (float t)
{
  if (t < 0.0 || t > 1.0) t -= floor (t);
  float ts = abs (t - 0.5);
  return cubehelix (vec3 ((360.0 * t - 100.0) / 180.0 * PI, 1.5 - 1.5 * ts, 0.8 - 0.9 * ts));
}

void
main ()
{
  vec2 uv = vTexCoord * 2.0 - 1.0;
  uv.x *= iResolution.x / iResolution.y;

  vec2 center = vec2 (0.0, 0.0); // Center coordinates (u, v)
  // float diameter = 9.0 / iResolution.x; // Normalize diameter

  // Compute pattern values
  float patternValue = funPattern (uv.x, uv.y, iTime);

  // Compute radius (for visualization purposes, not directly used here)
  // float r = funRadius(patternValue, diameter);

  // Assign color based on patternValue
  vec3 color;
  if (patternValue < 0.0)
  {
    float hue = mix (0.764, 1.0, (patternValue - iTime) / 1.0); // Map
    //color = hsb2rgb (hue, 1.0, 1.0); // Usual HSV color
    color = cubehelixRainbow (hue); // I like more these Rainbow colors compare to the usual HSV
  }
  else
  {
    color = vec3 (0.15); // Dark gray for positive pattern value
  }

  gl_FragColor = vec4 (color, 1.0);
}
