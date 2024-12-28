//precision mediump float;

uniform float time;
uniform vec2 resolution;

#define FC gl_FragCoord.xy
#define R resolution
#define T time
#define MN min(R.x,R.y)

float
rnd (vec2 p)
{
  p = fract (p * vec2 (12.9898, 78.233));
  p += dot (p, p + 34.56);
  return fract (p.x * p.y);
}

float
sf (vec2 p, float f, float r)
{
  float k = sin (f) * 0.2;
  mat2 m = mat2 (cos (0.5 * r * T - vec4 (0, 11, 33, 0)));
  p *= 44.0 * m;
  p = vec2 (atan (p.x, p.y), length (p));
  p.y += (0.2 - k) * floor (cos (p.x *  6.0) * 15.0);
  p.y += (0.1 - k) * floor (cos (p.x * 30.0) * 10.0);
  p.y -= fract (cos (p.x * 30.0) * 2.0);
  p = vec2 (p.y * cos (p.x), p.y * sin (p.x));
  float d = length (p), t = 0.0;
  if (d < 0.8) t = 1.0;
  return t;
}

vec3
snowflake (vec2 p, float f, float r)
{
  return vec3 (sf (p - 4.0 / MN, f, r),
               sf (p, f, r),
               sf (p + 4.0 / MN, f, r));
}

vec3
snow (vec2 p, float r)
{
  vec2 id = floor (p * 0.5 + 0.5);
  p.y += T * (1.0 + rnd (id.xx)) * 0.5;
  p = mod (p + 0.5, 2.0) -1.0;
  p.x += 0.1 * cos (dot (p, vec2 (3.0)) + T * rnd (id.xx));

  return snowflake (p, 1.0, r * 0.5) +
         snowflake (p + vec2 ( 0.2, -0.1), 3.0, r) +
         snowflake (p + vec2 (-0.2,  0.4), 9.0, r * 2.0) +
         snowflake (p + vec2 (-0.2, -0.25),6.0, r * 1.2) +
         snowflake (p + vec2 ( 0.5, -0.5) ,5.0,-r * 1.3) * 2.0;
}

void
main ()
{
  vec2 uv = (FC -0.5 * R) / MN;
  vec3 col = vec3 (0.0);
  col += snow (uv * 5.0, 0.4);
  col += snow (uv * 4.0 + vec2 (0.5, 0.0), 0.8);
  col += snow (uv * 2.0 + vec2 (0.5, 0.0), 1.0);
  col = mix (col, vec3 (0.08 * rnd (uv)), min (1.0, 1.2 * length (uv - vec2 (sin (T) * 0.125, 0))));
  gl_FragColor = vec4 (col, 1.0);
}
