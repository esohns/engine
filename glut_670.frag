#version 130

// glut_670_common.glsl
const float pi = radians(180.0);
const float twoPi = radians(360.0);

mat2
rotate (float theta)
{
  vec2 cs = vec2(cos(theta), sin(theta));
  return mat2(cs.x, cs.y, -cs.y, cs.x);
}

mat3
xyRotate (float theta)
{
  vec2 cs = vec2(cos(theta), sin(theta));
  return mat3(
      cs.x, cs.y, 0,
      -cs.y, cs.x, 0,
      0, 0, 1);
}

mat3
yzRotate (float theta)
{
  vec2 cs = vec2(cos(theta), sin(theta));
  return mat3(
      1, 0, 0,
      0, cs.x, cs.y,
      0, -cs.y, cs.x);
}

mat3
xzRotate (float theta)
{
  vec2 cs = vec2(cos(theta), sin(theta));
  return mat3(
      cs.x, 0, cs.y,
      0, 1, 0,
      -cs.y, 0, cs.x);
}
// glut_670_common.glsl

uniform vec2 iResolution;
uniform float iTime;

//#define CIRCLEORBIT

const float maxIters = 400.0;

const vec2 zoomCenter = 0.5 * vec2(
    -0.111792418416	- 0.105212213568,
    -0.902231313283 - 0.899972653438);

vec2
cSquare (vec2 c)
{
  return vec2(c.x*c.x - c.y*c.y, 2.0*c.x*c.y);
}

float
orbit (vec2 point)
{
#ifdef CIRCLEORBIT
  return dot(point, point);
#endif
  point.y = abs(point.y);
  return max(point.x, dot(point, vec2(-0.5, 0.5 * sqrt(3.0))));
}

vec2
mandelbrot (vec2 C)
{
  vec2 z = C;
  float zz; // z . z
  float orbitDist = 100.0;
  float iters;
  const float bound = 32.0;

  const float smoothIterOffset = -log2(log2(bound)) - 2.0;

  for (iters = 0.0; iters < maxIters; iters++)
  {
    z = cSquare(z) + C;
    zz = dot(z, z);
    if (zz >= bound * bound)
        break;
    orbitDist = min(orbitDist, orbit(z));
  }
  if (iters != maxIters)
    iters -= (log2(log2(zz)) + smoothIterOffset);

  return vec2(iters, orbitDist);
}

void
main ()
{
  vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / min(iResolution.x, iResolution.y);

  float smoothTime;
  if (iTime < 2.0)
    smoothTime = 0.25 * iTime * iTime;
  else
    smoothTime = iTime - 1.0;
  float rotTime = 0.11 * smoothTime;
  float zoom = exp2(-10.0 * (0.5 - 0.5 * cos(0.16 * iTime)));

  vec2 center = mix(zoomCenter, vec2(-0.6, 0.0), pow(zoom, 1.3));
  vec2 point = center + 1.35 * rotate(rotTime) * uv * zoom;
  vec2 result = mandelbrot(point);

#ifdef CIRCLEORBIT
  float height = log2(result.x) / log2(maxIters) + 0.05 * cos(2.0 * log2(result.y));
#else
  float height = log2(result.x) / log2(maxIters) + 0.05 * cos(3.0 * log2(result.y));
#endif

  float h1 = floor(height * 1024.0) / 1024.0;
  float h2 = fract(height * 1024.0);
  gl_FragColor = vec4(h1, h2, 1, 1);
}
