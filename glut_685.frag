#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

//#define NAIVE_HATCH
//#define PAPER_GRAIN
//#define UNTEXTURED
#define SHADOWS
#define HIGHLIGHTS
//#define GRAYSCALE
#define PERTURB_COORDINATES
//#define SHOW_GRID

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

vec2
hash22 (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  uint  n = 1103515245U*((p.x)^(p.y>>3U));
  uvec2 rz = uvec2(n, n*48271U);
  return vec2((rz.xy >> 1) & uvec2(0x7fffffffU))/float(0x7fffffff)*2. - 1.;
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

  p = p*p*p*(p*(p*6. - 15.) + 10.);

  return mix(mix(v.x, v.y, p.x), mix(v.z, v.w, p.x), p.y);
}

float
doHatch (vec2 p, float res)
{
  p *= res/16.;

  float hatch = clamp(sin((p.x - p.y)*3.14159*200.)*2. + .5, 0., 1.);

  float hRnd = hash21(floor(p*6.) + .73);
  if(hRnd>.66) hatch = hRnd;  

  return hatch;
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
}

float
hash21B (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);
  return (float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU) - .5)*.75;
}

vec3
pattern (vec2 p, float sc)
{
  vec2 ip = floor(p*sc) + .5;
  p -= ip/sc;

  vec3 e = vec3(-1, 0, 1);

  float h11 = hash21B(ip);

  float h10 = hash21B(ip + e.xy);
  float h01 = hash21B(ip + e.yz);
  float h12 = hash21B(ip + e.zy);
  float h21 = hash21B(ip + e.yx);
    
  float h00 = hash21B(ip + e.xz);
  float h02 = hash21B(ip + e.zz);
  float h22 = hash21B(ip + e.zx);
  float h20 = hash21B(ip + e.xx);

  vec2[4] ctr;
  vec2[4] l;

  if(mod((ip.x + ip.y), 2.)<.5)
  {
    l[0] = vec2(h01 - h10, h00 - h11) + 1.;
    l[1] = vec2(-h01 + h12, h02 - h11) + 1.;
    l[2] = vec2(-h21 + h12, -h22 + h11) + 1.;
    l[3] = vec2(h21 - h10, -h20 + h11) + 1.;

    ctr[0] = vec2(h01, h11) + l[0]*vec2(-.5, .5);
    ctr[1] = vec2(h01, h11) + l[1]*vec2(.5, .5);
    ctr[2] = vec2(h21, h11) + l[2]*vec2(.5, -.5);
    ctr[3] = vec2(h21, h11) + l[3]*vec2(-.5, -.5); 
  }
  else
  {
    l[0] = vec2(-h00 + h11, h01 - h10) + 1.;
    l[1] = vec2(h02 - h11, h01 - h12) + 1.;
    l[2] = vec2(h22 - h11, -h21 + h12) + 1.;
    l[3] = vec2(-h20 + h11, -h21 + h10) + 1.;

    ctr[0] = vec2(h11, h10) + l[0]*vec2(-.5, .5);
    ctr[1] = vec2(h11, h12) + l[1]*vec2(.5, .5);
    ctr[2] = vec2(h11, h12) + l[2]*vec2(.5, -.5);
    ctr[3] = vec2(h11, h10) + l[3]*vec2(-.5, -.5); 
  }

  l[0] /= sc; l[1] /= sc; l[2] /= sc; l[3] /= sc;

  float d = 1e5;
  vec2 tileID = vec2(0);

  for(int i = 0; i<4; i++)
  {
    float bx = sBoxS(p - ctr[i]/sc, l[i]/2. - .05/sc, .1/sc);
    if(bx<d)
    {
      d = bx;
      tileID = ip + ctr[i];
    }
  }
  return vec3(d, tileID);
}

float
gridField (vec2 p)
{
  p = abs(fract(p) - .5);
  float grid = abs(max(p.x, p.y) - .5) - .008;

  return grid;
}

void
main ()
{
  float iRes = min(iResolution.y, 750.);
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iRes;

  vec2 p = uv - vec2(-1, -.25)*iTime/12.;

  vec2 oP = p;

  float sf = 1.5/iResolution.y;

#ifdef PERTURB_COORDINATES
  p += vec2(n2D3G(p*8.5), n2D3G(p*8.5 + 7.3))*.008;
#endif

  const float sc = 12.;

  vec3 d = pattern(p, sc);
  vec2 e = vec2(.005*8./sc, -.007*8./sc);
  vec3 d2 = pattern(p - e, sc);

  float ba = mix(min(-d.x*4., .1), smoothstep(0., sf*2., -d.x), .1);
  float bb = mix(min(-d2.x*4., .1), smoothstep(0., sf*2., -d2.x), .1);
  float b = max(-bb - -ba, 0.)/length(e);

  vec3 tx = texture(iChannel0, d.yz/sc).xyz; tx *= tx;
  tx = smoothstep(-.1, .7, tx);

  vec3 tx2 = texture(iChannel1, oP).xyz; tx2 *= tx2;
  tx2 = smoothstep(-.1, .6, tx2);
  tx2 = mix(tx2, vec3(1)*dot(tx2, vec3(.299, .587, .114)), .25);

  vec3 col = vec3(.725, .7, .675)*tx2*tx2/3.;

#ifdef UNTEXTURED
  col = vec3(.1);
#endif

#ifdef SHADOWS
  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d2.x - .003))*.9);
#endif

#ifdef UNTEXTURED
  vec2 rnd = hash22(d.yz);
  vec3 lCol = vec3(1, .5 + rnd.y*.5, rnd.x*.4 + .4)*.75 + .15;
#else
  vec3 lCol = (tx2*2.)*tx*1.; 
#endif

#ifdef HIGHLIGHTS
  lCol *= (1. + b*.125);
#endif

  col = mix(col, vec3(0), (1. - smoothstep(0., sf*3., d.x - .002))*.9);
  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d.x - .002)));
  col = mix(col, lCol, (1. - smoothstep(0., sf, d.x + .002)));  

#ifdef SHOW_GRID
  float grid = gridField(p*sc);

  vec3 svC = col;
  col = mix(col, vec3(0), (1. - smoothstep(0., sf*1.5*sc, grid - .05))*.9);
  col = mix(col, mix(svC.zyx*1.5, vec3(1, 1, 4), .5), (1. - smoothstep(0., sf*sc, grid)));
#endif

#ifdef GRAYSCALE
  col = vec3(1)*dot(col, vec3(.289, .597, .114));
#endif

#ifdef NAIVE_HATCH
  float hatch = doHatch(oP, iRes);
  col *= hatch*.5 + .7;
#endif

#ifdef PAPER_GRAIN
  oP = floor(oP*1024.);
  vec3 rn3 = vec3(hash21(oP), hash21(oP + 2.37), hash21(oP + 4.83));
  col *= .9 + .1*rn3.xyz  + .1*rn3.xxx;
#endif

  uv = gl_FragCoord.xy/iResolution.xy;
  col *= min(pow(16.*(1. - uv.x)*(1. - uv.y)*uv.x*uv.y, 1./8.)*1.1, 1.);

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
