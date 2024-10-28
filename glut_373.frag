//precision highp float;
#version 330

uniform vec2 canvasSize;
uniform vec2 mouse;
uniform float time;

in vec2 vTexCoord;
out vec4 fragColor;

float
wobbly (vec2 uv, float t)
{
  float w0 = sin (0.3 * uv.x + 2.0 * t + 1.0 + 2.3 * sin (0.4 * uv.y - 2.0 * t + 0.5));
  float w1 = sin (0.2 * uv.y + 3.0 * t + 2.0 + 2.2 * sin (0.5 * uv.x - 3.0 * t + 0.4));
  return 0.5 + 0.25 * (w0 + w1);
}

float
fbm (vec2 uv, float t)
{
  float n = 0.0;
  n += 1.0   * wobbly (1.0 * uv + vec2 (100.0, 200.0), t * 0.25);
  n += 0.5   * wobbly (2.0 * uv + vec2 (300.0, 400.0), t * 0.25);
  n += 0.25  * wobbly (4.0 * uv + vec2 (500.0, 600.0), t * 0.25);
  n += 0.125 * wobbly (8.0 * uv + vec2 (700.0, 800.0), t * 0.25);
  n += 0.0625* wobbly (16.0* uv + vec2 (900.0,   0.0), t * 0.25);
  return n / 1.9375;
}

vec3
palette (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b * cos (6.28318 * (c * t + d));
}

void
main ()
{
//  vec2 uv = vTexCoord * 2.0 - 1.0; *NOTE*: already done in vertex shader
  vec2 uv = vTexCoord;
  if (canvasSize.x > canvasSize.y)
    uv.x *= canvasSize.x / canvasSize.y;
  else
    uv.y *= canvasSize.y / canvasSize.x;
  uv *= 6.0;

  float n = fbm (uv, time);
  vec2 a = vec2 (fbm (uv + 1.0 * n + vec2 (1.2, 2.3), time), fbm (uv + 1.0 * n + vec2 (3.4, 4.5), time));
  vec2 b = vec2 (fbm (uv + 2.0 * a + vec2 (5.6, 6.7), time), fbm (uv + 2.0 * a + vec2 (7.8, 8.9), time));
  float r = fbm (uv + 50.0 * b, time);
  r = n * fbm (uv + 5.0 * r, time);

  vec3 col = palette (r,
                      vec3 (1.0, 1.0, 1.0),
                      vec3 (1.5, 1.0, 1.5),
                      vec3 (1.0, 1.0, 1.0),
                      vec3 (0.2, 0.1, 0.0));

  fragColor = vec4 (col, 1.0);
}
