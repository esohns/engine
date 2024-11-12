//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;
uniform vec3 iColorPhase;
uniform float iFloorUvMode;
uniform float iDisplaceWei;
uniform float iUvScale;
uniform vec2 iSeed;

varying vec2 vTexCoord;

float
hash1 (vec2 uv, vec2 seed)
{
  return fract (sin (dot (uv + seed, vec2 (1234.5678, 567.8901) + seed)) * 12345.67);
}

vec2
hash2 (vec2 uv, vec2 seed)
{
  float x = hash1 (uv, seed);
  return vec2 (x, hash1 (uv + x, seed));
}

vec3
hash3 (vec2 uv, vec2 seed)
{
  vec2 xy = hash2 (uv, seed);
  return vec3 (xy, hash1 (uv + xy, seed));
}

float
smin (float a, float b, float t)
{
  float c = clamp (.5 + (a - b) / t, 0., 1.);
  return (1. - c) * (a - .5 * t * c) + c * b;
}

//	Input:
//		t: animate
//		mt: smooth minimum effect scale
//	Output:
//		moff: offset from current grid to the grid where the closest point is
//		mdst: distances
//		midst: interior distances
vec4
voronoi (vec2 uv, vec2 t, float mt, vec2 seed)
{
#define TAU 6.28318530718

  vec2 fuv = fract (uv);
  vec2 iuv = floor (uv);

  vec2 moff, mdir, off, pos, dir;
  float dst, idst;

  float mdst = 8.;
  for (int x = -1; x <= 1; x++)
    for (int y = -1; y <= 1; y++)
    {
      off = vec2(float(x), float(y));
      pos = hash2(iuv+off, seed);
      pos = .5+.49*sin(t+pos*TAU);
      dir = pos+off-fuv;
      dst = dot(dir, dir);
      if (dst < mdst)
      {
        mdst = dst;
        moff = off;
        mdir = dir;
      }
    }
    
  float midst = 8.;
  for (int x = -2; x <= 2; x++)
    for (int y = -2; y <= 2; y++)
    {
      off = moff+vec2(float(x), float(y));
      pos = hash2(iuv+off, seed);
      pos = .5+.49*sin(t+pos*TAU);
      dir = pos+off-fuv;
      if (dot(mdir-dir, mdir-dir) > 0.00001)
      {
        idst = dot(.5*(mdir+dir), normalize(dir-mdir));
        midst = smin(midst, idst, abs(mt));
      }
    }
    
  return vec4(moff, mdst, midst);
}

vec3
palette (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b * cos (6.28318 * (c * t + d));
}

vec2
displace (vec2 uv, vec2 duv, float off, float wei)
{
  //uv.x *= iResolution.x/iResolution.y; // square
  duv -= off;
  return uv - duv * wei;
}

/*vec4
displace (vec2 uv, sampler2D img, vec2 duv, float off, float wei)
{
  duv -= off;
  return texture (img, uv - duv * wei);
}*/

void
main()
{
  vec2 t = vec2 (iTime * 0.01);

  vec2 uv = vTexCoord;
  uv -= .5;
  uv.x *= iResolution.x / iResolution.y;

  vec2 fuv;
  if (iFloorUvMode == 0.) fuv = uv.xx-uv.yy;
  if (iFloorUvMode == 1.) fuv = uv.xx+uv.yy;
  if (iFloorUvMode == 2.) fuv = uv.xx;
  if (iFloorUvMode == 3.) fuv = uv.yy;
  if (iFloorUvMode == 4.) fuv = uv;

  vec2 iuv = floor (fuv * iUvScale);
  vec2 dimg = hash2 (iuv, iSeed);
  uv = vec2 (uv.x, displace (uv, dimg.rg, 0.5, iDisplaceWei).y);
  uv *= 8.;

  vec4 voro = voronoi (uv, t, 0.5, iSeed);
  vec3 col = vec3 (step (.05, voro.w));
  vec3 test = col;

  vec3 pal = palette (voro.z + voro.w + sin (iTime * 0.001), vec3 (.5), vec3 (.5), vec3 (1.), iColorPhase);

  if (hash1 (floor (uv) + voro.xy, iSeed / 2.) > 0.3)
    col.rgb = mix (col.rgb, pal, col.b);

  if (hash1 ((uv) + voro.xy, iSeed) < 0.5)
  {
    col = vec3 (smoothstep (.3, 0.31, voro.w));
    test = vec3 (smoothstep (.3, 0.31, voro.w));
  }

  if (hash1 (floor (uv) + voro.xy, iSeed) < 0.5)
  {
    col *= 0.;
    test *= 0.;
  }

  gl_FragColor = vec4 (col, test.b);
}
