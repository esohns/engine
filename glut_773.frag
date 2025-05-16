#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

#define FAR 1400.
#define FOG_COLOUR vec3(.33,.31,.3)
#define SKY_TOP vec3(.1, .13, 0.13)
#define FOAM_COLOUR vec3(.51,.55,.6)
#define PI 3.141592653
#define ZERO min(iFrame, 0)

vec3 camPos;
float time;
vec3 surf;
vec3 skyColour;
const vec3 sunDir = normalize(vec3(4,8,18));
//#define EXPORT_VERSON

#define UI0 1597334673U
#define UI1 3812015801U
#define UI2 uvec2(UI0, UI1)
#define UI3 uvec3(UI0, UI1, 2798796415U)
#define UIF (1.0 / float(0xffffffffU))

float
hash12i (vec2 p)
{
  uvec2 q = uvec2(ivec2(p)) * UI2;
  uint n = (q.x ^ q.y) * UI0;
  return float(n) * UIF;
}

vec2
hash22i (vec2 p)
{
  uvec2 q = uvec2(ivec2(p))*UI2;
  q = (q.x ^ q.y) * UI2;
  return vec2(q) * UIF;
}

float
hash13i (vec3 p)
{
  uvec3 q = uvec3(ivec3(p)) * UI3;
  uint n = (q.x ^ q.y ^ q.z) * UI0;
  return float(n) * UIF;
}

vec3
hash33i (vec3 p)
{
  uvec3 q = uvec3(ivec3(p)) * UI3;
  q = (q.x ^ q.y ^ q.z)*UI3;
  return vec3(q) * UIF;
}

float
noise (vec2 p)
{
  vec2 f = fract(p);
  p = floor(p);
  f = f * f * (3.0 - 2.0 * f);
  float res = mix(mix(hash12i(p),
                      hash12i(p + vec2(1.0, 0.0)), f.x),
                  mix(hash12i(p + vec2(0.0, 1.0)),
                      hash12i(p + vec2(1.0, 1.0)), f.x), f.y);
  return res;
}

float
noise (vec3 p)
{
  const vec2 add = vec2(1.0, 0.0);

  vec3 f = fract(p); 
  f *= f * (3.0-2.0*f);
  p = floor(p);

  float h = mix(mix(mix(hash13i(p), hash13i (p + add.xyy),f.x),
                    mix(hash13i(p + add.yxy), hash13i(p + add.xxy),f.x), f.y),
                mix(mix(hash13i(p + add.yyx), hash13i(p + add.xyx),f.x),
                    mix(hash13i(p + add.yxx), hash13i(p + add.xxx),f.x), f.y),
                f.z);
  return h*h*h*2.;
}

const float COSR = cos(.43);
const float SINR = sin(.52);
const mat2 rot2D = mat2(COSR, SINR, -SINR, COSR) * 1.4;

float
fbm (vec2 p)
{
  float a = 1.;
  float y = 0.0;
  for (int i = ZERO; i < 5; i++)
  {
    y += noise(p) * a;
    a *= .53;
    p *= rot2D;
    p.y += 9.1;
  }
  return y;
}

vec3
sky (vec3 p, vec3 dir)
{
  dir.y+=.02;

  dir.y = max(dir.y,0.001);
  float d = 20.0/dir.y;
  p = p + dir * d;
  vec3 s = vec3(fbm(p.xz*.06))*.2;
  s*=s;
  s = mix(s, FOG_COLOUR, max(1.-dir.y*3.,0.0));
  return mix(s, SKY_TOP ,abs(dir.y)*2.);
}

float
oceanFundamental (vec2 p, float tim, float iter)
{
  float a =noise(p*.01)*8.+3.0;
  float h = 0.0;
  float it = 1./iter;
  float spr = 0.0;

  p *= .035;

  for (float i = min(iTime,0.0); i <= 1.0; i += it)
  {
    float t = (1.08-i) * tim;
    float r =noise(p*2.33+t) * i;
    vec2 y1 = (cos(p-t)+1.0);
    vec2 y2 = (1.0-abs(sin(p-t)));

    y1 = mix(y1, y2, r);

    float s = y1.x + y1.y;

    h += s*a;

    a *= .54;
    p = p * rot2D;

    p += 37.7;
  }

  return h;
}

float
fbm (vec3 p)
{
  float a = 1.;
  float y = 0.0;
  for (int i = ZERO; i < 5; i++)
  {
    y += noise(p) * a;

    a *= .53;
    p.xz *= rot2D;
    p.y += 9.1;
  }
  return y;
}

float
multiFbm (vec3 p)
{
  float f = 0.0;
  const vec3 fre = vec3(0.87,1.0,0.5);
  const vec3 off = vec3(0.0 ,1.0,0.4);
  for( int i=0; i<3; i++ )
    f = fbm(p*fre[i] + f*off[i]);

  return f;
}

float
map (vec3 p, float iter)
{
  float h = oceanFundamental(p.xz, time, iter);
  return p.y-h;
}
 
vec2
rayMarchSea (vec3 p, vec3 dir)
{
  float d = 0.0;
  float spr = 0.0;
  for ( int i = ZERO; i < 100 && d < FAR; i++)
  {
    vec3 pos = p + dir*d;

    float hh = oceanFundamental(pos.xz, time, 8.);
    float h = pos.y-hh;
        
    if (h < .2)
      break;
    float wind = (noise(pos*.03)) * fbm(pos*2.+vec3(-time*12., time*.11,0));
    spr += max(25.-h, 0.0) * smoothstep(40.0, .0,max(h, 0.0))*smoothstep(FAR, 0.0,d)*smoothstep(30., 80.0, hh) * wind;
    d+= h*.7;
  }
  return vec2(d, min(spr*.03, 1.0));
}

#define FARCAB 100.0

float
roundedBox (vec3 p, vec3 b, float r)
{
  return length(max(abs(p)-b,0.0))-r;
}

#define rotate(p, a) p=cos(a)*p+sin(a)*vec2(p.y,-p.x)

vec2
nutCut (vec2 p, float s)
{
  float k = s / PI / 2.;
  return rotate(p, floor((atan(p.y, p.x)) * k + .5) / k);
}

float
nut (vec3 p, vec3 size, float r)
{
  rotate(p.xy,r);
  p.xy = nutCut(p.xy, 6.0);

  float t = roundedBox(p, size,.08);
  t = min( t, length(p+vec3(0,0,.2))- size.x );

  return t;
}

float length6 (vec2 p) { p=p*p*p; p=p*p; return pow(p.x+p.y,1.0/6.); }
float
sdTorusFlat (vec3 p, vec2 t)
{
  float x = length(p.xy)-t.x;
  return length6(vec2(x,p.z))-t.y;
}

vec2
maxV2 (vec2 v1, vec2 v2)
{
  return (v2.x > v1.x) ? v2:v1;
}

vec2
minV2 (vec2 v1, vec2 v2)
{
  return (v2.x < v1.x) ? v2:v1;
}

vec2
mapCab (vec3 p)
{
  vec2 d = vec2(sdTorusFlat(p-vec3(0,0,.4), vec2(9.1,1.1)), 0);

  d = minV2(d, vec2(.0-p.z, 2.0));

  p.xy = abs(p.xy);
  d = minV2(d, vec2(nut(p - vec3(6.2, 6.5, -.8),vec3(.15, .15, .12), .5), 1.0));
  d = minV2(d, vec2(nut(p - vec3(8.9,.0,-.8),   vec3(.15, .15, .12), 0.0), 1.));

  return d;
}

vec3
normalCab (vec3 pos, float ds)
{
  ds *= .5/iResolution.y;
  vec3 n = vec3(0);
  for( int i=ZERO; i<4; i++ )
  {
    vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
    n += e*mapCab(pos+ds*e).x;
  }
  return normalize(n);
}

float
shadowCab (vec3 ro, vec3 rd)
{
  float res = 1.0;
  float t = .01;
  for( int i = ZERO; i < 10; i++ )
  {
    float h = mapCab(ro + rd*t).x;

    res = min( res, 4.*h/t );
    t += h;
    if (res < .1)
      break;
  }
  return clamp( res, 0.1, 1.0 );
}

vec3
lightCab (vec3 pos, vec3 nor,  vec3 dir, vec4 mat, float wat)
{
  vec3 lamp = vec3((noise(vec2(time*.5))-.5)*30.,wat*.2,-15);

  vec3 ldir = normalize(lamp-pos);
  float a = smoothstep(-10.0, 4.0, wat)*.3+.05;
  float sha = shadowCab(pos, ldir);
  vec3 col = mat.xyz * max(dot(nor, ldir), 0.0)*4.*a*sha;
  vec3 ref = reflect(dir, nor);
  col += vec3(1.,.8, .7) * pow(max(dot(ref, ldir), 0.0), 2.0) * mat.w * sha;
  return min(col, 1.0);
} 

vec2
marchCabin (vec3 ro, vec3 rd)
{
  vec2 d = vec2(0);
  for (int i = ZERO; i < 30 && d.x < FARCAB; i++)
  {
    vec3 p = ro+rd*d.x;
    vec2 m = mapCab(p);
    d.y = m.y;
    if ((m.x) < .05 ||  d.x > FARCAB)
      break;
    d += m.x*.8;
  }
  return d;
}

mat3
viewMat (float ay, float az)
{
  vec2 o, ca, sa;
  o = vec2 (ay, az);
  ca = cos (o);
  sa = sin (o);
  return mat3 (ca.y, 0., - sa.y, 0., 1., 0., sa.y, 0., ca.y) *
         mat3 (1., 0., 0., 0., ca.x, - sa.x, 0., sa.x, ca.x);
}

vec3
normal (vec3 pos, float ds)
{
  ds *= 1./iResolution.y;
  ds = max(ds*ds, .1);

  vec3 n = vec3(0);
  for( int i=ZERO; i<4; i++ )
  {
    vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
    n += e*map(pos+ds*e, 14.0);
  }
  return normalize(n);
}

#define F length(.5-fract(k.xyw*=mat3(-2,-1,2, 3,-2,1, 1,2,2)*
float
waterPattern (vec2 p)
{
  vec4 k;
  k.xy = p*(sin(k).w+2.)*.009;
  return pow(min(min(F.5)),F.4))),F.3))), 7.)*20.;
}

float
waveDepth (vec3 p, vec3 dir)
{
  float d = 0.0;
  for( float i = min(time,3.0); i <= 23.0; i+=5.)
  {
    float h = map(p + dir*i, 8.);
    if (h > 0.)
      break;
    d += -h;
  }
  return clamp(1.0-d*.02, 0.0, 1.0);
}

vec3
lighting (vec3 pos, vec3 nor, vec3 dir, vec3 mat)
{
  vec3 col;
  col = mat * max(dot(sunDir, nor), 0.0);
  vec3 ref = reflect(dir, nor);
  float fres = clamp(pow( 1.+dot(nor, dir), 7. ), 0.0, 1.0)*.6;

  col = mix(col, sky(pos, nor), .3);
  col = mix(col, sky(pos, ref), fres);
  return col;
}

vec3
diffuse (vec3 pos, vec3 nor, float dep)
{
  pos.x -= time*1.3;
  vec3 mat = vec3(.1,.1,.12);
  float h = smoothstep(0., 1.0,nor.y);

  mat += h*.1;

  mat = mix(mat, vec3(.3,.7,.7), dep);
  pos.x += time * 4.;
  pos*=3.5;

  float ny = clamp(nor.y, 0.4, 1.0);
  float foam = waterPattern(pos.xz +pos.y*.3) * 3.;
  foam += waterPattern(pos.xz*.37)*5.;
  foam = clamp(foam, 0.0, 1.0);
  mat = mat+foam * (dep*dep*1.5+ny*.01);
  return mat;
}

float
distanceRayPoint (vec3 ro, vec3 rd, vec3 p)
{
  p -= ro;
  float h = dot(p,rd);
  return length(p-rd*h);
}

#define tri(x) abs(fract(x)-.5)*2.
vec3
getOffset (vec3 id)
{
  return tri(id+time*.5);
}

vec3
getColour (vec2 id)
{
  float t1 = id.x*.4;
  float t2 = id.y*.4;
  return vec3(.75,.6+t1, .6+t2);
}

#define BUBBLES 65
#define BUBBLE_SIZE 50.0
vec3
bubbles (vec3 ro, vec3 rd)
{ 
  float d;

  ro *= .00002;
  ro.x+=time*5.;
  float fade = 1.0;

  vec3 pos = floor(ro),
       ri = 1./rd,
       rs = sign(rd),
       dis = (pos-ro + 0.5 +rs*.5) * ri,
       sum = vec3(0), col = sum;
  for( int i = ZERO; i < BUBBLES; i++ )
  {
    vec3 id = hash33i(pos*19.31);
    float si = (id.x+id.z)*.25+.08;
    vec3 offset = getOffset(id);
    d = distanceRayPoint(ro, rd, pos+offset);
    vec3 mat =  getColour(id.xy);
    col = mat * pow(smoothstep(si, 0.0,d),8.)*10. *fade;
    sum += col;
    fade*= .98;
    vec3 mm = step(dis.xyz, dis.yzx) * step(dis.xyz, dis.zxy);
    dis += mm * rs * ri;
    pos += mm * rs;
  }

  return sum;
}

float
fader (float edge0, float edge1, float x)
{
  float t = (x - edge0) / (edge1 - edge0);
  return  clamp(exp((t-.9825)*3.)-.0525, 0.0, 1.0);
}

void
surfFoam (vec3 p, float water)
{
  float w = -(water-4.0)*pow(multiFbm(p*2.+vec3(0, time*8., time*4.)), 2.0)*.5;
  surf += FOAM_COLOUR*fbm(camPos*8.3+vec3(0,0,time*6.))* .35 * w;
  surf *= FOAM_COLOUR*smoothstep(-10.,1.0, water);
}

float
underWaterAtTime (vec3 p,  float ti)
{
  float h = oceanFundamental(p.xz, ti, 8.0)+multiFbm(camPos*5.3+ti*7.);

  return p.y-h-4.;
}

vec3
cameraLocation (float ti)
{
  vec3 p = vec3(ti*10.,100,0);
  float h = (sin(ti*.65)+1.0)*40. + (sin(ti*1.37))*10. + 20.;
  p.y = h;

  return p;
}

vec3
bumpmap (vec3 p, vec3 dir, float a)
{
  p *= 5.;
  float v0 = multiFbm(p);
  float v1 = multiFbm(p+vec3(.1,0.,0));
  float v2 = multiFbm(p+vec3(0,0.,.1));
  vec3 grad = normalize(vec3(v1-v0, 0.0, v2-v0));
  grad -= dir*dot(dir, grad);          

  return normalize( dir + grad*a );
}
 
vec4
getMaterial (int id, vec3 p, inout vec3 nor)
{
  vec4 col = vec4(0);
  vec2 bn = vec2(0.0);
  float d = 0.0;
  switch (id)
  {
    case 0:
      col = vec4(.1, .07, .04, .5);
      col = col*(.6+.4*multiFbm(p*5.+30.));
      col.w = col.w*col.w*multiFbm(p*3.);
      bn = vec2(3.0,0.08);
      break;
    case 1:
      col = vec4(.05, .05, .06, .3);
      col.w = col.w*(.4+.6*multiFbm(p*21.));
      break;
    case 2:
      col = texture(iChannel0, p.xy*.15) * vec4(.6,1.2,15.,1.);
      vec2 uv = fract(-p.xy*.15);
      col *= smoothstep(0.0, .05, uv.y)*smoothstep(1., .95, uv.y)+.1;
      col *= vec4(.1,.05,.01,.1);
      col.w = pow(col.x*10., 5.);
      break;
  }
  nor = bumpmap(p * bn.x, nor, bn.y);
  return col;
}

void
main ()
{
  vec4 outCol;
  vec2 coord = gl_FragCoord.xy;

  vec2 uv = (coord-iResolution.xy*.5)/iResolution.y;

  time = iTime+232.;
  uv*= .8+sin(clamp(iTime*.5-0.3, 0.,3.5))*.3;

  vec3 colour = vec3(0);
  surf = vec3(0.0);

  vec2 shake = vec2(noise(vec2(time*.6))-.5, noise(vec2(time*.7+233.))-.5) * vec2(.1, .1);
  uv += shake;

  camPos = cameraLocation(time);
  float porthole = length(uv)*2.;
  if (porthole < 1.0)
  {
    camPos.xy += uv*10.;

    vec3 dir  = vec3(0,0, 1.);
    dir = viewMat (uv.y +.0, uv.x+.5) * dir;

    rotate(dir.zy, (sin(time*.5)*sin(time*.37)*.6));

    float water = (underWaterAtTime(camPos, time));
    if(water < 4.0)
    {
      colour  = mix(FOAM_COLOUR, vec3(.0, .04,.03),clamp(pow(-(water-4.0), .4)*.3, 0.0, 1.0));
      colour  = mix(colour, vec3(.0, .0,.02),clamp((-3.0-water)*.015, 0.0, 1.0));
      colour  = mix(colour, FOAM_COLOUR,clamp(2.-(water*water*.4)*multiFbm(camPos*3.3), 0.0, 1.0));

      surfFoam(camPos, water);

      if (water < .0)
        surf += bubbles(camPos*25000., dir)*.3*clamp(4.0+water*.1,0.1, 3.0)*.03;
    }
    if (water >= 0.)
    {
      vec3 p = cameraLocation(time-1.);

      float past = underWaterAtTime(p, time-1.);

      float f = smoothstep(25.0, -8.0, past)*.15;

      float w = smoothstep(.6, -.3, uv.y);
      f = f*f*3. * w;
      float d =0.0;
      if (f > 0.0)
        dir = bumpmap(vec3(uv.x,time*.5,uv.y*.5), dir, f);

      vec2 dis = rayMarchSea(camPos, dir);
      if (dis.x < FAR)
      {
        vec3  pos = camPos + dir * dis.x;
        vec3  nor = normal(pos, dis.x);

        float dep = waveDepth(pos, dir);
        vec3  mat = diffuse(pos, nor, dep);

        colour = lighting(pos, nor, dir, mat);
        colour = mix(FOG_COLOUR, colour, exp(-dis.x*.001));
      }
      else
        colour = sky(camPos, dir);

      colour = mix(colour, FOAM_COLOUR, dis.y);
    }
  }
  else 
  {
    float water = (underWaterAtTime(camPos, time));

    vec3 dir  = vec3(0,0, 1.);
    dir = viewMat (uv.y, uv.x) * dir;
    camPos = vec3(0,0,-15);
    vec2 dis = marchCabin(camPos, dir);
    if (dis.x < FARCAB)
    {
      vec3 pos = camPos + dir * dis.x;
      vec3 nor = normalCab(pos, dis.x);
      int matID = int(dis.y);
      vec4 mat = getMaterial(matID, pos, nor);
      colour = lightCab(pos, nor, dir, mat, water);
    }
  }
  colour += surf;

  if (porthole < 1.0)
    colour = mix(colour, colour*.1, smoothstep(0.9, 1.,porthole));

  colour = clamp(colour*.2 + smoothstep(0.0, 1.0, colour)*.8,0.0, 1.0);

  colour = sqrt(colour);
  colour += hash12i(coord+time)/256.0; //...Dither those 8bit limitations away

#ifdef EXPORT_VERSON
  outCol = vec4(fader(0.0, 4.0, iTime) * fader(299.0, 294.0, iTime)*colour, 1);
#else
  outCol = vec4(fader(0.0, 4.0, iTime) * colour, 1);
#endif
  outCol += hash12i(coord+time)/256.0;

  gl_FragColor = outCol;
}
