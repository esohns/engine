//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;

varying vec2 vTexCoord;

#define PI 3.14159265359

// GLSL implementation of log and atan2
float
myLog (vec2 uv, vec2 center)
{
  vec2 offset = uv - center;
  return 6.0 * log (length (offset));
}

float
myAtan2 (vec2 uv, vec2 center)
{
  vec2 offset = uv - center;
  return 6.0 * atan (offset.y, offset.x);
}

// The pattern function based on your formula
float
funPattern (float logVal, float atanVal, float t)
{
  return cos (cos (logVal - t) + 2.0 * cos (atanVal + t - abs (sin (logVal - t))));
}

float
funRadius (float patternValue, float diameter)
{
  return diameter * abs (2.0 / (1.0 + exp (-5.0 * patternValue)) - 1.0);
}

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
  if (t < 0.0 || t > 1.0) t -= floor (t);
  float ts = abs (t - 0.5);
  return cubehelix (vec3 ((360.0 * t - 100.0) / 180.0 * PI, 1.5 - 1.5 * ts, 0.8 - 0.9 * ts));
}

void
main ()
{
  vec2 uv = vTexCoord * 2.0 - 1.0;
  uv.x *= iResolution.x / iResolution.y;

  vec2 center = vec2 (0.0, 0.0);
  // float diameter = 9.0 / iResolution.x; // Normalize diameter

  float logVal = myLog (uv, center);
  float atanVal = myAtan2 (uv, center);
  float patternValue = funPattern (logVal, atanVal, iTime);
  // float r = funRadius(patternValue, diameter);

  vec3 color;
  if (patternValue < 0.0)
  {
    float hue = mix (0.0, 1.0, (patternValue + 1.0) / 1.0);
    //color = hsb2rgb(hue, 1.0, 1.0); // Usual HSV color
		color = cubehelixRainbow (hue);
  }
  else color = vec3 (0.15); // Dark gray for positive pattern value

  gl_FragColor = vec4 (color, 1.0);
}
