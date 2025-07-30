#version 330

// glut_963_common.glsl
const float wrap = 8.;
const float cubeMapRes = 64.;

uint
hash (uint x)
{
  x += ( x << 10u ); x ^= ( x >>  6u );
  x += ( x <<  3u ); x ^= ( x >> 11u );
  x += ( x << 15u );
  return x;
}

uint hash (uvec2 v) { return hash( v.x ^ hash(v.y) ); }
uint hash (uvec3 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ); }
uint hash (uvec4 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

float
hash41 (vec4 f)
{
  const uint mantissaMask = 0x007FFFFFu;
  const uint one          = 0x3F800000u;

  uint h = hash( floatBitsToUint( f ) );
  h &= mantissaMask;
  h |= one;

  float  r2 = uintBitsToFloat( h );
  return r2 - 1.;
}

float
hash31 (vec3 f)
{
  const uint mantissaMask = 0x007FFFFFu;
  const uint one          = 0x3F800000u;

  uint h = hash( floatBitsToUint( f ) );
  h &= mantissaMask;
  h |= one;

  float  r2 = uintBitsToFloat( h );
  return r2 - 1.;
}

float
hash21 (vec2 f)
{
  const uint mantissaMask = 0x007FFFFFu;
  const uint one          = 0x3F800000u;

  uint h = hash( floatBitsToUint( f ) );
  h &= mantissaMask;
  h |= one;

  float  r2 = uintBitsToFloat( h );
  return r2 - 1.;
}

vec3
convertCoord (vec2 p)
{
  p = mod(floor(p), cubeMapRes); 

  float i = p.x + p.y*cubeMapRes;

  return mod(vec3(i, floor(i/wrap), floor(i/(wrap*wrap))), wrap);
}

vec2
convertCoord (vec3 p)
{
  p = mod(floor(p), wrap);

  float i = p.x + (p.y + p.z*wrap)*wrap;

  return mod(vec2(i, floor(i/cubeMapRes)), cubeMapRes);
}

const vec3[6] e = vec3[6](vec3(-1, 0, 0), vec3(1, 0, 0), vec3(0, -1, 0), vec3(0, 1, 0),
                          vec3(0, 0, -1), vec3(0, 0, 1));
 
vec3
indexToDir (float i)
{
  return e[int(i)]; 
}

float
dirToIndex (vec3 p)
{
  for(int i = 0; i<6; i++)
    if(p == e[i])
      return float(i);

  return 0.;
}

float
rndDirIndex (vec4 ut)
{
  return mod(floor(96.*hash41(ut)), 6.);
}

float
rndDirIndex (vec4 ut, float maxM)
{
  return mod(floor(72.*hash41(ut)), maxM);
}

vec3
rndDir (vec4 u)
{
  return indexToDir(rndDirIndex(u));
}

vec3
rndDir (vec4 u, float maxM)
{
  return indexToDir(rndDirIndex(u, maxM));
}

float
sBoxS (vec2 p, vec2 b, float sf)
{
  p = abs(p) - b + sf;

  return length(max(p, 0.)) + min(max(p.x, p.y), 0.) - sf;
}

const float PI = 3.14159265;

float
D_GGX (float NoH, float roughness)
{
  float alpha = pow(roughness, 4.);
  float b = (NoH*NoH*(alpha - 1.) + 1.);

  return alpha/(PI*b*b);
}

float
G1_GGX_Schlick (float NoV, float roughness)
{
  float r = .5 + .5*roughness;
  float k = (r*r)/2.;
  float denom = NoV*(1. - k) + k;

  return max(NoV, .001)/denom;
}

float
G_Smith (float NoV, float NoL, float roughness)
{
  float g1_l = G1_GGX_Schlick(NoL, roughness);
  float g1_v = G1_GGX_Schlick(NoV, roughness);

  return g1_l*g1_v;
}

vec3
BRDF (vec3 col, vec3 n, vec3 l, vec3 v, float type, float rough, float fresRef)
{
  vec3 h = normalize(v + l);

  float nv = clamp(dot(n, v), 0., 1.);
  float nl = clamp(dot(n, l), 0., 1.);
  float nh = clamp(dot(n, h), 0., 1.);
  float vh = clamp(dot(v, h), 0., 1.);  

  vec3 f0 = vec3(.16*(fresRef*fresRef)); 

  f0 = mix(f0, col, type);
  vec3 F = f0 + (1. - f0)*pow(1. - vh, 5.);

  float D = D_GGX(nh, rough); 

  float G = G_Smith(nv, nl, rough); 

  vec3 spec = F*D*G/(4.*max(nv, .001));

  vec3 diff = vec3(nl);
  diff *= 1. - F;
  diff *= (1. - type);

  return (col*diff + spec*PI);
}

vec4
Truchet (vec2 p, vec3 cellID, float faceID, inout vec2 scl)
{
  vec2 oP = p;

  vec2 oScl = scl;
  vec2 id2 = floor(p/scl) + .5;
  p -= (id2)*scl;

  vec3 id3 = cellID + vec3(id2/12., faceID/12.);

  int divN = 0;
  if(hash31(id3 + .09)<.5)
  {
    scl /= 2.; 
    p = oP;
    id2 = floor(p/scl) + .5;

    p -= (id2)*scl;

    id3 = cellID + vec3(id2/12., faceID/12.);

    divN++;
  }

  if(hash31(id3 + .42)<.5)
    p = p.yx*vec2(-1, 1);

  vec2 dd = vec2(1e5);

  if(hash31(id3 + .12)<.65)
  {
    if(hash31(id3 + .19)<.65)
    {
      dd.x = length(p - scl/2.);
      dd.x = abs(dd.x - scl.x/2.);
    }
    else
    {
      dd.x = length(p - vec2(1, 0)*scl/2.);
      dd.x = min(dd.x, length(p - vec2(0, 1)*scl/2.));
    }

    if(hash31(id3 + .21)<.65)
    {
      dd.y = length(p + scl/2.);
      dd.y = abs(dd.y - scl.y/2.);
    }
    else
    {
      dd.y = length(p - vec2(-1, 0)*scl/2.);
      dd.y = min(dd.y, length(p - vec2(0, -1)*scl/2.));
    }  
  }
  else
  {
    if(hash31(id3 + .13)<.65)
      dd.x = abs(p.y);
    else
    {
      dd.x = length(p - vec2(-1, 0)*scl/2.);
      dd.x = min(dd.x, length(p - vec2(1, 0)*scl/2.));
    }

    if(hash31(id3 + .14)<.65)
      dd.y = abs(p.x);
    else
    {
      dd.y = length(p - vec2(0, -1)*scl/2.);
      dd.y = min(dd.y, length(p - vec2(0, 1)*scl/2.));
    }
  }

  float lNum = 12./oScl.x;
  float offs = divN==0? .5 : .5;
  vec2 pat = (abs(fract(dd*lNum + offs) - .5) - .2)/lNum;

  if(divN == 0)
    dd = abs(dd - .25*scl.x);

  dd.xy -= .24/2.*oScl;

  if(hash31(id3 + .15)<.5)
  {
    dd = dd.yx;
    pat = pat.yx;
  }

  return vec4(dd, pat);  
}
// glut_963_common.glsl

uniform float iTimeDelta;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
  if (gl_FragCoord.x > cubeMapRes || gl_FragCoord.y > cubeMapRes)
    discard;
  vec2 u = mod(gl_FragCoord.xy, cubeMapRes);
  vec4 col;

  vec3 p = convertCoord(u);

  if(textureSize(iChannel0, 0).x<2 || iFrame==0)
  {
    col.x = p.x + p.y*wrap + p.z*wrap*wrap;
    col.y = 0.;
    col.z = 1.;
    col.w = step(.4, hash31(p + .11));
  }
  else
  {
    col = texelFetch(iChannel0, ivec2(u), 0);  

    float maxDirections = (p.z<0. || p.z>=wrap)? 4. : 6.;

    vec3 dir = rndDir(vec4(p, iFrame), maxDirections); 

    vec3 nP = p + dir;
    if(nP.z<0. || nP.z>=wrap)
      maxDirections = 4.;

    vec3 dirNgbr = rndDir(vec4(mod(nP, wrap), iFrame), maxDirections); 

    vec2 uvN = convertCoord(mod(nP, wrap));
    vec4 colNgbr = texelFetch(iChannel0, ivec2(uvN), 0);

    if (col.z < 1.)
    {
      vec3 ip = mod(vec3(col.x, floor(col.x/wrap), floor(col.x/(wrap*wrap))), wrap);

      float rnd = hash31(ip + .2);
      float dS =  (1. - rnd*.66)*3.*iTimeDelta;
      col.z = min(col.z + dS, 1.);
    }
    else if (colNgbr.z == 1. && dir == -dirNgbr && col.w != colNgbr.w)
    {
      if (col.w == 0.)
        col.x = colNgbr.x;
      col.y = dirToIndex(dir);
      col.z = 0.;
      col.w = colNgbr.w;
    }
  }

  gl_FragColor = col;
}
