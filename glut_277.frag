#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iPixelDensity;
uniform sampler2D iCanvas;
uniform vec2 iMouse;
uniform float iTime;

float hash1(vec2 uv)
{
  return fract(sin(dot(uv, vec2(1234.5678, 567.8901)))*12345.67);
}

vec2 hash2(vec2 uv)
{
  float x = hash1(uv);
  return vec2(x, hash1(uv+x));
}

vec3 hash3(vec2 uv)
{
  vec2 xy = hash2(uv);
  return vec3(xy, hash1(uv+xy));
}

// Calculate smooth minimum
float smin(float a, float b, float t)
{
  float c = clamp(.5+(a-b)/t, 0., 1.);
  return (1.-c)*(a-.5*t*c)+c*b;
}

//	Input:
//		t: animate
//		mt: smooth minimum effect scale
//	Output:
//		moff: offset from current grid to the grid where the closest point is
//		mdst: distances
//		midst: interior distances
vec4 voronoi(vec2 uv, vec2 t, float mt)
{
#define TAU 6.28318530718

  vec2 fuv = fract(uv);
  vec2 iuv = floor(uv);

  vec2 moff, mdir, off, pos, dir;
  float dst, idst;

  float mdst = 8.;
  for (int x = -1; x <= 1; x++)
  for (int y = -1; y <= 1; y++) {
    off = vec2(float(x), float(y));
    pos = hash2(iuv+off);
    pos = .5+.49*sin(t+pos*TAU);
    dir = pos+off-fuv;
    dst = dot(dir, dir);
    if (dst < mdst) {
      mdst = dst;
      moff = off;
      mdir = dir;
    }
  }

  float midst = 8.;
  for (int x = -2; x <= 2; x++)
  for (int y = -2; y <= 2; y++) {
    off = moff+vec2(float(x), float(y));
    pos = hash2(iuv+off);
    pos = .5+.49*sin(t+pos*TAU);
    dir = pos+off-fuv;
    if (dot(mdir-dir, mdir-dir) > 0.00001) {
      idst = dot(.5*(mdir+dir), normalize(dir-mdir));
      midst = smin(midst, idst, abs(mt));
    }
  }
    
  return vec4(moff, mdst, midst);
}

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x) { return mod289(((x*34.0)+10.0)*x); }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v)
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

vec4 snoise3DImage(vec2 uv, float scal, float gain, float ofst, vec3 move)
{
  uv *= scal;
  float R = snoise(vec3(uv, 100.)+move);
  float G = snoise(vec3(uv, 300.)+move);
  float B = snoise(vec3(uv, 500.)+move);
  vec3 color = ofst+gain*vec3(R, G, B);
  return vec4(color, 1.);
}

vec4 snoise3DImage(vec2 uv, float scal, float gain, float ofst, float expo, vec3 move)
{
  uv *= scal;
  float R = snoise(vec3(uv, 100.)+move);
  float G = snoise(vec3(uv, 300.)+move);
  float B = snoise(vec3(uv, 500.)+move);
  vec3 col;
  col.r = pow(abs(R), expo)*(step(0., R)*2.-1.);
  col.g = pow(abs(G), expo)*(step(0., G)*2.-1.);
  col.b = pow(abs(B), expo)*(step(0., B)*2.-1.);
  return vec4(ofst+gain*col, 1.);
}

#define PI 3.141592653589793
#define TWO_PI 6.283185307179586

vec2 conical(vec2 uv, vec2 pos, float tile, float ofst)
{
  uv -= pos;
  //uv.x *= iResolution.x/iResolution.y;  // square
  vec2 radialUv = vec2(atan(uv.y, uv.x)/TWO_PI+0.5, length(uv));
  //radialUv.x += radialUv.y*1.; // Swirl
  radialUv = radialUv*tile-fract(ofst);
  return fract(radialUv);
}

float horizontal(vec2 uv, float tile, float ofst) {
  return fract( (uv*tile)-ofst ).x;
}

float radial(vec2 uv, vec2 pos, float tile, float ofst)
{
  uv -= pos;
  //uv.x *= iResolution.x/iResolution.y;  // square
  float d = length(uv);
  d = d*tile-fract(ofst);
  return fract(d);
}

float vertical(vec2 uv, float tile, float ofst)
{
  return fract( (uv*tile)-ofst ).y;
}

float pulse(float start, float end)
{
  return step(0., start) * step(end, 0.);
}

vec4 grad(float area, vec4 startCol, vec4 endCol, float startPos, float endPos)
{
  float u = pulse(area-startPos, area-endPos);
  vec4 gradientCol = mix(startCol, endCol, (area-startPos)/(endPos-startPos))*u;
  return gradientCol;
}

vec4 Palette(vec2 puv)
{
  // define your colors
  vec4 cols[5];
  cols[0] = vec4(.0, .0, .0, 1.);
  cols[1] = vec4(.0, .0, .0, 1.);
  cols[2] = vec4(1., 1., 1., 1.);
  cols[3] = vec4(.0, .0, .0, 1.);
  cols[4] = vec4(.0, .0, .0, 1.);

  // define positions of your colors (0.0 - 1.0)
  float pos[5];
  pos[0] = 0.0;
  pos[1] = 0.1;
  pos[2] = 0.5;
  pos[3] = 0.9;
  pos[4] = 1.0;

  vec4 result_color = vec4(0.);
  for (int i = 0; i < 4; i++) {
    result_color += grad(puv.x, cols[i], cols[i+1], pos[i], pos[i+1]);
  }

  return result_color;
}

vec2 rotate(vec2 uv, float angle) {
  float s = sin(angle);
  float c = cos(angle);
  mat2 rot = mat2(c, -s, s, c);
  return rot*uv;
}

float dust(vec2 uv, float oriChannel, float dustChannel, float wei) {
  oriChannel += dustChannel*wei;
  return oriChannel-wei/2.;
}

varying vec2 vTexCoord;
void main ()
{
  vec2 uv = vTexCoord;
  vec2 mo = iMouse.xy/iResolution.xy;
  uv -= .5;
  uv.x *= iResolution.x/iResolution.y;
    
  uv *= 20.-10.*sin(iTime*0.005);
    
  uv = rotate(uv, sin(iTime*0.01));
    
  vec3 col = vec3(0.);
  vec4 voro = voronoi(uv, vec2(iTime*0.025), .1);
  float noise = snoise3DImage(uv, 1000., 2., 0.5, vec3(0.)).r;
    
  float rad = radial((floor(uv)+voro.xy), vec2(0.), .05, iTime*0.005);
  rad = clamp(rad, .05, 1.);
  rad /= voro.w;
  rad -= voro.z;
  rad = dust(uv, rad, noise, 0.15);
    
  gl_FragColor = vec4(Palette(vec2(rad)).rgb, 1.);
}
