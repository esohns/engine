//precision highp float;
#version 330

in vec2 aPosition;
in vec2 aTexCoord;

uniform vec2 uRandomVec2;
uniform float uTime;

vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
float mod289(float x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x) { return mod289(((x*34.0)+10.0)*x); }
float permute(float x) { return mod289(((x*34.0)+10.0)*x); }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }
float taylorInvSqrt(float r) { return 1.79284291400159 - 0.85373472095314 * r; }

vec4
grad4 (float j, vec4 ip)
{
  const vec4 ones = vec4(1.0, 1.0, 1.0, -1.0);
  vec4 p,s;

  p.xyz = floor( fract (vec3(j) * ip.xyz) * 7.0) * ip.z - 1.0;
  p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
  s = vec4(lessThan(p, vec4(0.0)));
  p.xyz = p.xyz + (s.xyz*2.0 - 1.0) * s.www; 

  return p;
}

#define F4 0.309016994374947451

float
snoise (vec4 v)
{
  const vec4  C = vec4( 0.138196601125011,  // (5 - sqrt(5))/20  G4
                        0.276393202250021,  // 2 * G4
                        0.414589803375032,  // 3 * G4
                      -0.447213595499958);  // -1 + 4 * G4

  vec4 i  = floor(v + dot(v, vec4(F4)) );
  vec4 x0 = v -   i + dot(i, C.xxxx);

  vec4 i0;
  vec3 isX = step( x0.yzw, x0.xxx );
  vec3 isYZ = step( x0.zww, x0.yyz );
    
  i0.x = isX.x + isX.y + isX.z;
  i0.yzw = 1.0 - isX;

  i0.y += isYZ.x + isYZ.y;
  i0.zw += 1.0 - isYZ.xy;
  i0.z += isYZ.z;
  i0.w += 1.0 - isYZ.z;

  vec4 i3 = clamp( i0, 0.0, 1.0 );
  vec4 i2 = clamp( i0-1.0, 0.0, 1.0 );
  vec4 i1 = clamp( i0-2.0, 0.0, 1.0 );

  vec4 x1 = x0 - i1 + C.xxxx;
  vec4 x2 = x0 - i2 + C.yyyy;
  vec4 x3 = x0 - i3 + C.zzzz;
  vec4 x4 = x0 + C.wwww;

  i = mod289(i); 
  float j0 = permute( permute( permute( permute(i.w) + i.z) + i.y) + i.x);
  vec4 j1 = permute( permute( permute( permute (
            i.w + vec4(i1.w, i2.w, i3.w, 1.0 ))
          + i.z + vec4(i1.z, i2.z, i3.z, 1.0 ))
          + i.y + vec4(i1.y, i2.y, i3.y, 1.0 ))
          + i.x + vec4(i1.x, i2.x, i3.x, 1.0 ));

  vec4 ip = vec4(1.0/294.0, 1.0/49.0, 1.0/7.0, 0.0) ;

  vec4 p0 = grad4(j0,   ip);
  vec4 p1 = grad4(j1.x, ip);
  vec4 p2 = grad4(j1.y, ip);
  vec4 p3 = grad4(j1.z, ip);
  vec4 p4 = grad4(j1.w, ip);

  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;
  p4 *= taylorInvSqrt(dot(p4,p4));

  vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.0);
  vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.0);
  m0 = m0 * m0;
  m1 = m1 * m1;
  return 49.0 * ( dot(m0*m0, vec3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
              + dot(m1*m1, vec2( dot( p3, x3 ), dot( p4, x4 ) ) ) ) ;
}

vec4
snoise4DImage (vec2 uv, float scal, float gain, float ofst, vec4 move)
{
  uv *= scal;
  float R = snoise(vec4(uv, 100., 200.)+move);
  float G = snoise(vec4(uv, 300., 400.)+move);
  float B = snoise(vec4(uv, 500., 600.)+move);
  vec3 color = ofst+gain*vec3(R, G, B);
  return vec4(color, 1.);
}

vec4
snoise4DImage (vec2 uv, float scal, float gain, float ofst, float expo, vec4 move)
{
  uv *= scal;
  float R = snoise(vec4(uv, 100., 200.)+move);
  float G = snoise(vec4(uv, 300., 400.)+move);
  float B = snoise(vec4(uv, 500., 600.)+move);
  vec3 col;
  col.r = pow(abs(R), expo)*(step(0., R)*2.-1.);
  col.g = pow(abs(G), expo)*(step(0., G)*2.-1.);
  col.b = pow(abs(B), expo)*(step(0., B)*2.-1.);
  return vec4(ofst+gain*col, 1.);
}

vec4
noise (vec2 uv, float scal, float gain, float ofst, float expo, vec4 move)
{
  vec4 noise;
  noise  =     1.*snoise4DImage((uv-vec2(421., 132))*1., scal, gain, ofst, move);
  noise +=     .5*snoise4DImage((uv-vec2(913., 687))*2., scal, gain, ofst, move);
  noise +=    .25*snoise4DImage((uv-vec2(834., 724))*4., scal, gain, ofst, move);
  noise +=   .125*snoise4DImage((uv-vec2(125., 209))*8., scal, gain, ofst, move);
  noise +=  .0625*snoise4DImage((uv-vec2(387., 99))*16., scal, gain, ofst, move);
  noise /= 1.9375;
  return noise;
}

vec2
displace (vec2 uv, vec2 duv, float off, float wei)
{
  //uv.x *= iResolution.x/iResolution.y; // square
  duv -= off;
  return uv-duv*wei;
}

vec4
displace (vec2 uv, sampler2D img, vec2 duv, float off, float wei)
{
  duv -= off;
  return texture(img, uv-duv*wei);
}

out vec2 vTexCoord;
out vec2 vCol;

void
main ()
{
  vTexCoord = aTexCoord;

  vec2 pos = aPosition;
  float circle = smoothstep(1., .0, length(0.-aPosition));
  vec2 n = noise (pos, 2., 5., .5, 1., vec4(vec2(0.), vec2(cos(uTime*.5), sin(uTime*.5))+uRandomVec2)).rg*circle;
  vec2 dpos = displace (pos, n, .5, .2*circle);

  vCol = n.rg*noise (pos*1000., 1., 1., .5, 1., vec4(0.)).r;

  gl_Position = vec4 (dpos, 0., 1.);
  gl_PointSize = 1.;
}
