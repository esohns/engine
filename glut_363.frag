//precision mediump float;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

const float pi = 3.14159;
float rect(vec2 p, vec2 a)
{
  return max(abs(p.x - a.x), abs(p.y - a.y));
}

vec3 getRGB (float h, float s, float b)
{
  vec3 c = vec3(h, s, b);
  vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
  rgb = rgb * rgb * (3.0 - 2.0 * rgb);
  return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 hexa (vec2 p, float a)
{
  float s = p.x / a - p.y / (sqrt(3.0) * a);
  float t = 2.0 * p.y / (sqrt(3.0) * a);
  vec2 q = vec2(s, t);
  vec2 i = floor(q);
  vec2 f = fract(q);

  vec2 cell_i;
  vec2 g = vec2(2.0 * f.x + f.y, f.x + 2.0 * f.y);
  if (g.x < 1.0 && g.y < 1.0)
  {
    cell_i = i;
  }
  else if (g.x > 2.0 && g.y > 2.0)
  {
    cell_i = i + vec2(1.0, 1.0);
  }
  else
  {
    if (f.x > f.y)
    {
      cell_i = i + vec2(1.0, 0.0);
    }
    else
    {
      cell_i = i + vec2(0.0, 1.0);
    }
  }

  vec2 e1 = vec2(a, 0.0);
  vec2 e2 = vec2(a * 0.5, a * 0.5 * sqrt(3.0));
  vec2 cell = cell_i.x * e1 + cell_i.y * e2;
  vec2 u0 = vec2(1.0, 0.0);
  vec2 u1 = vec2(0.5, sqrt(3.0) * 0.5);
  vec2 u2 = vec2(-0.5, sqrt(3.0) * 0.5);

  float d = abs(dot(p - cell, u0));
  d = max(d, abs(dot(p - cell, u1)));
  d = max(d, abs(dot(p - cell, u2)));
  d *= 2.0 / a;
  d = d * d;
  return getRGB (cell_i.x * 0.02 + mod(u_time * 0.1, 1.0), 1.0 - d, 0.93);
}

void
main (void)
{
  vec2 p = (gl_FragCoord.xy - u_resolution) / min(u_resolution.x, u_resolution.y);
  float scale = 0.1 + u_mouse.x * 0.3;
  float t = pi * (1.0 + 2.0 * u_mouse.y);
  mat2 m = mat2(cos(t), sin(t), -sin(t), cos(t));
  p = m * p;
  vec3 color = hexa(p, scale);
  float diff = abs(p.x * p.y) * 0.4 + 0.6;
  gl_FragColor = vec4(color, 1.0) * diff;
}
