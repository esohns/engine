#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

#define cloudy 0.3
#define cld1 0.4
#define cld2 0.6
#define rainvol 0.0
#define rainmulti 8.0
#define RAINBOW_BRIGHTNESS 0.
#define scatonly 0.
#define snowvol 0.
#define fireworkson 0.

#define xaxiscloud -100.0*t
#define yaxiscloud t*100.0
#define zaxiscloud 250.0*t

#define auroraINT 1.5
#define lensflare 1.

#define Dfact 0.5

#define scatter_steps 8
#define scatter_steps2 4

#define TAU 6.283185307179586
#define ANIMATE_CLOUDS 0

#define ENABLE_RAIN 1
#define ENABLE_DENSE_RAIN 1
#define ENABLE_DENSE_RAIN2 0
#define ORIG_CLOUD 0
#define SIMPLE_SUN 1
#define ENABLE_MOON 1
#define ENABLE_MOON_2 0
#define ENABLE_SEA 1
#define ENABLE_LENS_FLARE 1
#define ENABLE_BACK_TEXTURE 1
#define EXPENSIVE_CALC 1
#define ENABLE_SCAT_OPTIONS 1
#define SOFT_SUN 1
#define NICE_HACK_SUN 1
#define FIREWORK_COUNT 2
#define FIREWORK_DURATION 8.
#define FIREWORK_LOW .75
#define FIREWORK_HIGH 1.05
#define ROCKET_PARTICES 16
#define ROCKET_DURATION 1.5
#define FLASH_DURATION ROCKET_DURATION + .2
#define THRUSTER_SPEED .25
#define EXPLOSION_STRENGTH .03;
#define EXPLOSION_PARTICLES 32

#define cameraheight 5e1
#define t iTime

#define fov tan(radians(60.0))

#define moonspec 10.
#define moonillumination 1.0
#define moon_rotate  0.
#define moonlod 2.
#define MoonINT 2.

#define Rm 120.0

#define rr 0.0000059
#define rg 0.0000121
#define rb 0.0000182

#define haze 0.01
#define g0 0.76

#define RI0 20.

#define mincloudheight 5000.
#define maxcloudheight 8000.
#define Hr 7.994e3
#define Hm 1.2e3
#define R0 6361e3
#define Ra 6369e3
#define rorder 0.
#define SI 10.
#define MI0 8.

#define bM vec3(0.000051)
#define cloudnoise 0.0002
#define phaseR1 0.0596831
#define phaseM1 0.1193662

#define aurora_mt 1.

#define aurora_noise 0.06
#define aurora_steps 50
#define aurora_height 0.
#define aurorardy -0.05
#define aurora2 0.
#define aurorardz 0.
#define aurora_f 0.065
#define aurora_sy 1.

#define aurora_col 0.043
#define wavenoise 0.003
#define wave_f 10.
#define wave_t1 4.
#define wave_t2 -1.
#define aur_style 0.
#define cloudtype 0.
#define add_noise 0.

#define eclispethreshold 0.05
#define camera_height 10.
#define camera_H_rotate radians(0.)
#define camera_V_rotate radians(0.)
#define camera_T_rotate radians(0.)
#define camera_depth 0.
#define aurora_ss 5.
#define aurora_ss2 0.002
#define aurora_of 0.006
#define res 1.

#define h1 100.
#define h2 1000.

#define aurora_r 2.15
#define aurora_g -1.
#define aurora_b 1.

#define MIMin1  8.0
#define SEA_HEIGHT 0.6
#define SEA_CHOPPY 4.0
#define SEA_SPEED 0.8
#define SEA_FREQ 0.16
#define SEA_BASE vec3(0.1,0.19,0.22)

#define camz 0.001
#define tonemap 9.
#define rb1 1.25

#define HmMin1 1200.
#define SEA_NUM_STEPS 7.
#define aa1 -1.
#define aa2 0.
#define sat 1.1
#define vib 0.
#define hue 0.
#define spec 40.
#define winddirx 0.
#define rain1 4.
#define rain2 0.05
#define rain3 0.0
#define rain4 0.05
#define rain5 0.1
#define rain6 10000.
#define rain7 1.
#define rain8 0.01
#define rain9 1000.
#define rain10 1000.
#define rain11 0.4
#define rain12 0.6
#define rain13 0.25
#define raleighgam 2.2
#define miegam 1.6
#define raleighgammin 1.0
#define miegamin 1.6
#define specmin 20.
#define sealight 1.
#define specbrightness 0.5
#define mietype 0.

#define lensflareb 1.
#define lensgam 1.4
#define lensflaretype 2.

#define spectype 1.
#define diffusepow 80.
#define RRMin 0.0000025
#define RRMax 0.0000058

#define round(x, f) (floor((x)/(f) + 0.5) * (f))
#define C1(x) clamp(x, 0., 1.)
#define S1(a, b, x) smoothstep(a, b, x)
#define F(x, f) (floor(x * f) / f)
#define saturate(x)	clamp(x, 0.0, 1.0)

#define Argb vec3(aurora_r,aurora_g,aurora_b)
#define C  vec3(0., -R0, 0.)

const int steps = 8;
const int stepss = 4;

const float I = 10.;

const float ng = 0.05;
const float ng2 = ng * ng;

const float s = 0.999;
#if SOFT_SUN
const float s2 = s;
#else
const float s2 = s * s;
#endif

const float moon_s = 0.999;
#if SOFT_SUN
const float moon_s2 = moon_s;
#else
const float moon_s2 = moon_s * moon_s;
#endif

vec3 bM2 = vec3(5e-6);

vec3 bR = vec3(5.8e-6, 13.5e-6, 33.1e-6);

vec3 Ds = normalize(vec3(0., .09, -1.));
vec3 Dsm = normalize(vec3(0., .09, -1.));

const float PI = 3.14159265358979323846;
const float PI_2 = 1.57079632679489661923;
const float PI_4 = 0.785398163397448309616;

float cloudyhigh = 0.05;
#if ORIG_CLOUD
float cloudnear = 1.0; //9e3 12e3
float cloudfar = 1e3; //15e3 17e3
#else
float cloudnear = 1.0;
float cloudfar = 160e3;
#endif

#define S(x, y, z) smoothstep(x, y, z)

mat3
rotateX (float theta)
{
  float c = cos(theta);
  float s = sin(theta);
  return mat3(
    vec3(1, 0, 0),
    vec3(0, c, -s),
    vec3(0, s, c)
  );
}

mat3
rotateY (float theta)
{
  float c = cos(theta);
  float s = sin(theta);
  return mat3(
    vec3(c, 0, s),
    vec3(0, 1, 0),
    vec3(-s, 0, c)
  );
}

mat3
rotateZ (float theta)
{
  float c = cos(theta);
  float s = sin(theta);
  return mat3(
    vec3(c, -s, 0),
    vec3(s, c, 0),
    vec3(0, 0, 1)
  );
}

mat2 mm2 (float a) { float c = cos(a), s = sin(a);return mat2(c,s,-s,c); }
mat2 m2 = mat2(0.95534, 0.29552, -0.29552, 0.95534);
float tri (float x) { return clamp(abs(fract(x)-.5),0.01,0.49); }
vec2 tri2 (vec2 p) { return vec2(tri(p.x)+tri(p.y),tri(p.y+tri(p.x))); }

float
triNoise2d (vec2 p, float spd)
{
  float z=1.8;
  float z2=2.5;
  float rz = 0.;
  p *= mm2(p.x*0.06);
  vec2 bp = p;
  for (float i=0.; i<5.; i++)
  {
    vec2 dg = tri2(bp*1.85)*.75;
    dg *= mm2(t*spd);
    p -= dg/z2;

    bp *= 1.3;
    z2 *= .45;
    z *= .42;
    p *= 1.21 + (rz-1.0)*.02;

    rz += tri(p.x+tri(p.y))*z;
    p*= -m2;
  }

  return clamp(1./pow(rz*29., 1.3),0.,.55);
}

vec3
bg (vec3 rd)
{
  float sd = dot(normalize(vec3(-0.5, -0.6, 0.9)), rd)*0.5+0.5;
  sd = pow(sd, 5.);
  vec3 col = mix(vec3(0.05,0.01,0.15), vec3(0.01,0.05,0.15), sd);
  return col*.63;
}

float hash21 (vec2 n) { return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453); }

void
aurora (vec3 rayorigin, vec3 raydirection, out vec3 bpos, out vec4 col)
{
  mat3 rotY = rotateX(-0.25);
  vec4 avgCol = vec4(0);
  rayorigin.z += t*aurora_height;
  raydirection.y +=0.1;
  raydirection.y *= aurora_sy;
  raydirection.z += -0.3;
  raydirection *= rotY;

  float mt = aurora_mt;
  float ms = 50.;
  for(int i=0;i<aurora_steps;i++)
  {
    float of = aurora_of * hash21(gl_FragCoord.xy) * smoothstep(0.,15., float(i) * mt);
    float pt = ((.8 + pow(float(i),1.4) * aurora_ss2) - rayorigin.y) / (raydirection.y * 2. + 0.4);
    pt -= of;
    bpos = rayorigin + pt * raydirection ;
    vec2 p = bpos.zx +sin(t*0.01);
    float rzt = triNoise2d(p, aurora_noise);
    vec4 col2 = vec4(0,0,0, rzt);
    col2.rgb = (sin(1. -vec3(aurora_r, aurora_g, aurora_b) + (float(i) * mt) * aurora_col) * 0.5 + 0.5) * rzt;
    avgCol =  mix(avgCol, col2, .5);
    col += avgCol * exp2( (-float(i)*mt) * aurora_f - 2.5) * smoothstep(0., aurora_ss, float(i)*mt);
  }
  col *= clamp(raydirection.y * 15. +0.4, 0., 1.);
  col *= auroraINT;
}

const float density = 0.5;
const vec3 skyColor = vec3(0.37, 0.55, 1.0) * (1.0 + 0.0);

float
getSunPoint (vec2 p, vec2 lp)
{
  return smoothstep(0.036*(fov/2.0), 0.001*(fov/2.0), distance(p, lp)) * 100.0;
}

float
getMie (vec2 p, vec2 lp)
{
  float mytest = lp.y < 0.5 ? (lp.y+0.5)*pow(0.05,20.0):0.05;
  float disk = clamp(1.0 - pow(distance(p, lp), mytest), 0.0, 1.0);
  return disk*disk*(3.0 - 2.0 * disk) * 0.25 * PI;
}

vec3
getSkyAbsorption (vec3 x, float y)
{
  vec3 absorption = x * y;
  absorption = pow(absorption, 1.0 - (y + absorption) * 0.5) / x / y;
  return absorption;
}

vec3
jodieReinhardTonemap (vec3 c)
{
  float l = dot(c, vec3(0.2126, 0.7152, 0.0722));
  vec3 tc = c / (c + 1.0);
  return mix(c / (l + 1.0), tc, tc);
}

vec3
getAtmosphericScattering (vec2 p, vec2 lp, float zenithOffset)
{
  float zenithnew = density / pow(max(p.y - zenithOffset, 0.0035), 0.75);
  float sunPointDistMult =  clamp(length(max(lp.y + 0.1 - zenithOffset, 0.0)), 0.0, 1.0);
  vec3 absorption = getSkyAbsorption(skyColor, zenithnew);
  vec3 sunAbsorption = getSkyAbsorption(skyColor, density / pow(max((lp.y + 0.2) - zenithOffset, 0.0035), 0.15));
  vec3 sun3 = getSunPoint(p, lp) * absorption;
  vec3 totalSky = sun3;
  totalSky *= sunAbsorption * 0.5 + 0.5 * length(sunAbsorption);
  vec3 newSky = jodieReinhardTonemap(totalSky);
  return newSky;
}

vec3
hash33 (vec3 p)
{
  p = fract(p * vec3(443.8975,397.2973, 491.1871));
  p += dot(p.zxy, p.yxz+19.27);
  return fract(vec3(p.x * p.y, p.z*p.x, p.y*p.z));
}

vec2
hash21 (float p)
{
  vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.xx + p3.yz) * p3.zy);
}

vec3
hash31 (float p)
{
  vec3 p3 = fract(vec3(p,p,p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);
  return fract((p3.xxy+p3.yzz)*p3.zyx);
}

float
hash11 (float p)
{
  p = fract(p * .1031);
  p *= p + 33.33;
  p *= p + p;
  return fract(p);
}

float
hashf (vec3 p3)
{
  p3 = fract(p3 * 0.1031);
  p3 += dot(p3,p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

float
noisef (vec3 x)
{
  vec3 i = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.0-2.0*f);
  return mix(mix(mix(hashf(i+vec3(0, 0, 0)), 
                     hashf(i+vec3(1, 0, 0)),f.x),
                 mix(hashf(i+vec3(0, 1, 0)), 
                     hashf(i+vec3(1, 1, 0)),f.x),f.y),
             mix(mix(hashf(i+vec3(0, 0, 1)), 
                     hashf(i+vec3(1, 0, 1)),f.x),
                 mix(hashf(i+vec3(0, 1, 1)), 
                     hashf(i+vec3(1, 1, 1)),f.x),f.y),f.z);
}

float
remap (float x, float a, float b, float c, float d)
{
  return (((x - a) / (b - a)) * (d - c)) + c;
}

float
random (float p)
{
  return fract(52.043*sin(p*205.429));
}

float
random2 (float p)
{
  return random(p)*2.0-1.0;
}

vec3
meteor (vec2 uv, float gtime, float delay)
{
  float seed = round(gtime, delay);

  float startTime = (delay - 1.5) * random(seed);
  float time = max(0.0, min(1.0, gtime-seed - startTime));

  vec2 start = vec2(
    random2(seed),
    0.7 + 0.3 * random(seed+0.1)
  );

  vec2 end = start * 0.5;

  uv = uv - mix(start, end, time);

  end = normalize(end - start);
  uv = uv * mat2(end.x, end.y, -end.y, end.x);
  uv.x *= 0.1;

  float alpha = 16.0 * pow(time, 2.0) * pow(time - 1.0, 2.0);
  return vec3(max(0.0, alpha - iResolution.y * length(uv)));
}

vec3
meteorstorm (vec2 uv)
{
  return meteor(uv, iTime, 9.5837) +
         meteor(uv, iTime + 15.3, 15.459) +
         meteor(uv, iTime + 125.0, 31.2);
}

void
fireworks (vec2 st, out float MI2, out vec3 fire, out vec3 fireworkCol2)
{
  vec2 fireworkPos, particlePos;
  float radius, theta, radiusScale, spark, sparkDistFromOrigin, shimmer,
  shimmerThreshold, fade, fade2, timeHash, timeOffset, rocketPath;
  vec3 particleHash, fireworkHash, fireworkCol,finalCol;

  for (int j = 0; j < FIREWORK_COUNT; ++j)
  {
    timeHash = hash11(float(j + 1) * 9.6144 + 4098.6118);

    timeOffset = float(j + 1) + float(j + 1) * timeHash;

    fireworkHash = hash31(471.5277 * float(j) + 1226.9146 + float(int((iTime+timeOffset) / FIREWORK_DURATION))) * 2. - 1.;
    fireworkCol = fireworkHash * .5 + .5;
    fireworkHash.y = remap(fireworkHash.y, -1., 1., FIREWORK_LOW, FIREWORK_HIGH);

    fireworkHash.x = ((float(j) + .5 + fireworkHash.x * .50)/ float(FIREWORK_COUNT)) * 2. - 1.;

    float time = mod(iTime + timeOffset, FIREWORK_DURATION);
    if (time > ROCKET_DURATION)
    {
      fireworkPos = vec2(fireworkHash.x, fireworkHash.y);

      for (int i = 0; i < EXPLOSION_PARTICLES; ++i)
      {
        particleHash = hash31(float(j) * 1291.1978 + float(i) * 1619.8196 + 469.7119);
        theta = remap(particleHash.x, 0., 1., 0., TAU);
        radiusScale = particleHash.y * EXPLOSION_STRENGTH;

        radius = radiusScale * time * time;

        particlePos = vec2(radius * cos(theta), radius * sin(theta));
        particlePos.y -= 8. * pow(particlePos.x, 4.);
        spark = .0003 / pow(length(st - particlePos - fireworkPos), 1.7);
        sparkDistFromOrigin = 2. * length(fireworkPos - particlePos);

        shimmer = max(0., sqrt(sparkDistFromOrigin) * (sin((iTime + particleHash.y * TAU) * 18.)));
        shimmerThreshold = FIREWORK_DURATION * .6;

        fade = C1((FIREWORK_DURATION * 2.) * radiusScale - radius);
        fade2 += fade;			
        
        if(shimmer > 0. && fade>0.)
          fireworkCol2 = fireworkCol;
        finalCol += clamp(mix(spark, spark * shimmer, smoothstep(shimmerThreshold * radiusScale, (shimmerThreshold + 1.) * radiusScale , radius)) * fade * fireworkCol,0.0,1.);
      }

      MI2 = fade2;
      if(time < FLASH_DURATION)
        finalCol += clamp(spark / (.01 + mod(time, ROCKET_DURATION)),0.,1.);
    }
    else
    {
      rocketPath = mod(time, ROCKET_DURATION) / ROCKET_DURATION;
      rocketPath = sin(rocketPath / (ROCKET_DURATION * .75) * PI * .5);
      fireworkPos = vec2(fireworkHash.x, rocketPath * fireworkHash.y);

      fireworkPos.x += sin(st.y * 50. + time) * fireworkCol.z * .0035;

      for (int i = 0; i < ROCKET_PARTICES; ++i)
      {
        particleHash = hash31((float(i) * 603.6837) + 1472.3486);

        float t = time * (2. - time);
        radius = mod(time + particleHash.y, THRUSTER_SPEED) / THRUSTER_SPEED * particleHash.z * .1;

        theta = remap(particleHash.x, 0., 1., 0., PI * .1) + PI * 1.45;
        particlePos = vec2(radius * cos(theta), radius * sin(theta));
        finalCol += clamp(8e-5 / pow(length(st - particlePos - fireworkPos), 1.1) * mix(vec3(1.4, .7, .2), vec3(1.4), radius * 16.),0.0,10.);
      }
    }
  }

  fire =finalCol;
  MI2 = clamp(MI2,0.,1000.);
}

vec3
stars (vec3 p)
{
  vec3 c = vec3(0.,0.,0.0);
  float res0 = res / 2.0;
  float res1 = 0.;
  if (iResolution.x <iResolution.y)
    res1 = iResolution.y * res0;
  else
    res1 = iResolution.x * res0;

  for (int i=0;i<4;i++)
  {
    vec3 q = fract(p*(.15*res1))-0.5;
    vec3 id = floor(p*(.15*res1*(1./res0)));
    vec2 rn = hash33(id).xy;
    float c2 = 1.-smoothstep(0.,.6,length(q));
    c2 *= step(rn.x,.0005+float(i)*float(i)*0.001);
    c += c2*(mix(vec3(1.0,0.49,0.1),vec3(0.75,0.9,1.),rn.y)*0.1+0.9);
    p *= 1.3;
  }

  return c*c*.8;
}

float noise (vec2 v) { return textureLod(iChannel0, (v+.5)/256., 0.).r; }

float
noise (vec3 v)
{
  vec3 p = floor(v);
  vec3 f = fract(v);
  
  vec2 uv = (p.xy+vec2(37.,17.)*p.z) + f.xy;
  vec2 rg1 = textureLod( iChannel0, (uv+.5)/256., -100.).yx;
  return mix(rg1.x, rg1.y, f.z);
}

float
Noise2 (vec3 x)
{
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.0-2.0*f);

  vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
  vec2 rg1 = texture( iChannel0, (uv+ 0.5)/256.0, -100.0).yx;
  return mix( rg1.x, rg1.y, f.z );
}

float
fnoise (vec3 v)
{
#if ANIMATE_CLOUDS
  return
    .55 * noise(v) +
    .225 * noise(v*2. + (t +(t*0.0)) *.04) +
    .125 * noise(v*3.99) +
    .0625 * noise(v*8.9);
#else
  return
    .55 * noise(v) +
    .225 * noise(v*2.) +
    .125 * noise(v*3.99) +
    .0625 * noise(v*8.9);
#endif
}

float
fnoise2 (vec3 p, float t_)
{
  p *= .25;
  float f;

  f = 0.5000 * Noise2(p); p = p * 3.02; p.y -= t_*.02;
  f += 0.2500 * Noise2(p); p = p * 3.03; p.y += t_*.06;
  f += 0.1250 * Noise2(p); p = p * 3.01;
  f += 0.0625   * Noise2(p); p =  p * 3.03;
  f += 0.03125  * Noise2(p); p =  p * 3.02;
  f += 0.015625 * Noise2(p);

  return f;
}

float
cloud (vec3 p, float t_)
{
  float cld =0.;
  cld = fnoise2(p*cloudnoise,t_) + cloudy*0.1;
  cld = smoothstep(cld1+.04, cld2+.04, cld);
  cld *= cld * (5.0*rainmulti);

  return cld+haze;
}

void
densities (vec3 pos, float highclouds, out float rayleigh, out float mie, float Hm1, float height_off)
{
  vec3 d = pos;
  d *=fov;
  d.y = 0.0;
  float dist = length(d);

  float cld = 0.;
  float godray =0.;
  float test =0.;
  float test2 =0.;
  float test3 =0.;
  float AR = iResolution.x/iResolution.y;
  float h = length(pos - C) - R0;

  if(dist < 500000.)
  {
    if (mincloudheight < h && h < maxcloudheight)
    {
      cld = cloud(pos + vec3(xaxiscloud,yaxiscloud, zaxiscloud),t)*cloudy;
      cld *= sin(3.1415*(h-mincloudheight)/mincloudheight) * cloudy;
    }

#ifdef cloud2
    float cld2 = 0.;
    if (12e3 < h && h < 15.5e3)
    {
      cld2 = fnoise2(pos*3e-4,t)*cloud(pos*32.0+vec3(27612.3, 0.,-t*15e3), t);
      cld2 *= sin(3.1413*(h-12e3)/12e3) * cloudyhigh;
      cld2 = clamp(cld2,0.0,1.0);
    }
#endif

    float factor = clamp(1.0-((dist - cloudfar)/(cloudfar-cloudnear)),0.,10.);
    cld *= factor;

    if(highclouds >0.)
    {
      if(d.y > 1000. && d.z > -20000.)
      {
        vec2 uv00 = (gl_FragCoord.xy / iResolution.xy);
        vec2 uv3 = uv00 * (2.0) - (1.0);
        uv3.x *=AR;
        uv3.y +=Dfact;
        float cldhighvol = cloudy + 0.5;
        vec2 st0 =  (uv3 * vec2(.5 +(uv00.y +2.) *.75, 10.) + vec2(0. + uv00.y *10., t*0.05)) * 0.05;
        float f0 = texture(iChannel0, st0, -100.0).y * texture(iChannel0, st0*.123, -100.0).x * 0.25;
        float cloud1 = clamp(cldhighvol -.15, 0.0, 1.0);
        float cloudthing = clamp( pow( abs(f0), 15.) * 1.0 * (cloud1*cloud1*5.), 0.0, (uv00.y +.1) *.6);
        test3 = clamp(mix(cloudthing, cldhighvol,f0),0.,0.5);
      }
    }

    if(pos.y < rain6)
    {
      if(rainvol >0.)
      {
        vec2 uv0 = (gl_FragCoord.xy / iResolution.xy);
        vec2 uv = uv0 * (2.0) - (1.0);
        uv.x *=AR;
        uv.y +=Dfact;
        float rainvol0 = rainvol;
        float rainthing;
        float rainangle = winddirx;
        vec2 st =  (uv * vec2(.5 +(uv0.y +rain1) *rain2, .1) + vec2(rain3 + uv0.y *rainangle, t*rain4)) * rain5;
        float f = texture(iChannel0, st, -100.0).y * texture(iChannel0, st*.123, -100.0).x * 1.55;
        float rain = clamp(rainvol0 -.15, 0.0, 1.0);
        rainthing = clamp( pow( abs(f), 15.) * 1.0 * (rain*rain*5.), 0.0, (uv0.y +0.1) *0.6);
        test = clamp(mix(rainthing, rainvol0,f),0.,0.5);
        float fade = smoothstep(0.5,0.6,abs(1.-uv0.y));
        test = test * fade;
      
        if(rain7 ==1.)
        {
          vec2 uv2 = uv0 * (2.0) - (1.0);
          uv2.x *=AR;
          uv2.y +=Dfact;
          float rainthing2;
            vec2 st2 =  (uv2 * vec2(.5 +(uv0.y +2.) *.5, .1) + vec2(0. + uv0.y *rainangle, t*0.05));
          float f2 = texture(iChannel0, st2, -100.0).y * texture(iChannel0, st2*.123, -100.0).x * 1.55;
          float rain2a = clamp(rainvol -.15, 0.0, 1.0);
          rainthing2 = clamp( pow( abs(f2), 15.0) * 1.0 * (rain2a*rain2*5.), 0.0, (uv0.y +.1) *.6);
          test2 = clamp(mix(rainthing2, 0.5*rainvol,f2),0.,0.1);
          float fade2 = smoothstep(rain11,rain12,abs(1.-uv0.y));
          test2 = test2 * fade2;
        }
      }
    }
  }
  
  if(tonemap == 4.0)
    rayleigh = exp(-h/Hr) + (cld*0.5);
  else
    rayleigh = exp(-h/Hr);
    
  if(tonemap == 10.)
  {
    if(h<height_off && h>=0.)
      mie = exp((-h-rain10)/Hm1) + cld * + godray + test + test2 + test3 + haze ;
    else
      mie = exp((-h)/Hm1) + cld  + godray + test + test2 + test3 + haze;
  }
  else if (tonemap==5.0)
    mie = exp(-h/Hm1) + cld + godray + test + test2 + test3 + haze;
  else
    mie = exp((-h)/Hm1) + godray + cld + test + test2 + test3 + haze;

#ifdef cloud2
  mie += cld2;
#endif
}

void
densities (vec3 pos, float Hm1, out float rayleigh, out float mie)
{
  vec3 d = pos;
  d *=fov;
  
  float h = length(pos - C) - R0;
  rayleigh =  exp(-h/Hr);
  float dist = length(d);
    
  float cld = 0.;
  if (mincloudheight < h && h < maxcloudheight)
  {
    cld = cloud(pos+vec3(xaxiscloud,yaxiscloud, zaxiscloud),t)*cloudy;
    cld *= sin(3.1415*(h-mincloudheight)/mincloudheight) * cloudy;
  }
  
#ifdef cloud2
  float cld2 = 0.;
  if (12e3 < h && h < 15.5e3)
  {
    cld2 = fnoise2(pos*3e-4,t)*cloud(pos*32.0+vec3(27612.3, 0.,-t*15e3), t);
    cld2 *= sin(3.1413*(h-12e3)/12e3) * cloudyhigh;
    cld2 = clamp(cld2,0.0,1.0);
  }
#endif

#if ORIG_CLOUD
  if (dist<cloudfar)
  {
    float factor = clamp(1.0-((cloudfar - dist)/(cloudfar-cloudnear)),0.0,1.0);
    cld *= factor;
  }
#else
  float factor = clamp(1.0-((dist - cloudfar)/(cloudfar-cloudnear)),0.,10.);
  cld *= factor;
#endif

  mie = exp(-h/Hm1) + cld + haze;
#ifdef cloud2
  mie += cld2;
#endif
}

float
escape (vec3 p, vec3 d, float R)
{
  vec3 v = p - C;
  float b = dot(v, d);
  float c = dot(v, v) - R*R;
  float det2 = b * b - c;
  if (det2 < 0.)
    return -1.;
  float det = sqrt(det2);
  float t1 = -b - det, t2 = -b + det;
  return (t1 >= 0.) ? t1 : t2;
}

void
subscatter (float step_size_l, vec3 p, vec3 lightsource, float Hm1, float highclouds, float height_off, inout float depthRs, inout float depthMs)
{
  for (int j = 0; j < 4; ++j)
  {
    float ls = float(j) * step_size_l;
    vec3 ps = ( p + lightsource * ls );
    float dRs, dMs;
#if ENABLE_DENSE_RAIN2
    densities(p,highclouds,dRs,dMs,Hm1,height_off);
#else
    densities(ps,Hm1, dRs, dMs);
#endif
    depthRs += dRs * step_size_l;
    depthMs += dMs * step_size_l;
  }
}

void
lightcalc (float Ls, inout vec3 R, inout vec3 M, vec3 BR1, vec3 bM1, vec3 p, vec3 lightsource, float Hm1, float highclouds, float height_off, inout float dR, inout float dM, inout float depthR, inout float depthM)
{
  if (Ls > 0.)
  {
    float depthRs = 0., depthMs = 0.;
    float step_size_l = Ls / float(4);
    subscatter(step_size_l, p, lightsource, Hm1, highclouds, height_off, depthRs, depthMs);
    vec3 A = exp(-(BR1 * (depthRs + depthR) + bM1  * (depthMs + depthM)));
    R += (A * dR);
    M += A * dM;
  } 
}

void
scatter (vec3 camera_position, vec3 camera_vector, vec3 light_dir, vec2 uvMouse, vec3 moon_light_dir, vec3 aur_pos, vec3 aurr, vec3 BR1, vec3 bM1, float MI, float RI, float Hm1, float g1,out vec3 raleigh, out float scat,out vec3 themie,out vec3 thesun,out vec3 themoon, out float depthM,  out vec3 auro)
{
  float AR = iResolution.x/iResolution.y;
  float Dsfact = (0.5 - (Dfact/2.0));
  float ymax = Dsfact*0.8;
  float hz0 = Dsfact+0.08;

  vec3 light_dir2 = normalize(vec3(uvMouse.x-(0.5*AR), 0.08, (fov/-2.0)));

  vec3 bR2 =vec3( 13.5e-6, 13.5e-6, 13.5e-6 );
  vec3 aR2 = vec3( 0.000005, 0.000005, 0.000005 );

  float MIM = 8.;

  float g = g1/fov;
  float g2 = g * g;
  
  float mg = 0.999;
  float mg2 = mg;
  
  float au = rain8;
  float au2 = au * au;

  vec3 aur_v = aur_pos *4.;

  float mu = dot(camera_vector, light_dir);
  float mu_moon = dot(camera_vector,moon_light_dir);
  float mu_aur = dot(camera_vector,aur_v);
  
  float opmu2 = 1. + mu*mu;
  float opmu2_moon = 1. + mu_moon*mu_moon;
  float opmu2_aur = 1. + mu_aur*mu_aur;

  float phaseR = .0596831 * opmu2;
  float phaseM = .1193662 * (1. - g2) * opmu2 / ((2. + g2) * pow(1. + g2 - 2.*g*mu, 1.5));
  float phasenM = .1193662 * (1. - ng2) * opmu2 / ((2. + ng2) * pow(1. + ng2 - 2.*ng*mu, 1.5));
  float phaseS = .1193662 * (1. - s2) * opmu2 / ((2. + s2) * pow(1. + s2 - 2.*s*mu, 1.5));

  float phaseMoon = .1193662 * (1. - mg) * opmu2_moon / ((2. + mg) * pow(1. + mg - 2.*mg*mu_moon, 1.5));

  float phaseAur= 0.00596831 * opmu2_aur;

  float highclouds = 0.;
  float height_off = 0.;
  float depthR = 0.; 
  depthM=0.;
  vec3 R = vec3(0.), M = vec3(0.),R2 = vec3(0.), M2 = vec3(0.), MR = vec3(0.), MM = vec3(0.),AMR = vec3(0.), AMM = vec3(0.), FMR = vec3(0.), FMM = vec3(0.);

  float L = escape(camera_position, camera_vector, Ra);

  float step_size_i = 0.;
  step_size_i = L / float(8);

  for(int i=0; i < 8; ++i)
  {
    float l = float(i) * step_size_i;
    vec3 p = (camera_position + camera_vector * l);
    float dR, dM;
    
#if ENABLE_DENSE_RAIN
    densities(p,highclouds,dR,dM,Hm1,height_off) ;
#else
    densities(p,Hm1, dR, dM);
#endif

    dR *= step_size_i; dM *= step_size_i;
    depthR += dR;
    depthM += dM;

    float Ls = escape(p, light_dir, Ra);
    float Ls2 = escape(p, light_dir2, Ra);
    float mLs = escape(p, moon_light_dir,Ra);
    float AmLs = escape(p, aur_v,Ra);

    lightcalc(Ls, R , M, BR1, bM1, p, light_dir, Hm1, highclouds, height_off, dR, dM, depthR, depthM);

    if (uvMouse.y < hz0)
    {
      float depthRs = 0., depthMs = 0.;

      float step_size_l = Ls2 / float(4);
      subscatter(step_size_l, p, light_dir2, Hm1,highclouds, height_off, depthRs, depthMs);

      vec3 A = exp(-(BR1 * (depthRs + depthR) + bM1  * (depthMs + depthM)));
      R2 += (A * dR);
      M2 += A * dM; ;
    } 

    lightcalc(mLs, MR , MM, bR2, bM1, p, moon_light_dir, Hm1, highclouds, height_off, dR, dM, depthR, depthM);
    lightcalc(AmLs, AMR , AMM, aR2, bM1, p, aur_v, Hm1, highclouds, height_off, dR, dM, depthR, depthM);
    //lightcalc(fLs, FMR , FMM, aR2, bM1, p, fire, Hm1, highclouds, height_off, dR, dM, depthR, depthM);
  }

  if (uvMouse.y < hz0)
  {
    themie = (MI) *(M2 * bM1  * (phaseM ));
    depthM = depthM;
  }
  else
  {
    themie = (MI) *(M * bM1  * (phaseM ));
    depthM = depthM;
  }

  raleigh = (RI) *(max(R,Rm) * BR1 * phaseR);

#if NICE_HACK_SUN
  thesun = (SI) *(M * bM1  *phaseS); //Sun
#endif

  scat = 1.0 - clamp(depthM*3e-5,0.,0.99); 

  themoon = (MoonINT * moonillumination) *(MM * bM1 * phaseMoon);
  auro = (1.) * (AMM * bM1) * phaseAur;
}

vec3
get_camera_vector (vec2 resolution, vec2 coord, float Dfacta)
{
  vec2 uv = (coord.xy / resolution) * 2. - 1.;
  uv.x *= resolution.x / resolution.y;
  uv.y +=Dfacta;

  return normalize(vec3(uv.x, uv.y, -fov));
}

const float MAX = 10000.0;

float
diffuse (vec3 n, vec3 l, float p)
{
  return pow(dot(n,l) * 0.4 + 0.6,p);
}

float
specular (vec3 n, vec3 l, vec3 e, float s)
{
  float nrm = (s + 8.0) / (3.1415 * 8.0);
  return pow(max(dot(reflect(e,n),l),0.0),s) * nrm;
}

float
fresnel (vec3 n, vec3 e, float s)
{
  return pow(clamp(1.-dot(n,e), 0., 1.),s);
}

vec2
rotate (float angle, vec2 v)
{
  return vec2(cos(angle) * v.x + sin(angle) * v.y, cos(angle) * v.y - sin(angle) * v.x);
}

float
boolSub (float a, float b)
{
  return max(a,-b); 
}

float
sphere (vec3 p,float r)
{
  return length(p)-r;
}

mat3
fromEuler (vec3 ang)
{
  vec2 a1 = vec2(sin(ang.x),cos(ang.x));
  vec2 a2 = vec2(sin(ang.y),cos(ang.y));
  vec2 a3 = vec2(sin(ang.z),cos(ang.z));
  mat3 m;
  m[0] = vec3(a1.y*a3.y+a1.x*a2.x*a3.x,a1.y*a2.x*a3.x+a3.y*a1.x,-a2.y*a3.x);
  m[1] = vec3(-a2.y*a1.x,a1.y*a2.y,a2.x);
  m[2] = vec3(a3.y*a1.x*a2.x+a1.y*a3.x,a1.x*a3.x-a1.y*a3.y*a2.x,a2.y*a3.y);
  return m;
}

vec3
nSphere (vec3 pos, vec4 sph)
{
  return (pos-sph.xyz)/sph.w;
}

float
iSphere (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot( oc, rd );
  float c = dot( oc, oc ) - sph.w*sph.w;
  float h = b*b - c;
  if( h<0.0 )
    return -1.0;

  return -b - sqrt( h );
}

float
iCSphereF (vec3 p, vec3 dir, float r)
{
  float b = dot( p, dir );
  float c = dot( p, p ) - r * r;
  float d = b * b - c;
  if ( d < 0.0 )
    return -MAX;
  return -b + sqrt( d );
}

vec2
iCSphere2 (vec3 p, vec3 dir, float r)
{
  float b = dot( p, dir );
  float c = dot( p, p ) - r * r;
  float d = b * b - c;
  if ( d < 0.0 )
    return vec2( MAX, -MAX );
  d = sqrt( d );
  return vec2( -b - d, -b + d );
}

vec3
nPlane (vec3 ro, vec4 obj)
{
  return obj.xyz;
}

float
iPlane (vec3 ro, vec3 rd, vec4 pla )
{
  return (-pla.w - dot(pla.xyz,ro)) / dot( pla.xyz, rd );
}

const int   SEA_ITER_GEOMETRY = 3;
const int   SEA_ITER_FRAGMENT = 5;
const float SEA_EPSILON	= 1e-3;
#define SEA_EPSILON_NRM	(0.1 / iResolution.x)
const vec3  SEA_WATER_COLOR = vec3(0.8,0.9,0.6);
#define SEA_TIME (1.0 + iTime * SEA_SPEED)
const mat2  sea_octave_m = mat2(1.6,1.2,-1.2,1.6);

float
hash (vec2 p)
{
  float h = dot(p,vec2(127.1,311.7));	
  return fract(sin(h)*43758.5453123);
}

float
sea_noise (vec2 p)
{
  vec2 i = floor( p );
  vec2 f = fract( p );	

  vec2 u = f*f*(3.0-2.0*f);

  return -1.0+2.0*mix( 
    mix( hash( i + vec2(0.0,0.0) ), 
         hash( i + vec2(1.0,0.0) ), 
         u.x),
    mix( hash( i + vec2(0.0,1.0) ), 
         hash( i + vec2(1.0,1.0) ), 
         u.x), 
    u.y);
}

float
seaOctave (vec2 uv, float choppy)
{
  if(tonemap!=8.)
    uv += sea_noise(uv);
  else
    uv += noise(uv);
  vec2 wv = 1.0-abs(sin(uv));
  vec2 swv = abs(cos(uv));
  wv = mix(wv,swv,wv);
  return pow(1.0-pow(wv.x * wv.y,0.65),choppy);
}

float
seaMap (vec3 p)
{
  float freq = SEA_FREQ;
  float amp = SEA_HEIGHT;
  float choppy = SEA_CHOPPY;
  vec2 uv = p.xz; uv.x *= 0.75;

  float d, h = 0.0;    
  for(int i = 0; i < SEA_ITER_GEOMETRY; i++)
  {
    d = seaOctave((uv+SEA_TIME)*freq,choppy);
    d += seaOctave((uv-SEA_TIME)*freq,choppy);
    h += d * amp;
    uv *= sea_octave_m; freq *= 1.9; amp *= 0.22;
    choppy = mix(choppy,1.0,0.2);
  }
  return p.y - h;
}

float
seaMapHigh (vec3 p)
{
  float freq = SEA_FREQ;
  float amp = SEA_HEIGHT;
  float choppy = SEA_CHOPPY;
  vec2 uv = p.xz; uv.x *= 0.75;

  float d, h = 0.0;    
  for(int i = 0; i < SEA_ITER_FRAGMENT; i++)
  {
    d = seaOctave((uv+SEA_TIME)*freq,choppy);
    d += seaOctave((uv-SEA_TIME)*freq,choppy);
    h += d * amp;        
    uv *= sea_octave_m; freq *= 1.9; amp *= 0.22;
    choppy = mix(choppy,1.0,0.2);
  }
  return p.y - h;
}

vec3
getSkyColor (vec3 e, vec3 skycolor)
{
  e.y = (max(e.y,0.0)*0.8+0.2)*0.8;
  if (tonemap==3.)
    return  skycolor;
  return vec3(pow(1.0-e.y,2.0), 1.0-e.y, 0.6+(1.0-e.y)*0.4) * 1.1;
}

vec3
hsv (float shue, float ssaturation, float value)
{
  vec4 t = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(vec3(shue) + t.xyz) * 6.0 - vec3(t.w));
  return value * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), ssaturation);
}

vec3
getSeaColor (vec3 p, vec3 n, vec3 l, vec3 eye, vec3 dist, vec3 skycolor, float spec1, vec3 moon_light, float mLight, float scat)
{
  float RAINBOW_WATER_SATURATION	= 0.35;
  float RAINBOW_WATER_LIGHTNESS		= 0.1; //0.2
  float RAINBOW_WATER_SPEED 		= 0.1;
  float fresnel = clamp(1.0 - dot(n,-eye), 0.0, 1.0);
  fresnel = pow(fresnel,3.0) * 0.5;

  vec3 reflected = getSkyColor(reflect(eye,n),skycolor);

  if(tonemap==7.)
  {
    vec3 weights_ = vec3(0.2125, 0.7154, 0.0721);
    float luminance_ = dot(reflected, weights_);
    reflected = mix(vec3(luminance_), reflected, 0.1);
  }
  vec3 refracted = SEA_BASE + diffuse(n,l,diffusepow) * SEA_WATER_COLOR * 0.12; 

  vec3 color = mix(refracted,skycolor,fresnel) * skycolor ;

  float atten = max(1.0 - dot(dist,dist) * 0.001, 0.0);
  color +=  clamp(SEA_WATER_COLOR * (p.y - SEA_HEIGHT) * 0.08 * atten,0.,1.) ;

  if(spectype==0.)
    color += clamp(vec3(specular(n,l,eye,60.))* skycolor * specbrightness,0.,1.);
  if(spectype==1.)
    color += clamp(vec3(specular(n,l,eye,spec1))* skycolor * specbrightness,0.,1.);
  if(spectype==2.)
    color += clamp(vec3(specular(n,l,eye,spec1*scat))* skycolor * specbrightness,0.,1.);
  if(spectype==3.)
    color += clamp(vec3(specular(n,l,eye,clamp(1000.0*scat,10.,60.)))* skycolor * specbrightness,0.,1.);
  if(spectype==4.)
    color += clamp(vec3(specular(n,l,eye,clamp(1000.0*scat,10.,60.)))* skycolor * scat,0.,1.);
  if(spectype==5.)
    color += clamp(vec3(specular(n,l,eye,clamp(1000.0*scat,10.,60.)))* skycolor * scat* specbrightness,0.,1.);
  if(spectype==6.)
    color += clamp(vec3(specular(n,l,eye,spec1))* skycolor * scat* specbrightness,0.,1.);
  if(spectype==7.)
    color += clamp(vec3(specular(n,l,eye,60.))* skycolor * scat* specbrightness,0.,1.);
  color += vec3(specular(n,moon_light,eye,moonspec))* skycolor * moonillumination*mLight;

  color = pow(color,vec3(0.65));

  return color;
}

vec3
seaGetNormal (vec3 p, float eps)
{
  vec3 n;
  n.y = seaMapHigh(p);    
  n.x = seaMapHigh(vec3(p.x+eps,p.y,p.z)) - n.y;
  n.z = seaMapHigh(vec3(p.x,p.y,p.z+eps)) - n.y;
  n.y = eps;
  return normalize(n);
}

float
seaheightMapTracing (vec3 ori, vec3 dir, out vec3 p)
{
  float tm = 0.0;
  float tx = 1000.0;
  float hx = seaMap(ori + dir * tx);
  if(hx > 0.0) return tx;   
  float hm = seaMap(ori + dir * tm);    
  float tmid = 0.0;
  
  for(int i = 0; i < int(SEA_NUM_STEPS); i++)
  {
    tmid = mix(tm,tx, hm/(hm-hx));                   
    p = ori + dir * tmid;                   
    float hmid = seaMap(p);
    if(hmid < 0.0)
    {
      tx = tmid;
      hx = hmid;
    }
    else
    {
      tm = tmid;
      hm = hmid;
    }
  }

  return tmid;
}


vec3
seaTransform (vec3 x)
{
  x.yz = rotate( 0.8, x.yz );
  return x;
}

vec3
seaUntransform (vec3 x)
{
  x.yz = rotate( -0.8, x.yz );
  return x;
}

float
noisef2 (float t_)
{
  vec2 texSize = vec2(textureSize(iChannel0, 0));
  return texture(iChannel0, vec2(t_, .0) / texSize).x;
}

float
noisev2 (vec2 t_)
{
  vec2 texSize = vec2(textureSize(iChannel0, 0));
  return texture(iChannel0, t_ / texSize).x;
}

vec3
lensFlare (vec2 uv, vec2 pos, float whichtype)
{
  vec2 main = uv-pos;
  vec2 uvd = uv*(length(uv));

  float ang = atan(main.x,main.y);
  float dist=length(main); dist = pow(dist,.1);
  float n = noisev2(vec2(ang*16.0,dist*32.0));

  float f0 = 1.0/(length(uv-pos)*16.0+6.0);

  f0 = f0*(sin(noisef2(sin(ang*2.+pos.x-(sin(iTime*0.05))*2.)*4.0 - cos(ang*4.+pos.y+cos(iTime*0.15)))*16.)*.1 + dist*.1 + .8)*2.2;

  float f1 = max(0.01-pow(length(uv+1.2*pos),1.9),.0)*7.0;
  float f2,f22,f23;
  float f4,f42,f43;
  float f5,f52,f53;
  float f6,f62,f63;

  if(whichtype == 2.)
  {
    f2 = max(1.0/(1.0+32.*pow(length(uv-0.05-pos),1.5)),.0)*0.25;
    f22 = max(1.0/(1.0+32.*pow(length(uv+0.0-pos),1.5)),.0)*0.23;
    f23 = max(1.0/(1.0+32.*pow(length(uv+0.05-pos),1.5)),.0)*0.25;
  }
  else
  {
    f2 = max(1.0/(1.0+32.0*pow(length(uvd+0.8*pos),2.0)),.0)*0.25;
    f22 = max(1.0/(1.0+32.0*pow(length(uvd+0.85*pos),2.0)),.0)*0.23;
    f23 = max(1.0/(1.0+32.0*pow(length(uvd+0.9*pos),2.0)),.0)*0.21;
  }
  vec2 uvx = mix(uv,uvd,-0.5);

  if(whichtype == 2.)
  {
    f4 = max(0.01-pow(length(uv+0.0-pos),1.4),.0)*6.0;
    f42 = max(0.01-pow(length(uv+0.0-pos),1.4),.0)*5.0;
    f43 = max(0.01-pow(length(uv+0.0-pos),1.4),.0)*3.0;
  }
  else
  {
    f4 = max(0.01-pow(length(uvx+0.4*pos),2.4),.0)*6.0;
    f42 = max(0.01-pow(length(uvx+0.45*pos),2.4),.0)*5.0;
    f43 = max(0.01-pow(length(uvx+0.5*pos),2.4),.0)*3.0;
  }

  uvx = mix(uv,uvd,-.4);

  if(whichtype == 2.)
  {
    f5 = max(0.01-pow(length(uv+0.0-pos),2.5),.0)*2.0;
    f52 = max(0.01-pow(length(uv+0.0-pos),2.5),.0)*2.0;
    f53 = max(0.01-pow(length(uv+0.0-pos),2.5),.0)*2.0;
  }
  else
  {
    f5 = max(0.01-pow(length(uvx+0.2*pos),5.5),.0)*2.0;
    f52 = max(0.01-pow(length(uvx+0.4*pos),5.5),.0)*2.0;
    f53 = max(0.01-pow(length(uvx+0.6*pos),5.5),.0)*2.0;
  }

  uvx = mix(uv,uvd,-0.5);

  if(whichtype == 2.)
  {
    f6 = max(0.01-pow(length(uv+0.0-pos),1.6),.0)*6.0;
    f62 = max(0.01-pow(length(uv+0.0-pos),1.6),.0)*3.0;
    f63 = max(0.01-pow(length(uv+0.0-pos),1.6),.0)*5.0;
  }
  else
  {
    f6 = max(0.01-pow(length(uvx-0.3*pos),1.6),.0)*6.0;
    f62 = max(0.01-pow(length(uvx-0.325*pos),1.6),.0)*3.0;
    f63 = max(0.01-pow(length(uvx-0.35*pos),1.6),.0)*5.0;
  }

  vec3 c = vec3(.0);

  if(whichtype == 2.)
  {
    c.r+=f2+f4+f5+f6; 
    c.g+=f22+f42+f52+f62; 
    c.b+=f23+f43+f53+f63;
  }
  else
  {
    c.r+=f2+f4+f5+f6; 
    c.g+=f22+f42+f52+f62; 
    c.b+=f23+f43+f53+f63;
  }

  c = c*1.3 - vec3(length(uvd)*.05);
  c+=vec3(f0);

  return c;
}

float SIZE_RATE = 0.1;
float XSPEED = 0.5;
float YSPEED = 0.75;
float LAYERS = 20.;

float
Hash11 (float p)
{
  vec3 p3 = fract(vec3(p) * 0.1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z); 
}

vec2
Hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * 0.3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec2
Rand22 (vec2 co)
{
  float x = fract(sin(dot(co.xy ,vec2(122.9898,783.233))) * 43758.5453);
  float y = fract(sin(dot(co.xy ,vec2(457.6537,537.2793))) * 37573.5913);
  return vec2(x,y);
}

vec3
SnowSingleLayer (vec2 uv, float layer)
{
  vec3 acc = vec3(0.0,0.0,0.0);
  uv = uv * (2.0 + layer);
  float xOffset = uv.y * (((Hash11(layer)*2.-1.)*0.5+1.)*XSPEED);
  float yOffset = YSPEED * iTime;
  uv += vec2(xOffset,yOffset);
  vec2 rgrid = Hash22(floor(uv)+(31.1759*layer));
  uv = fract(uv) - (rgrid*2.-1.0) * 0.35 - 0.5;
  float r = length(uv);
  float circleSize = 0.04*(1.5+0.3*sin(iTime*SIZE_RATE));
  float val = smoothstep(circleSize,-circleSize,r);
  vec3 col = vec3(val,val,val)* rgrid.x  ;
  return col;
}

float
calcvalue (float pos, float min1, float max1, float bmin, float bmax)
{
  float val = 0.;
  val = clamp(((max1-min1) * ((pos-bmin)/(bmax-bmin))) + min1,min1,max1);
  return val;
}

vec3
getPixel (vec2 coord, float time, vec3 skycolor, vec3 light_dir, float spec1, vec3 moon_light_dir2, float mLight, float scat)
{
  vec2 uv = coord / iResolution;
  uv = uv * 2.0 - 1.0;
  uv.x *= iResolution.x / iResolution.y;
  uv.y += Dfact;

  vec3 ang = vec3(0.0,0.0,0.0);
  vec3 ori = vec3(xaxiscloud*0.001,3.5,zaxiscloud*0.001);
    
  vec3 dir = normalize(vec3(uv.x,uv.y,-fov));
  vec3 dir2 = dir;
    
  mat3 rotZ = rotateZ(camera_T_rotate);
  mat3 rotY = rotateY(camera_H_rotate);
  mat3 rotX = rotateX(camera_V_rotate);

  dir = normalize(dir) * rotX * rotY * rotZ;

  vec3 p;
  seaheightMapTracing(ori,dir,p);
  vec3 dist = p - ori;

  vec3 n = seaGetNormal(p, dot(dist,dist) * SEA_EPSILON_NRM);

  vec3 moon_light = normalize(moon_light_dir2);
  vec3 light = normalize(vec3(light_dir.x,light_dir.y,light_dir.z));

  if (tonemap==3.)
  {
    return mix(skycolor,
               getSeaColor(p,n,light,dir,dist,skycolor,spec1,moon_light,mLight,scat),
               pow(smoothstep(0.0,-0.02,dir.y),0.9));
  }
  return getSeaColor(p,n,light,dir,dist,skycolor,spec1,moon_light,mLight,scat);
}

#define mul3x(x) 				x * x * x
#define RAINBOW_START_Y					0.0

vec2
_smoothstep (vec2 f)
{
  return f * f * (3.0 - 2.0 * f);
}

vec3
_smoothstep (vec3 p)
{
  return p * p * 3.0 - 2.0 * mul3x(p);
}

vec3
rainbowColor (vec3 ray_dir, vec3 light_dir)
{
  float RAINBOW_INTENSITY = 0.3;
  vec3 RAINBOW_COLOR_RANGE = vec3(56.0, 53.0, 50.0);

  vec3 RAINBOW_DIR = vec3(-0.2, -0.1, 0.0);
  RAINBOW_DIR = normalize(light_dir);

  float theta = degrees(acos(dot(RAINBOW_DIR, ray_dir)))*1.;
  vec3 nd = saturate(1.0 - abs((RAINBOW_COLOR_RANGE - theta) * 0.2));
  vec3 color = _smoothstep(nd) * RAINBOW_INTENSITY;

  return color * max((RAINBOW_BRIGHTNESS - 0.75) * 1.5, 0.0);
}

vec3
rainbow (vec2 fragCoord, vec3 light_dir)
{
  vec2 p = fragCoord;
  float Dsfact = 0.5 - (Dfact/2.0);
  vec3 RAINBOW_POS	= vec3(0., -0.5, 0.5);
  vec3 color = vec3(0.0);

  if (p.y >= RAINBOW_START_Y)
  {
    vec3 rainbowW   	  = -normalize(-RAINBOW_POS);
    vec3 rainbowUp  	  = normalize(cross(rainbowW, vec3(0.0, 1.0, 0.0)));
    vec3 rainbowVertical = normalize(cross(rainbowUp, rainbowW));

    vec3 dir = normalize(vec3(p, fov) - vec3(0.0, 0.25, rb1));
    vec3 wdDir = normalize(dir.x * rainbowUp + dir.y * rainbowVertical - dir.z * rainbowW);

    color += rainbowColor(wdDir,light_dir);
  }
  return saturate(color);
}

vec3
rainbow2 (vec2 fragCoord, vec3 light_dir)
{
  vec2 uv = fragCoord.xy / iResolution;
  vec2 p = (-1.0 + 2.0 * uv) * vec2(iResolution.x / iResolution.y, 1.0);
  p.y +=Dfact ;
  vec3 RAINBOW_POS	= vec3(4.5, -0.5, 0.5);
  vec3 color = vec3(0.0);

  if (p.y >= RAINBOW_START_Y)
  {
    vec3 rainbowW   	  = -normalize(-RAINBOW_POS);
    vec3 rainbowUp  	  = normalize(cross(rainbowW, vec3(0.0, 1.0, 0.0)));
    vec3 rainbowVertical = normalize(cross(rainbowUp, rainbowW));

    vec3 dir = normalize(vec3(p, 0.0) - vec3(0.0, 0.0, -1.5));
    vec3 wdDir = normalize(dir.x * rainbowUp + dir.y * rainbowVertical - dir.z * rainbowW);

    color += rainbowColor(wdDir,vec3(-0.2, -0.1, 0.0));
  }
  return saturate(color);
}

vec3
rainbowhalo (vec2 fragCoord, vec3 light_dir)
{
  vec2 p = fragCoord;
  float Dsfact = 0.5 - (Dfact/2.0);
  vec3 RAINBOW_POS	= vec3(0., 0., 0.15);
  vec3 color = vec3(0.0);

  if (p.y >= RAINBOW_START_Y)
  {
    vec3 rainbowW   	  = -normalize(-RAINBOW_POS);
    vec3 rainbowUp  	  = normalize(cross(rainbowW, vec3(0.0, 1.0, 0.0)));
    vec3 rainbowVertical = normalize(cross(rainbowUp, rainbowW));

    vec3 dir = normalize(vec3(p, -fov) - vec3(0.0, 0.25, 1.5));
    vec3 wdDir = normalize(dir.x * rainbowUp + dir.y * rainbowVertical - dir.z * rainbowW);

    color += rainbowColor(wdDir,light_dir);
  }
  return saturate(color);
}

vec3
shiftHue (vec3 col, float Shift)
{
  vec3 P = vec3(0.55735) * dot(vec3(0.55735), col);
  vec3 U = col - P;
  vec3 V = cross(vec3(0.55735), U);    
  col = U * cos(Shift * 6.2832) + V * sin(Shift * 6.2832) + P;
  return col;
}

vec4
circle (vec2 uv, vec2 pos, float rad, vec3 color)
{
  float d = length(pos - uv) - rad;
  float t = clamp(d, 0.0, 1.0);
  return vec4(color, d);
}

vec3
circle2 (vec2 uv, vec2 pos, float rad, vec3 color)
{
  float d = length(pos - uv) - rad;
  float t_ = clamp(d, 0.0, 1.0);
  return color * d;
}

vec3
DrawCircle (vec2 uv, float radius, float fade)
{
  float uvLength = length(uv);
  vec3 circle = vec3(smoothstep(radius, radius - fade, uvLength));
    
  return vec3(circle);
}

void
main ()
{
  float AR = iResolution.x/iResolution.y;
  float M = 1.;
  float Dsfact = 0.5 - (Dfact/2.0);

  vec2 uvMouse = (iMouse.xy / iResolution);
  uvMouse.x *= AR;

  vec2 uvMouseN = ((iMouse.xy / iResolution)) * 2.0 - 1.0;

  if (uvMouse.y == 0.) uvMouse.y=(0.5-(0.05*fov));
  if (uvMouse.x == 0.) uvMouse.x=(1.0-(0.05*fov));
  
  mat2 ma = mat2(cos(moon_rotate), -sin(moon_rotate), sin(moon_rotate), cos(moon_rotate));

  mat3 rotZ = rotateZ(camera_T_rotate);
  mat3 rotY = rotateY(camera_H_rotate);
  mat3 rotX = rotateX(camera_V_rotate);

  float moonsz = 1.0;
  float moon_x2 = uvMouse.x-(0.5*AR);
  float mlightoffsetx = 0.;
  float moonpos_y =sin((uvMouseN.x) * 1.5);

  float uvMoonpos_y = (moonpos_y); 
  float uvMoonpos_y3 = ((moonpos_y) / 2.) + 0.125;
  float uvMoonpos_y2 = (moonpos_y)-Dsfact;

  float moon_size = moonsz *M;

  vec2 q = (gl_FragCoord.xy /(iResolution) ) * 2.0 - 1.0;

  float mfixy = (uvMoonpos_y2);

  vec2 uv0 = (gl_FragCoord.xy / iResolution);
  uv0 *= M;

  vec2 uv = uv0 * (2.0*M) - (1.0*M);
  uv.x *=AR;

  vec3 D_moon = normalize(vec3(q, -(fov*M)));

  mat2 mar = mat2(cos(t*0.9), -sin(t*0.9), sin(t*0.9), cos(t*0.9));

  vec2 uvMouse0 = (iMouse.xy / iResolution);
  float uvM = radians(uvMouse0.x*360.);

  mat2 ma2 = mat2(cos(uvM), -sin(uvM), sin(uvM), cos(uvM));
  float mlightoffsetx1 = (mlightoffsetx -0.5)/10.0;

  vec3 camera_position = vec3(0,camera_height, camera_depth);
  vec3 camera_vector = get_camera_vector(iResolution, gl_FragCoord.xy, Dfact)* rotZ * rotY * rotX;

  float uvMo = uvMouse.x - (0.5 * AR) ;
    
  vec3 light_dir = normalize(vec3(uvMo, uvMouse.y-Dsfact, (fov/-2.0)));
  vec3 moon_light_dir = normalize(vec3(moon_x2, uvMoonpos_y3 , (fov/-2.0)));
  vec3 light_dir_original = normalize(vec3(uvMouse.x-(0.5*AR), clamp(uvMouse.y,0.,1.), (fov/-2.0)));

  vec3 color = vec3(0.);
  vec3 color2 = vec3(0.);
  vec3 raleigh = vec3(0.);
  vec3 themoon = vec3(0.);
  vec3 thesun = vec3(0.);
  vec3 themie = vec3(0.);
  vec3 star = vec3(0.);
  vec3 lensf = vec3(0.);
  vec3 fireworkCol = vec3(0.);
  vec3 fire = vec3(0.);
  vec3 auro = vec3(0.);
  vec4 aur = vec4(0.);
  vec3 a_pos = vec3(0.);

  float scat = 0.;
  float depthM = 0.;
  float att = 1.;
  float refatt = 1.;
  float att_moon = 1.;
  float sunatt = 1.;
  float sunscat = 1.;
  float moonatt = 1.;
  float thing = 1.;

  float staratt = 1.1 -min(1.0,pow(uvMouse.y+((Dfact/2.)+0.51),16.));
  float scatatt = 1.0 -min(1.0,clamp((clamp(uvMouse.y,0.,1.)*((Dfact*4.)+2.)),0.55,1.));

  float auratt = 1. -min(1.0,pow(uvMouse.y+0.51,64.));

  float fade = smoothstep(0.,0.01,abs(camera_vector.y))*0.1+0.9;

  vec3 bg1 = vec3(0.);
  bg1 = bg(camera_vector) * fade;

  float MI2 = 1.;
  
  vec2 uvl = gl_FragCoord.xy / iResolution - 0.5;
  uvl.x *= iResolution.x/iResolution.y;

  float hz = Dsfact;
  float hzO = 0.08;
  float hz0 = hz+hzO;
  float hz1 = hz0*0.84; 
  float hz2 = Dsfact*0.84;
  float hd = hz0-hz1;
  float hz1a = hz*0.84;
  float hda = hz-hz1a;

  float g1 = g0;
  float gmax = g0;
  float gmin = 0.1;

  float newx = uvMouse.x-(0.5*AR);
  float newx2 = 0.;
  float diffEI = (uvMoonpos_y3) - (uvMouse.y-Dsfact);
  float diffEIp = 1.-abs(diffEI/eclispethreshold);

  float Hm1 = Hm;
  float HmMax =Hm;
  float HmMin =HmMin1;

  float RR1 = rr;
  float RR1Max = rr;
  float RR1Min = RRMin;

  float gam = 1.0;
  float gamMax =miegam;
  float gamMin =miegamin;

  float rgam = raleighgam;
  float rgamMax =raleighgam;
  float rgamMin =raleighgammin;
  
  float spec1 = spec;
  float specMax = spec;
  float specMin = specmin;
  float symax = Dsfact * 0.6;
  float symin = Dsfact;

  float spymax = Dsfact * 0.8;
  float spymin = Dsfact;
  float uvyD = (uvMouse.y-Dsfact);

  float ymax = Dsfact * 0.8;
  float ymin = Dsfact * 0.6;

  float lensb = lensflareb;
  float lensbMax = lensflareb;

  float mLight = 0.;
  float mLightMax =1.;
  float mLightMin = 0.;

  float bM1 = bM.x;
  float bMMax = bM.x;
  float bMMin = 0.000021;
  
  vec3 BM1 = bM;

  float MI1 = MI0;
  float MImax = MI0;
  float MImin = MIMin1;

  float RI1 = RI0;
  float RImax = RI1;
  float RImin = 0.;
  
  float EI = 0.;
  float EImax = MI0;
  float EImin1 = 0.;
  float RE =rain13;

  float EImin = clamp(((EImax-EImin1)* ((moonillumination-1.)/(0.-1.))) + EImin1,EImin1,EImax);

  float sLight = 0.;
  float sLightMax =2.;
  float sLightMin =hzO;

  bM1 = clamp(((bMMax-bMMin) * ((uvMouse.y-(Dsfact*1.5))/(1.-(Dsfact*1.5)))) +   bMMin,bMMin,bMMax);
  vec3 bM11 = vec3(bM1);

  vec3 BR1;

  float uvMyin_ymaxymin = (uvMouse.y-ymin)/(ymax-ymin);
  float uvMyin_ymaxymin2 = (uvMouse.y-ymax)/(0.5-ymax);
  float uvMyin_ymaxymin3 = (uvMouse.y-Dsfact)/(0.5-Dsfact);
  float uvMyin_ymaxymin4 = (uvMouse.y-hz0)/(0.5-hz0);
  float uvMyin_ymaxymin5 = (uvMouse.y-Dsfact)/(0.3-Dsfact);
  float uvMyin_ymaxyminA = (uvMouse.y-ymax)/(Dsfact-ymax);
  
  if(sealight==0.)
    uvyD = sLight;

  newx2 = uvMouse.x-(0.5*AR);
  vec3 sea_light_dir = normalize(vec3(newx2, uvyD, (fov/-2.0)));
  vec3 moon_light_dir2 =vec3(0.);
  
#if EXPENSIVE_CALC
  mLight = calcvalue(uvMouse.y, mLightMin, mLightMax,spymin,spymax);
  sLight = calcvalue(uvMouse.y, sLightMin, sLightMax,hz0,1.);
  spec1 = clamp(((specMin-specMax) * ((uvMouse.y-Dsfact)/(0.75-Dsfact))) + specMax,specMin,specMax);
  rgam = clamp(((rgamMax-rgamMin) * (uvMyin_ymaxymin3)) + rgamMin,rgamMin,rgamMax);
  gam = clamp(((gamMin-gamMax) * (uvMyin_ymaxymin2)) + gamMax,gamMin,gamMax);
#endif

  if(tonemap==9.)
  {
#if EXPENSIVE_CALC
    Hm1 = clamp(((HmMax-HmMin) * (uvMyin_ymaxymin4)) + HmMin,HmMax,HmMin);
    RR1 = clamp(((RR1Max-RR1Min) * (uvMyin_ymaxymin5)) + RR1Min,RR1Min,RR1Max);
#endif
    BR1 = vec3(RR1,rg,rb);
  }
  else
    BR1 = vec3(RR1,rg,rb);
  
  if (uvMouse.y < Dsfact)
  {
    light_dir = normalize(vec3(newx, -(0.01), (fov/-2.0)));
    moon_light_dir2 = normalize(vec3((uvMouse.x-mlightoffsetx1)-(0.5*AR), uvMoonpos_y2 , (fov/-2.0)));
    spec1 = clamp(((specMax-specMin) * ((uvMouse.y-spymax)/(symin-spymax))) + specMin,specMin,specMax);
    gam = clamp(((gamMax-gamMin) * (uvMyin_ymaxymin)) + gamMin,gamMin,gamMax);
    lensb = clamp(((lensbMax-0.) * (uvMyin_ymaxyminA)) + 0.,0.,lensbMax);
    att = 1.;
    sunatt = 0.; 
    staratt = min(0.7,staratt);
    
    if(tonemap!=9.)
    {
#if EXPENSIVE_CALC
      Hm1 = clamp(((HmMax-HmMin) * ((uvMouse.y-spymax)/(symin-spymax))) + HmMin,HmMin,HmMax);
#endif
    }
    else
    {
      Hm1 = clamp((((HmMin-HmMax) * ((uvMouse.y-0.25)/(0.25-0.2)))) + HmMin,HmMax,HmMin);	
    }
  }
  
  if (uvMouse.y < hz0)
  {
    sLight = clamp(((sLightMax-sLightMin) * ((uvMouse.y-symin)/(symax-symin))) + sLightMin,sLightMin,sLightMax);
    sea_light_dir = normalize(vec3(newx2, sLight, (fov/-2.0)));
    g1 = clamp(((gmax-gmin) * (uvMyin_ymaxymin)) + gmin,gmin,gmax);
    MI1 = clamp(((MImax-MImin) * (uvMyin_ymaxymin)) + MImin,MImin,MImax);

    if(tonemap!=9.)
      Hm1 = clamp(((HmMax-HmMin) * (uvMyin_ymaxymin)) + HmMin,HmMin,HmMax);
  
    if (uvMouse.y < Dsfact * 0.95);
    else
    {
      if (diffEI >= -eclispethreshold && diffEI <= eclispethreshold)
      {
        EI = clamp((((EImax-EImin) * (abs(diffEI)/(eclispethreshold-0.))) + EImin),EImin,EImax);
        RE = clamp((((rain13-1.) * (abs(diffEI)/(eclispethreshold-0.))) + 1.),0.,1.);
        MI1 = EI;
      }
    }

    RI1 = clamp(((RImax-RImin) * ((uvMouse.y-ymin)/(ymax-ymin))) + RImin,RImin,RImax);
 
    att = 1.;
    staratt = min(0.7,staratt);
  }
  else
  {
    if (diffEI >= -eclispethreshold && diffEI <= eclispethreshold)
    {
      EI = clamp((((EImax-EImin) * (abs(diffEI)/(eclispethreshold-0.))) + EImin),EImin,EImax);
      RE = clamp((((rain13-1.) * (abs(diffEI)/(eclispethreshold-0.))) + 1.),0.,1.);
      MI1 = EI;
    }
  }

  vec2 b,b1,b2;
  b = b1 = b2 = (gl_FragCoord.xy /(iResolution) ) * 2.0 - 1.0;
  b1.y +=Dfact;

  float z = 1.2;
  if (AR<1.0)
    z = 2.4;
  else
    z = 1.2;

  float zoom = (2.0 *AR) * z;
  b /= zoom;
  b.x *= AR;
  b1.x *= AR;

#if SIMPLE_SUN
  vec2 uv1 = (gl_FragCoord.xy / iResolution);
  uv1 *=M;
  uv1.x *=AR;

  float zenithoffset = (1.-Dfact)*0.48;

  vec3 sun2 = getAtmosphericScattering(uv1, uvMouse, zenithoffset) ;
#endif

  if (camera_vector.y < -0.01)
  {
    float L = - camera_position.y / camera_vector.y;
    camera_position = camera_position + camera_vector * L;
    camera_vector.y = -camera_vector.y;
    q.y = (Dfact *-(moonsz * 2.)) -q.y;
    b1.y = -b1.y;
    b.y = -b.y;

    refatt = .6;
    att_moon = .2;
    star = stars(vec3(camera_vector.xy*ma2,camera_vector.z -camz));
    if( uvMouse.y < Dsfact)
    {
      aurora(vec3(0.,camera_height/10000.,(camera_depth/10000.)-6.7),camera_vector,a_pos,aur);
      aur = smoothstep(0.0,2.5,aur);
    }
    else
      aur = aur;
  }
  else
  {
    float L1 =  camera_position.y / camera_vector.y;
    vec3 O1 = camera_position + camera_vector * L1;
    vec3 D1 = vec3(1.);
    D1 = normalize(camera_vector +vec3(1., 0.0009*sin(t+6.2831 * noise(O1.xz +vec2(0.,t*0.8))), 0.));
    D1.xy *=ma2;
    star = stars(D1);
    if( uvMouse.y < Dsfact)
    {
      aurora(vec3(0.,camera_height/10000.,(camera_depth/10000.)-6.7),camera_vector,a_pos,aur);
      aur = smoothstep(0.0,1.5,aur)* fade;
    }
    else
      aur = aur;
  }
  
  q.y > (0.2 * (moon_size/2.)/M) ? thing = 1.:  thing = (q.y / ((moon_size/2.)/M))*5.;

  q.x += -uvMouseN.x;
  q.y += -mfixy;

  q.x *= AR;

  b.xy *=ma2;
  b +=0.5;
  
  if (fireworkson > 0.)
    fireworks(b1,MI2,fire,fireworkCol);
    
  vec3 moon_texture1= textureLod(iChannel1,q, moonlod ).rgb;

  vec4 layer1 = vec4(0.0, 0.0, 0.0, 1.0);
  vec3 layer2 = DrawCircle(q,0.05,0.01);
  vec3 moon_texture = layer2*moon_texture1.rgb;
  moon_texture = pow(moon_texture,vec3(2.2));

#if ENABLE_MOON_2
  themoon *= moon_texture;
  themoon *= att_moon;
  color += themoon;
#endif

#if ENABLE_MOON
  moon_texture *=att_moon;
#endif

  vec3 Dn = camera_vector;
  Dn.xy *=ma2; 
  Dn += 0.5;

  vec3 back_texture = textureLod(iChannel2,vec2(b.x,b.y), 0. ).xyz;
  back_texture = pow(back_texture,vec3(2.2))*0.8;;

  vec3 newmie = vec3(0.);
  vec3 nightmie = vec3(0.);

  scatter(camera_position, camera_vector,light_dir,uvMouse,moon_light_dir,a_pos,aur.rgb,BR1,bM11,MI1,RI1,Hm1,g1,raleigh,scat,themie,thesun,themoon,depthM,auro);

  float seed = round(t, 1.);
  float metx = random(seed);

  vec3 met = meteorstorm(vec2(camera_vector.x - metx,camera_vector.y+0.5));

  float scatfactor = ((abs(camera_vector.y)/0.5) * 0.00003) +0.00001;

  scat = 1.0 - clamp(depthM*scatfactor,0.,1.);
  
  themie *=  refatt;
  raleigh *= refatt;
  
  if (uvMouse.y > hz0);
  else
    sunscat = scat - (-1. +((uvMouse.y-hz0)/(hz0-hz1)));
  
  float ralatt = 1.0;
  float mieatt = 1.0;
  if (uvMouse.y >= Dsfact);
  else
  {
    vec3 weights2_ = vec3(0.2125, 0.7154, 0.0721);
    float newmie2 = dot(themie, weights2_);
    newmie = mix(themie,vec3(newmie2),1. -(max(hz2,uvMouse.y)-hz2)/0.04);
    ralatt = (max(0.2,uvMouse.y)-0.2)/(0.25-0.2);
    mieatt = clamp((max(hz2,uvMouse.y)-hz2)*25.,0.15,1.0);

    color +=vec3(0.0,0.0,0.01* (1. -(max(hz2,uvMouse.y)-hz2)/0.04) );
    themie = newmie;
    themie = themie;
  }

  color += back_texture.rgb * scat * scatatt;

  themoon *= moonatt;
  thesun *=  sunatt;

  star *=    staratt * refatt * scat;
  met *=     staratt * refatt * scat;
  fire *=     staratt * refatt * scat;

  vec3 mcolor =((vec3(scat)))*themie;

  if (diffEI >= -eclispethreshold && diffEI <= eclispethreshold)
  {
    vec3 newsun = clamp(thesun,0.,1.);
    vec3 newsun2 = clamp(sun2,0.,1.);
    color += (themoon*moonillumination*scat) * (1.-diffEIp);
    color += moon_texture.rgb * scat * (1.-diffEIp);
    vec3 moon2 = clamp(moon_texture.rgb*1000.,0.,1.);
    color += clamp(newsun-(moon2),0.,1.)  * refatt * (scat);
    color += clamp(newsun2-(moon2),0.,1.)  * refatt * (scat);
  }
  else
  {
    if(tonemap==0. || tonemap==9.)
    {
      mcolor *=moon_texture.rgb * 30.;
      color += mix(mcolor,color,0.9);
    }
    color += sun2 * (scat);
    color += thesun* (scat);
    color += (themoon*moonillumination*scat);
    color += moon_texture.rgb * scat;
  }

  color += clamp(auro * (aur.rgb*2. *themie)  * scatatt * refatt,0.,1.);

  if(tonemap!=12.)
    raleigh = mix(raleigh*refatt,raleigh*refatt*scat,RE );
  else
    raleigh *= refatt  *  (scat);

  if(tonemap==1. || tonemap==9.)
    color += pow(raleigh *ralatt, vec3(rgam));
  else
    color += raleigh *ralatt;

  if(tonemap==2. || tonemap==9.)
    color += pow(themie, vec3(gam)) ;
  else
    color += themie;

  color += pow(star, vec3(2.2));
  color += pow(met, vec3(2.2));

  vec3 firescatter = ((vec3(1.0-scat)))*themie;
  firescatter *= (fireworkCol * MI2);
  color += mix(firescatter,color,0.5) *scatatt;
  color += fire;

  vec3 aur_col = bg1 *(1.-aur.a) + (aur.rgb*2.) * (scat);
  aur_col = pow(aur_col, vec3(2.2));
  aur_col *= scatatt * refatt;
  color += aur_col;

#if ENABLE_RAIN
  if(rainvol>0.)
  {
    float rainangle = winddirx;
    vec2 st =  (uv * vec2(.5 +(uv0.y +2.) *.5, .02) + vec2(t*.5 + uv0.y *rainangle, t*0.1));
    float f = texture(iChannel0, st, -100.0).y * texture(iChannel0, st*.773, -100.0).x * 1.55;
    float rain = clamp(rainvol -.15, 0.0, 1.0);
    f = clamp( pow( abs(f), 15.0) * 1.0 * (rain*rain*5.0), 0.0, (uv0.y +.1)*.6);
    color = mix(color, vec3(0.15, .15, .15), f);
  }
#endif
  color += rainbow(camera_vector.xy,light_dir_original) *refatt * clamp(min(RI1/RI0,MI1/MI0)*ralatt,0.,1.) * clamp(scat+0.1,0.,1.);

  if(snowvol>0.)
  {
    vec2 uv2 = (gl_FragCoord.xy-.5*iResolution)/iResolution.y;

    vec3 acc = vec3(0,0,0);
    for (float i = 0.; i < LAYERS; i++)
      acc += SnowSingleLayer(uv2,i); 

    color += acc.rgb;
  }

#if ENABLE_LENS_FLARE
  if(lensflare>0.)
  {
    vec2 mouse = iMouse.xy/iResolution - 0.5;
    mouse.x *= iResolution.x/iResolution.y;
    if(lensflaretype==1.)
      lensf= vec3(1.4,1.2,1.0)*lensFlare(camera_vector.xy, normalize(vec3(uvMouse.x-(0.5*AR), uvMouse.y-Dsfact, (fov/-2.0))).xy,lensflaretype);
    else
      lensf= vec3(1.4,1.2,1.0)*lensFlare(uvl, mouse,lensflaretype);
    lensf = clamp(lensf*scat*lensb,0.,1.);
    lensf=pow(lensf,vec3(lensgam));
    color += lensf*ralatt;
  }
#endif

  color =pow(color, vec3(1./2.2));

#if ENABLE_SEA
#define AA
#ifdef AA
  int aa1a = int(aa1);
  int aa2a = int(aa2);
  float aadiff = abs(aa2-aa1)+1.;
  float aapow = pow(aadiff,2.);
  vec3 seacolor = vec3(0.0);
  for(int i = aa1a; i <= aa2a; i++)
    for(int j = aa1a; j <= aa2a; j++)
    {
      vec2 uv = gl_FragCoord.xy+vec2(i,j)/aadiff;
      seacolor += getPixel(uv, t,color,sea_light_dir,spec1,moon_light_dir2,mLight,scat);
    }
  seacolor /= aapow;
#else
  vec3 seacolor = getPixel(gl_FragCoord.xy, t,color,sea_light_dir,spec1,moon_light_dir2,mLight,scat);
#endif

  if (tonemap==3.)
    color = seacolor;
  else
  {
    float L2 =  camera_position.y / camera_vector.y;
    if(L2 < 1.)
      color = mix(color,seacolor,pow(smoothstep(0.0,-0.05,-camera_vector.y), 0.5));
  }
#endif

  if (scatonly==1.)
    color = vec3(scat);
  if(scatonly==2.)
    color = vec3(1.-scat);
  if(scatonly==3.)
    color = vec3(0.5-scat);
  if(scatonly==4.)
    color = (vec3(1.-(scat))) * pow(vec3(0.0,1.,1.5)*(auroraINT*0.1)*scatatt,vec3(2.2));
  if(scatonly==5.)
    color = themie;
  if(scatonly==6.)
    color = 1.-themie;
  if(scatonly==7.)
    color = vec3(scat)*themie;
  if(scatonly==8.)
    color = vec3(1.-scat)*themie;
  if(scatonly==9.)
    color = vec3(scat)*(1.-themie);
  if(scatonly==10.)
    color = vec3(1.-scat)*(1.-themie);
  if(scatonly==11.)
  {
    vec3 acolor2 = clamp(((vec3(1.0-scat)))*(themie-0.01),0.,1.);
    acolor2 *=  aur.rgb*(20.);
    color = acolor2* scatatt;
  }
  if(scatonly==12.)
  {
    color = vec3(scat)*themie;
    color += raleigh;
  }
  if(scatonly==13.)
  {
    color = vec3(scat)+themie;
    color += raleigh;
  }
  if(scatonly==14.)
  {
    color = vec3(scat);
    color += raleigh;
  }
  if(scatonly==15.)
  {
    color = themie;
    color += raleigh;
  }
  if(scatonly==16.)
    color = raleigh;
  if(scatonly==17.)
    color = vec3(scat)*raleigh;
  if(scatonly==18.)
    color = lensf;
  if(scatonly==19.)
    color = auro;
  if(scatonly==20.)
    color = auro * ((aur.rgb) * themie) * scatatt * refatt;
  if(scatonly==21.)
    color = auro * ((aur.rgb)) * scatatt * refatt;
  if(scatonly==22.)
    color = aur.rgb;
  if(scatonly==23.)
    color = aur_col;
  if(scatonly==24.)
  {
    color = aur_col;
    color += auro * ((aur.rgb) * themie) * scatatt * refatt;
  }

  vec3 weights_ = vec3(0.2125, 0.7154, 0.0721);
  float luminance_ = dot(color, weights_);
  color = mix(vec3(luminance_), color, sat);

  float average = (color.r + color.g + color.b) / 3.;
  float mx = max(color.r, max(color.g, color.b));
  float vibrance = (mx - average) * vib;
  color = clamp(color - (mx - color) * vibrance,0.,1.);

  color = shiftHue(color, hue);
  gl_FragColor = vec4(color, 1.);
}
