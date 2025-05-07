#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

float
hash21 (vec2 p)
{
  p = mod(p, 64.);
  return fract(sin(dot(p, vec2(1.27, 113.93))) * 43758.5453);
}

vec2
hash22 (vec2 p)
{
  p = mod(p, 64.);
  float n = sin(dot(p, vec2(113, 1)));
  return fract(vec2(2097152, 262144) * n);
}

float
Voronoi (vec2 p)
{
  vec2 g = floor(p), o;
  p -= g;

  vec2 d = vec2(2);

  for (int y = -1; y <= 1; y++)
    for (int x = -1; x <= 1; x++)
    {
            
      o = vec2(x, y);
      o += hash22(g + o) - p;
            
      float h = dot(o, o);
      d.y = max(d.x, min(d.y, h));
      d.x = min(d.x, h);
    }

  return min(sqrt(d.x), 1.);
}

vec4
tx (vec2 p)
{
  return texture(iChannel0, p);
}

vec4
Laplacian (vec2 p)
{
  const int mDim = 5, halfDim = (mDim - 1) / 2;

  float kernel[mDim * mDim] = float[mDim * mDim] (
        0., 0., .25, 0., 0.,
        0., .25, .5, .25, 0.,
        .25, .5, -4., .5, .25,
        0., .25, .5, .25, 0.,
        0., 0., .25, 0., 0.
  );

  vec4 col = vec4(0);

  float px = 1. / iResolution.y;

  for (int j = 0; j < mDim; j++)
    for (int i = 0; i < mDim; i++)
      col += kernel[j * mDim + i] * tx(p + vec2(i - halfDim, j - halfDim) * px);

  return col;
}

void
main ()
{
  vec2 p = gl_FragCoord.xy / iResolution.xy;

  vec4 rdVal = texture(iChannel0, p);

  vec2 lap = Laplacian(p).xy;

  const float feed = 0.0545, kill = 0.062;

  vec2 dAB = vec2(.2, .106);

  const float t = 1.;
    
  vec2 diffusion = dAB * lap;

  vec2 reaction = vec2(rdVal.x * rdVal.y * rdVal.y) * vec2(-1, 1);

  vec2 feedKill = vec2(feed * (1. - rdVal.x), (kill + feed) * rdVal.y) * vec2(1, -1);

  vec2 delta = diffusion + reaction + feedKill;
    
  gl_FragColor.xy = clamp(rdVal.xy + delta * t, 0., 1.);
  gl_FragColor.zw = iResolution.xy;

  if (iFrame < 10 || abs(rdVal.w - iResolution.y) > .001)
    gl_FragColor.xy = vec2(1, 0) + (vec2(Voronoi(p * 64.), Voronoi(p * 64. + vec2(2.93, 19.37))) - .5);
}
