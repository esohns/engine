#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAKE_AO
#define STROKE
#define HILIGHT
#define SHADOW
#define TEXTURED
#define HATCH
//#define PAPER_GRAIN
// Settings: Greyscale: 0, Red: 1, Blue: 2, Earth: 3, Pink and grey: 4, 
// Grey and pink: 5, Green and white: 6.
#define PALETTE 0

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 p)
{
  return fract(sin(dot(p, vec2(41, 289)))*45758.5453); 
}

vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(1, 113)));
  p = fract(vec2(262144, 32768)*n); 
  return sin(p*6.2831853 + iTime); 
}

float
n2D (vec2 p)
{
  vec2 i = floor(p);
  p -= i;
  p *= p*(3. - p*2.);

  return dot(mat2(fract(sin(vec4(0, 1, 113, 114) + dot(i, vec2(1, 113)))*43758.5453)) * vec2(1. - p.y, p.y), vec2(1. - p.x, p.x) );
}

float
n2D3G (vec2 p)
{
  vec2 i = floor(p); p -= i;

  vec4 v;
  v.x = dot(hash22(i), p);
  v.y = dot(hash22(i + vec2(1, 0)), p - vec2(1, 0));
  v.z = dot(hash22(i + vec2(0, 1)), p - vec2(0, 1));
  v.w = dot(hash22(i + 1.), p - 1.);

#if 1
  // Quintic interpolation.
  p = p*p*p*(p*(p*6. - 15.) + 10.);
#else
  // Cubic interpolation.
  p = p*p*(3. - 2.*p);
#endif

  return mix(mix(v.x, v.y, p.x), mix(v.z, v.w, p.x), p.y);
}

float
map (vec3 p, float i)
{
  return n2D3G(p.xy*3.)*.66 + n2D3G(p.xy*6.)*.34 + i/10.*1. - .15;
}

vec3
getNormal (vec3 p, float m, float i)
{
  vec2 e = vec2(.001, 0);
  return (vec3(m - map(p - e.xyy, i), m - map(p - e.yxy, i),	0.))/e.x*1.4142;
}

vec4
mapLayer (vec3 p, float i)
{
  vec4 d;
  d.x = map(p, i);
  d.yzw = getNormal(p, d.x, i);

  return d;
}

vec3
getCol (vec2 p, float sh, float fi, float sf)
{
  vec3 col;

#if PALETTE == 0
  // Light attenuation palette.
  col = vec3(1)*(1. - .75/(1. + sh*sh*2.));
#elif PALETTE == 1
  col = pow(min(vec3(1.5, 1, 1)*(sh*.35 + .6), 1.), vec3(1, 3, 16));
  if(fi==0.) col = vec3(.35, .05, .3);
  col = mix(col.xzy, col, sh*.5 + .5);
#elif PALETTE == 2
  col = pow(min(vec3(1.5, 1, 1)*(sh*.35 + .6), 1.), vec3(1, 3, 16));
  if(fi==0.) col = vec3(.6, .2, .07);
  col = mix(col.xzy, col, sh*.5 + .5).zyx;
#elif PALETTE == 3
  if(fi==0.) col = vec3(.25, .52, .75);
  if(fi==1.) col = vec3(.8, .8, .6);
  if(fi==2.) col = vec3(.75, .6, .5);
  if(fi==3.) col = vec3(.6, .58, .5);
  if(fi==4.) col = vec3(.5, .72, .4);
  if(fi==5.) col = vec3(.65, .85, .5);
  else col = min(vec3(.65, .85, .5)*1.25, 1.);
#elif PALETTE == 4
  if(mod(fi, 2.)<.5) col = vec3(.25, .15, .2);
  else col = vec3(1, .15, .4)*.8;
#elif PALETTE == 5
  if(mod(fi, 2.)<.5) col = vec3(1, .15, .4)*.8;
  else col = vec3(.25, .15, .2);
#else
  if(mod(fi, 2.)>.5) col = vec3(.6);
  else col = vec3(.6, 1, .15)*.6;
#endif

#ifdef TEXTURED
  vec3 tx = texture(iChannel0, p + hash21(vec2(sh, fi))).xyz; tx *= tx;
  col = min(col*tx*3., 1.);
#endif

  return col;
}

float
doHatch (vec2 p, float res)
{
#ifdef HATCH
  p *= res/16.;

  float hatch = clamp(sin((p.x - p.y)*3.14159*200.)*2. + .5, 0., 1.);

  float hRnd = hash21(floor(p*6.) + .73);
  if(hRnd>.66)
    hatch = hRnd;

#ifdef TEXTURED
  hatch = hatch*.75 + .5;
#else
  hatch = hatch*.5 + .75;
#endif

  return hatch;
#else
return 1.;
#endif 
}

void
main ()
{
  float res = min(iResolution.y, 700.);
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/res;

  vec2 p = uv;

  float sf = 1./iResolution.y;

  vec3 col = getCol(p, 0., 0., sf);

  float pL = 0.;

  float hatch = doHatch(p, res);

  col *= hatch;

  int lNum = 5;
  float flNum = float(lNum);

  for(int i = 0; i<lNum; i++)
  {
    float fi = float(i);

    hatch = doHatch(p + sin(vec2(41, 289)*(fi + 1.)), res);

    vec4 c = mapLayer(vec3(p, 1.), fi);
    vec4 cSh = mapLayer(vec3(p - vec2(.03, -.03)*((flNum - fi)/flNum*.5 + .5), 1.), fi);

    float sh = (fi + 1.)/(flNum);

    vec3 lCol = getCol(p, sh, fi + 1., sf);

    vec3 ld = normalize(vec3(-1, 1, -.25));
    vec3 n = normalize(vec3(0, 0, -1) + c.yzw);
    float diff = max(dot(ld, n), 0.);
#ifdef TEXTURED
    diff *= 2.5;
#else
    diff *= 2.;
#endif

    vec3 eCol = lCol*(diff + 1.);

    float sfL = sf*length(c.yzx)*2.;
    float sfLSh = sf*length(cSh.yzx)*6.;

#ifdef SHADOW
#ifdef TEXTURED
    const float shF = .5;
#else
    const float shF = .35;
#endif
    col = mix(col, vec3(0), (1. - smoothstep(0., sfLSh, max(cSh.x, pL)))*shF);
#endif

#ifdef FAKE_AO
    col = mix(col, vec3(0), (1. - smoothstep(0., sfL*3., c.x))*.25);
#endif

#ifdef STROKE
    col = mix(col, vec3(0), (1. - smoothstep(0., sfL, c.x))*.85);
#endif

#ifdef HILIGHT
    col = mix(col, eCol*hatch, (1. - smoothstep(0., sfL, c.x + length(c.yzx)*.003)));
    col = mix(col, lCol*hatch, (1. - smoothstep(0., sfL, c.x + length(c.yzx)*.006)));
#else
    col = mix(col, lCol*hatch, (1. - smoothstep(0., sfL, c.x + length(c.yzx)*.0025)));
#endif

    pL = c.x;
  }

#ifdef TEXTURED
#if PALETTE == 0
  col *= mix(vec3(1.8, 1, .7).zyx, vec3(1.8, 1, .7).xzy, n2D(p*2.));
#endif
#endif

#ifdef PAPER_GRAIN
  vec3 rn3 = vec3(n2D((uv*iResolution.y/1. + 1.7)) - n2D(vec2(uv*iResolution.y/1. + 3.4)));
  col *= .93 + .07*rn3.xyz  + .07*dot(rn3, vec3(.299, .587, .114));
#endif

  uv = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .0625);

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
