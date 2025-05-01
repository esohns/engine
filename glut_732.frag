#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

#define PSEUDO3D
//#define PURE_OULINES
//#define SPH_ABB

#define Res  iResolution.xy
#define Res0 vec2(textureSize(iChannel0,0))
#define Res1 vec2(textureSize(iChannel1,0))
#define Res2 vec2(textureSize(iChannel2,0))
#define Res3 vec2(textureSize(iChannel3,0))

#define PI2 6.28318530718
#define N(v) (v.yx*vec2(1,-1))
#define CS(x) sin(x+vec2(PI2/4.,0))
#define ROT2(x) mat2(CS(x),N(CS(x)))

vec2
tr_i (vec2 p)
{
  return (p * vec2(1, .5 * sqrt(3.)) + vec2(.5 * p.y, 0));
}

vec2
tr (vec2 p)
{
  return (p - vec2(p.y / sqrt(3.), 0)) / vec2(1, .5 * sqrt(3.));
}

void
getTri (vec2 p, inout vec2 p1, inout vec2 p2, inout vec2 p3, float size)
{
  vec2 pt = tr(p) / size;
  vec2 pf = floor(pt);
  vec2 pc = ceil(pt);
  p1 = vec2(pf.x, pc.y);
  p2 = vec2(pc.x, pf.y);
  p3 = pc;
  if (dot(pt - pf, vec2(1)) < 1.)
    p3 = pf;
  p1 = tr_i(p1) * size;
  p2 = tr_i(p2) * size;
  p3 = tr_i(p3) * size;
}

float
tri01 (float x)
{
  return abs(fract(x) - .5) * 2.;
}

vec4
getRand (vec2 p)
{
  return texelFetch(iChannel0, ivec2(mod(p + .1, 256.0)) & 255, 0);
}

vec4
getRandS (vec2 p)
{
  return texture(iChannel0, p);
}

#define PI 3.14159265

float
dist (vec2 p, inout vec2 p1, inout vec2 p2, inout vec2 p3, float size)
{
  getTri(p, p1, p2, p3, size);
  vec4 rnd = getRand(p1 + p2 + p3);
  float r = rnd.z;
  if (fract(r * 2.) > .3333)
  {
    vec2 d = p3;
    p3 = p2;
    p2 = p1;
    p1 = d;
  }
  if (fract(r * 2.) > .6666)
  {
    vec2 d = p3;
    p3 = p2;
    p2 = p1;
    p1 = d;
  }
  float d = 10000.;
  d = min(d, length(p - p1));
  d = min(d, length(p - p2));
  return d;
}

float
ndist (vec2 a, vec2 b)
{
  return length(cross(vec3(a, 0), normalize(vec3(b, 0))));
}

float
schraff (vec2 dir, float period, float w, vec2 coord, float gscale)
{
  float x = dot(dir, coord);
  float pat = tri01(x / period) * period * .5;
  pat = smoothstep(w * .5 - 1.2, w * .5 + 1.2, pat / gscale);
  return pat;
}

vec3
getEnv (vec3 dir)
{
  return texture(iChannel1, dir).xyz;
}

void
main ()
{
  vec2 fragCoord = gl_FragCoord.xy;
  
  vec2 fc0 = fragCoord;

  float gscale = .6;
  if (iMouse.x > 0.)
    gscale = 2. * exp(-iMouse.x / Res.x * 2.);
    
#ifdef SPH_ABB
    fragCoord -= Res*.5;
    float dist2=dot(fragCoord,fragCoord);
    fragCoord = fragCoord+fragCoord*dist2*dist2/Res.x/Res.x/Res.x/Res.x*3.;
    fragCoord += Res*.5;
#endif
    
  float mspeed = .5;
  fragCoord = (fragCoord - Res * .5) * gscale - Res * .5;
  vec2 cs = CS(sin(iTime*mspeed*.13));
  fragCoord = mat2(cs, N(cs)) * (fragCoord - Res * .5);
    
  fragCoord += 100. * sin(vec2(0, 1.6) + .33 * iTime * mspeed);
  fragCoord += 70. * iTime * mspeed;
    
  //gl_FragColor = vec4(dist(fragCoord,period,60.)/(.5*period));
  float d = 10000.;
  //d=min(d,dist(fragCoord,.007*iResolution.x,.07*iResolution.x));
  float sc = 170. / sqrt(600. / iResolution.x);

  float lineWidth = 1.5 / gscale * sc / 170.;

  vec2 p1, p2, p3;
  float r, sc2;
  float divNum = float(6 * 2 * 2);
  sc2 = 1.;
  float phase;
  float phase2;
  for (int i = 0; i < 3; i++)
  {
    phase = ((int(divNum * sc2) & 1) == 1) ? .5 : .0;
    phase2 = ((int(divNum * sc2) & 2) == 2) ? .5 : .0;
    d = dist(fragCoord, p1, p2, p3, sc * sc2);
    r = getRand(p1 + p2 + p3).x;
    if (r > .8 - float(i) * .5)
      break;
    sc2 /= 2.;
  }
  float d0 = d;
  d = tri01((d - sc * sc2 * .5) / sc * divNum + phase) * .5 * sc / divNum;
  float d2 = tri01((d0 - sc * sc2 * .5) / sc * divNum * .5 + phase2 * 1. + .5) * .5 * .5 * sc / divNum;
    
  float dg = 10000.;
  dg = min(dg, ndist(fragCoord - p1, normalize(p2 - p1)));
  dg = min(dg, ndist(fragCoord - p2, normalize(p3 - p2)));
  dg = min(dg, ndist(fragCoord - p3, normalize(p1 - p3)));
    
  gl_FragColor.xyz = vec3(1);
    
#ifndef PURE_OULINES
  vec2 sdir = CS(7.);
  float pat1 = schraff(sdir, sc / divNum * .75, lineWidth * .05, fragCoord, gscale);
  float pat2 = schraff(N(sdir), sc / divNum * .75, lineWidth * .05, fragCoord, gscale);
  // force to non-cubemap version for brushed metal, because it aliases too much
  vec3 n1 = vec3(.2, 1, 1) * (texture(iChannel0, fragCoord * .008 * vec2(.05, 1)).xyz - vec3(.5, .5, -1.));
  n1 = normalize(n1);
  n1.xy = n1.xy * mat2(cs, N(cs));
  vec2 scr = (fc0 - .5 * Res) / Res.x * 3.;
  vec3 c1 = .5 + .5 * getEnv(reflect(vec3(scr, -1.), n1).xzy);
  c1 =
      -.5 * (texture(iChannel0, fragCoord * .008 * vec2(.05, 1)).xyz - .25)
      - 1. * (texture(iChannel0, fragCoord * .016 * vec2(.05, 1)).xyz - .25);
  c1 = vec3(0) + (.85 + .3 * c1.xy * ROT2(iTime*2.+3.6*getRandS(fc0/Res/Res0*3.+iTime/Res0.x*1.).x)).x;
  vec3 c2 = vec3(1., .8, .3) * mix(vec3(.3), vec3(1.), pat1 * pat2) * clamp(pow(.6 + .6 * d / (sc / divNum) * 2., 1.), 0., 1.);
  gl_FragColor.xyz *= mix(c2, c1, step(.5, fract((d0 / sc * divNum + .5 + phase) * .5)));
#endif

#ifdef PURE_OULINES
  gl_FragColor.xyz*=smoothstep(lineWidth*.5-1.2,lineWidth*.5+1.2,d/gscale);
#else
#ifndef PSEUDO3D
  gl_FragColor.xyz*=smoothstep(lineWidth*.5-1.2,lineWidth*.5+1.2,d/gscale);
#else
  vec3 n3 = normalize(vec3(vec2(-dFdx(d2), -dFdy(d2)), .5));
  gl_FragColor.xyz = mix(
        (.5 + .5 * getEnv(reflect(vec3(scr, -1.), n3).xzy)) *
        vec3(.7) * dot(n3, normalize(vec3(-1, 1, .7))),
        gl_FragColor.xyz,
        smoothstep(lineWidth * .5 - 1.2, lineWidth * .5 + 1.2, d / gscale)
        );
#endif
#endif

  /*if(iMouse.x==iMouse.z && iMouse.x!=0.0)
  {
      fragColor.xyz*=smoothstep(lineWidth*.5-1.2,lineWidth*.5+1.2,dg/gscale);
  }*/

  if (true)
  {
    vec2 scc = (fc0 - .5 * iResolution.xy) / iResolution.x;
    float vign = 1. - .3 * dot(scc, scc);
    vign *= 1. - .7 * exp(-sin(fc0.x / iResolution.x * 3.1416) * 40.);
    vign *= 1. - .7 * exp(-sin(fc0.y / iResolution.y * 3.1416) * 20.);
    gl_FragColor.xyz *= vign;
  }
  gl_FragColor.w = 1.;
}
