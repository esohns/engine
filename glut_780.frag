#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

//#define SHOW_GRID

mat2
rot2 (float a)
{
  float c = cos(a), s = sin(a);
  return mat2(c, -s, s, c);
}

float
hash21 (vec2 p)
{
  return fract(sin(dot(p, vec2(27.619, 57.583)))*43758.5453);
}
 
vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(27, 57)));
  return fract(vec2(262144, 32768)*n)*2. - 1.; 
  //p = fract(vec2(262144, 32768)*n);
  //return sin(p*6.2831853 + iTime); 
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
  p = p*p*p*(p*(p*6. - 15.) + 10.);
#else
  p = p*p*(3. - 2.*p);
#endif

  return mix(mix(v.x, v.y, p.x), mix(v.z, v.w, p.x), p.y);
  //return v.x + p.x*(v.y - v.x) + p.y*(v.z - v.x) + p.x*p.y*(v.x - v.y - v.z + v.w);
}

float
distT (vec2 p)
{
  return length(p);

  //p = abs(p);
  //return max(max(p.x, p.y), (p.x + p.y)*.7071);
}

float id = 0.01;

vec3
distFieldT (vec2 q)
{
  float d = 1e5;

  vec2 p = q - .5;

  vec2 ip = floor(p);
  p -= ip + .5;

  const float sqrt2 = sqrt(2.);

  float s = 1./(1. + sqrt2);

  float dia = (abs(p.x) + abs(p.y))/sqrt2 - s/2.;

  id = -1.;

  if(dia<.0)
  {
    float rnd = hash21(ip);
    if(rnd<.5) p = rot2(3.14159/2.)*p;

    p.y = abs(p.y);
    d = min(d, distT(p - vec2(0, s/sqrt2)) - s/2.);

    float ch = mod(ip.x + ip.y, 2.);
    if(ch<.5) d = -d;

    if(rnd<.5) d = -d;

    ip += .5;
  }
  else
  {
    p = q;
    vec2 ip = floor(p);
    p -= ip + .5;

    float rnd = hash21(ip + vec2(.1, .3));
    if(rnd<.5) p = rot2(3.14159/4.)*p;

    d = min(d, distT(p - vec2(-.5, s/2.)) - s/2.);
    d = min(d, distT(p - vec2(s/2., .5)) - s/2.);
    d = min(d, distT(p - vec2(.5, -s/2.)) - s/2.);
    d = min(d, distT(p - vec2(-s/2., -.5)) - s/2.);

    float ch = mod(ip.x + ip.y, 2.);
    if(ch>.5)
    {
      d = -d;
      id = -id;
    }

    if(rnd<.5)
    {
      d = -d;
      id = -id;
    }
  }
    
  return vec3(d, ip);
}

float
distS (vec2 p)
{
  return length(p);
  //p = abs(p);
  //return max(max(p.x, p.y), (p.x + p.y)*.7071);

  //return max(p.x, p.y);
  //return max(p.x*.8660254 + p.y*.5, p.y);
  //return (p.x + p.y)*.7071;
}

vec3
distFieldS (vec2 p)
{
  vec2 ip = floor(p);
  p -= ip + .5;

  float d = distS(p - (hash22(ip) - .5)*.05) - .115;

  return vec3(d, ip);
}  

#ifdef SHOW_GRID
float
gridField (vec2 q)
{
  vec2 p = q - .5;
  vec2 ip = floor(p);
  p -= ip + .5;

  float dia = abs(p.x) + abs(p.y) - (1. - sqrt(2.)/2.);

  float d = 1e5;

  if(dia<.0)
  {
      d = dia;
      ip += .5;
  }
  else
  {
    p = q;
    vec2 ip = floor(p);
    p -= ip + .5; // Equivalent to: fract(p) - .5;

    float oct = max((abs(p.x) + abs(p.y))/sqrt(2.), max(abs(p.x), abs(p.y))) - .5;
    d = oct;
  }

  d = abs(d) - .01;

  return d;
}
#endif

void
main ()
{
  float iRes = min(iResolution.y, 800.);
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  float gSc = 4.5;

  vec2 p = uv*gSc - vec2(cos(iTime/3.)*1.5, -.5*iTime);
  vec2 oP = p;

  float sf = 1./iResolution.y*gSc;

  p += vec2(n2D3G(p*4.), n2D3G(p*4. + 7.3))*.025;

  float vsd = .08;
  vec2 vs = rot2(6.2831/8.*3.5)*vec2(0, 1);

  vec3 d = distFieldT(p);
  float svID = id;

  vec3 d3d = distFieldT(p - vs*vsd);
  vec3 dsh = distFieldT(p - vs*vsd*2.5);
  vec3 dh = distFieldT(p + vs*vsd);

  float vsd2 = .06;
  vec3 d2 = distFieldS(p + vs/2.*vsd2);
  vec3 d3d2 = distFieldS(p - vs/2.*vsd2);
  vec3 dsh2 = distFieldS(p - vs*vsd2*2.5);
  vec3 dh2 = distFieldS(p + vs*vsd2);

  float rnd = hash21(d2.yz + .07);
  float rnd2 = hash21(d2.yz + .34);
  float rndS = hash21(dsh2.yz + .07);

  vec3 bCol = vec3(.26, .52, 1);
  vec3 tCol = vec3(.9, .7, .55);
  vec3 sCol = vec3(1.2, .6, .4);

  vec3 col = bCol;

  vec2 pTx = rot2(-6.2831/8.*3.5)*p;
  vec2 pTx2 = rot2(-6.2831/8.*3.5 - 3.14159/4.)*p;

  float lnPat = clamp(cos(pTx.x*6.2831*22.)*2. + 1.5, 0., 1.);
  float lnPat2 = clamp(cos(pTx2.x*6.2831*22.)*2. + 1.5, 0., 1.);

  vec3 tx = texture(iChannel0, pTx/gSc).xyz; tx *= tx;
  tx = min(smoothstep(-.1, .3, tx), 1.);
  tx *= lnPat*.5 + .7;

  vec3 tx2 = texture(iChannel0, pTx2/gSc).xyz; tx2 *= tx2;
  tx2 = min(smoothstep(-.1, .3, tx2), 1.);
  tx2 *= lnPat2*.3 + .8;

  col *= tx2;

  float sFunc = (n2D3G(p/gSc*3. - iTime/4.)*.67 + n2D3G(p/gSc*6. - iTime/2.)*.33)*.16;
  d.x += sFunc;
  d3d.x += sFunc;
  dsh.x += sFunc;
  d2.x += sFunc/1.5;
  d3d2.x += sFunc/1.5;
  dsh2.x += sFunc/1.5;     

  col = mix(col, vec3(0), (1. - smoothstep(0., sf*4., d3d.x - vsd/2. - .07))*.5);
  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d3d.x - vsd/2. - .035)));
  col = mix(col, sCol*tx, (1. - smoothstep(0., sf, d3d.x - vsd/2.)));

  col = mix(col, vec3(0), (1. - smoothstep(0., sf*4., dsh.x - vsd*2.5/2.))*.5);

  if(rnd<.5)
  {
    col = mix(col, vec3(0), (1. - smoothstep(0., sf, d3d2.x - vsd2/2. - .035)));
    col = mix(col, sCol*tx, (1. - smoothstep(0., sf, d3d2.x - vsd2/2.)));

    col = mix(col, vec3(0), (1. - smoothstep(0., sf*4., dsh2.x - vsd2*2.5/2.))*.5);
  }

  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d.x - .035))*.98);
  col = mix(col, tCol*tx2, (1. - smoothstep(0., sf, d.x)));

  col = mix(col, min(col*1.5, 1.), (1. - smoothstep(0., sf*4., max(-d3d.x, d.x)))*.9);
  col = mix(col, vec3(1.5)*tx2, (1. - smoothstep(0., sf*4., max(-d3d.x, d.x + vsd*(450./iRes))))*.9);

  if(rnd<.5 && rndS<.5)
  {
    if(svID>0.)
    {
      col = mix(col, vec3(0), (1. - smoothstep(0., sf, d3d2.x - vsd2/2. - .035)));
      col = mix(col, sCol*tx, (1. - smoothstep(0., sf, d3d2.x - vsd2/2.)));

      col = mix(col, vec3(0), (1. - smoothstep(0., sf*4., dsh2.x - vsd2*2.5/2.))*.5);
    }

    vec3 dCol = .525 + .425*cos(rnd2*6.2831 + vec3(11, 22, 3)/3.);

    col = mix(col, vec3(0), (1. - smoothstep(0., sf, d2.x - .035))*.98);
    col = mix(col, dCol*tx2, (1. - smoothstep(0., sf, d2.x)));

    dCol = .525 + .425*cos(rnd2*6.2831 + vec3(11, 22, 3)/3. + .25);
    float sz = (.05 + rnd2*.035);
    col = mix(col, vec3(0), (1. - smoothstep(0., sf, d2.x + sz - .035))*.98);
    col = mix(col, dCol*tx2, (1. - smoothstep(0., sf, d2.x + sz)));

    col = mix(col, min(col*1.5, 1.), (1. - smoothstep(0., sf*4., max(-d3d2.x, d2.x)))*.9);
    col = mix(col, min(col*3., 1.), (1. - smoothstep(0., sf*4., max(-d3d2.x, d2.x + vsd2*(450./iRes))))*.9);
  }

#ifdef SHOW_GRID
  float grid = gridField(p);
  col = mix(col, col*5., (1. - smoothstep(0., sf*2., grid - .035)));
  col = mix(col, vec3(0), (1. - smoothstep(0., sf, grid - .0275)));
  col = mix(col, vec3(1.25)*tx2, (1. - smoothstep(0., sf, grid + .005)));
#endif

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
