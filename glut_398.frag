//precision mediump float;

uniform sampler2D tex0;
uniform vec2 texelSize;
uniform vec2 canvasSize;
uniform vec2 mouse;
uniform float time;

varying vec2 vTexCoord;

float mod7(float x) { return x - floor(x * (1.0 / 7.0)) * 7.0; }
vec2  mod7(vec2  x) { return x - floor(x * (1.0 / 7.0)) * 7.0; }
vec3  mod7(vec3  x) { return x - floor(x * (1.0 / 7.0)) * 7.0; }
vec4  mod7(vec4  x) { return x - floor(x * (1.0 / 7.0)) * 7.0; }

float mod289(float x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2  mod289(vec2  x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3  mod289(vec3  x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4  mod289(vec4  x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }

float permute(float x) { return mod289(((x*34.0)+10.0)*x); }
vec2  permute(vec2  x) { return mod289(((x*34.0)+10.0)*x); }
vec3  permute(vec3  x) { return mod289(((x*34.0)+10.0)*x); }
vec4  permute(vec4  x) { return mod289(((x*34.0)+10.0)*x); }

float fade(float t) { return t*t*t*(t*(t*6.0-15.0)+10.0); }
vec2  fade(vec2  t) { return t*t*t*(t*(t*6.0-15.0)+10.0); }
vec3  fade(vec3  t) { return t*t*t*(t*(t*6.0-15.0)+10.0); }
vec4  fade(vec4  t) { return t*t*t*(t*(t*6.0-15.0)+10.0); }

float taylorInvSqrt(float r) { return 1.79284291400159 - 0.85373472095314 * r; }
vec2  taylorInvSqrt(vec2  r) { return 1.79284291400159 - 0.85373472095314 * r; }
vec3  taylorInvSqrt(vec3  r) { return 1.79284291400159 - 0.85373472095314 * r; }
vec4  taylorInvSqrt(vec4  r) { return 1.79284291400159 - 0.85373472095314 * r; }

float
snoise (vec3 v)
{
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;

  i = mod289(i);
  vec4 p = permute( permute( permute( 
              i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
            + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
            + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  float n_ = 0.142857142857;
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  vec4 m = max(0.5 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 105.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                  dot(p2,x2), dot(p3,x3) ) );
}

float
hash12 (vec2 p)
{
  vec3 p3 = fract (vec3 (p.xyx) * .1031);
  p3 += dot (p3, p3.yzx + 33.33);
  return fract ((p3.x + p3.y) * p3.z);
}

vec2 hash2 (vec2 uv) {
  float x = hash12 (uv * 314159.);
  return vec2 (x, hash12 (uv + x));
}

float
smin (float a, float b, float t)
{
  float c = clamp (.5 + (a - b) / t, 0., 1.);
  return (1. - c) * (a - .5 * t * c) + c * b;
}

vec4
snoise3DImage (vec2 uv, float scal, float gain, float ofst, float expo, vec3 move)
{
  uv *= scal;
  float R = snoise(vec3(uv, 100.)+move);
  float G = snoise(vec3(uv, 300.)+move);
  float B = snoise(vec3(uv, 500.)+move);
  vec3 col;
  col.r = pow(abs(R), expo)*(step(0., R)*2.-1.);
  col.g = pow(abs(G), expo)*(step(0., G)*2.-1.);
  col.b = pow(abs(B), expo)*(step(0., B)*2.-1.);
  return vec4 (ofst + gain * col, 1.);
}

vec4
voronoi (vec2 uv, vec2 t, float mt)
{
#ifndef TAU
#define TAU 6.283185307179586
#endif

  vec2 fuv = fract (uv);
  vec2 iuv = floor (uv);

  vec2 moff, mdir, off, pos, dir;
  float dst, idst;

  float mdst = 8.;
  for (int x = -1; x<=1; x++)
    for (int y = -1; y<=1; y++)
    {
      off = vec2(float(x), float(y));
      pos = hash2(iuv+off);
      pos = .5+.49*sin(t+pos*TAU);
      dir = pos+off-fuv;
      dst = dot(dir, dir);
      if (dst<mdst)
      {
        mdst = dst;
        moff = off;
        mdir = dir;
      }
    }

  float midst = 8.;
  for (int x = -2; x<=2; x++)
    for (int y = -2; y<=2; y++)
    {
      off = moff+vec2(float(x), float(y));
      pos = hash2(iuv+off);
      pos = .5+.49*sin(t+pos*TAU);
      dir = pos+off-fuv;
      if (dot(mdir-dir, mdir-dir)>0.00001)
      {
        idst = dot(.5*(mdir+dir), normalize(dir-mdir));
        midst = smin(midst, idst, abs(mt));
      }
    }

  return vec4 (moff, mdst, midst);
}

vec2
pixelate (vec2 uv, vec2 pos, vec2 grid)
{
  uv -= pos;
  uv *= grid;
  vec2 iuv = floor (uv) + .5; // +.5 to center to cell
  return iuv / grid;
}

vec3
dust (vec2 uv, vec3 oriChannel, vec3 dustChannel, float wei)
{
  oriChannel += dustChannel * wei;
  return oriChannel - wei / 2.0;
}

void main ()
{
  vec2 uv = (2. * gl_FragCoord.xy - canvasSize) / min (canvasSize.x, canvasSize.y);

  vec2 pos = vec2 (.5, .5);
  vec2 grid = floor (vec2 (16., 1.));
  vec4 voro = voronoi ((uv + pos) * grid, vec2 (time), 1.);

  vec2 puv = (floor ((uv + pos) * grid) + voro.xy) / grid;
  puv = pixelate (puv, vec2 (0.), grid);

  vec4 noise =      snoise3DImage (puv, 1., 1., .5, 1., vec3 (0., 0., time * .25));
  noise     += .5 * snoise3DImage (puv, 2., 4., .5, 1., vec3 (0., 0., time * .25));
  noise /= 1.5;

  vec3 col = noise.rrr + vec3 (0., .35, .5);
  col -= voro.w;
  col *= col;
  col *= voro.w;
  col /= voro.z;
  //col = clamp (vec3 (0.), vec3 (1.), col);

  float hash = hash12 (uv * 3141.1234 + 1000.8090);
  gl_FragColor = vec4 (dust (uv, col, vec3 (hash), .25), 1.);
}
