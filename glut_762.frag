#version 130

// glut_762_common.glsl
vec2[6] vID = vec2[6](vec2(-.5, -2./6.)/vec2(.5, 1), vec2(-.5,  2./6.)/vec2(.5, 1), vec2(0,  2./3.)/vec2(.5, 1), 
                      vec2( .5,  2./6.)/vec2(.5, 1), vec2( .5, -2./6.)/vec2(.5, 1), vec2(0, -2./3.)/vec2(.5, 1));

mat2
rot2 (float a)
{
  float c = cos(a), s = sin(a);
  return mat2(c, -s, s, c);
}

float
hash21 (vec2 p)
{
  return fract(sin(mod(dot(p, vec2(27.619, 57.583)), 6.2831589)) * 43758.5453);
}

float
distLine (vec2 a, vec2 b)
{
  b = a - b;
  float h = clamp(dot(a, b) / dot(b, b), 0., 1.);
  return length(a - b * h);
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
    
}

float
sBox (vec2 p, vec2 b)
{
  vec2 d = abs(p) - b;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.));
}

float
lBoxHV (vec2 p, vec2 a, vec2 b, float w)
{
  vec2 l = abs(b - a);
  p -= vec2(mix(a.x, b.x, .5), mix(a.y, b.y, .5));

  vec2 d = abs(p) - (l + w) / 2.;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.));
}

float
lBox (vec2 p, vec2 a, vec2 b, float ew)
{
  float ang = atan(b.y - a.y, b.x - a.x);
  p = rot2(ang) * (p - mix(a, b, .5));

  vec2 l = vec2(length(b - a), ew);
  return sBox(p, (l + ew) / 2.);
}

#define NV2 4
float
sdPoly4 (vec2 p, vec2[NV2] v)
{
  const int num = v.length();
  float d = dot(p - v[0], p - v[0]);
  float s = 1.0;
  for (int i = 0, j = num - 1; i < num; j = i, i++)
  {
    vec2 e = v[j] - v[i];
    vec2 w = p - v[i];
    vec2 b = w - e * clamp(dot(w, e) / dot(e, e), 0., 1.);
    d = min( d, dot(b,b) );

    bvec3 cond = bvec3(p.y >= v[i].y, p.y < v[j].y, e.x * w.y > e.y * w.x);
    if( all(cond) || all(not(cond)) )
      s*=-1.0;  
  }

  return s*sqrt(d);
}

float
sdHexagon (vec2 p, float r)
{
  const vec3 k = vec3(-.8660254, .5, .57735);

  p = abs(p);
  p -= 2. * min(dot(k.xy, p), 0.) * k.xy;
    
  return length(p - vec2(clamp(p.x, -k.z * r, k.z * r), r)) * sign(p.y - r);
}

float
line (vec2 p, vec2 a, vec2 b)
{
  return ((b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x));
}

float
n2D (vec2 p)
{
  const vec2 s = vec2(1, 113);

  vec2 ip = floor(p);
  p -= ip;
  vec4 h = vec4(0., s.x, s.y, s.x + s.y) + dot(ip, s);

  p = p * p * (3. - 2. * p);

  h = fract(sin(mod(h, 6.2831589)) * 43758.5453);
  h.xy = mix(h.xy, h.zw, p.y);

  return mix(h.x, h.y, p.x);
}

float
fbm (vec2 p)
{
  return n2D(p) * .533 + n2D(p * 2.) * .267 + n2D(p * 4.) * .133 + n2D(p * 8.) * .067;
}

vec3
pencil (vec3 col, vec2 p)
{
  vec2 q = p * 4.;
  const vec2 sc = vec2(1, 12);
  q += (vec2(n2D(q * 4.), n2D(q * 4. + 7.3)) - .5) * .03;
  q *= rot2(-3.14159 / 2.5);

  col = min(col, 1.);

  float gr = (dot(col, vec3(.299, .587, .114)));

  float ns = (n2D(q * sc) * .66 + n2D(q * 2. * sc) * .34);

  q *= rot2(3.14159 / 2.);
  float ns2 = (n2D(q * sc) * .66 + n2D(q * 2. * sc) * .34);
  q *= rot2(-3.14159 / 5.);
  float ns3 = (n2D(q * sc) * .66 + n2D(q * 2. * sc) * .34);

  const float contrast = 1.;
  ns = (.5 + (gr - (max(max(ns, ns2), ns3))) * contrast);

  return vec3(clamp(ns, 0., 1.));
}
// glut_762_common.glsl

uniform vec2 iResolution;
uniform float iTime;

//#define SUBDIV_LINES 
//#define GREYSCALE
//#define BLINKING_LIGHTS

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy * .5) / iResolution.y;

  const float gSc = 1.;
  float sf = gSc / iResolution.y;

  float rT = 0.; //sin(-iTime/6.*3.)/12.;

  vec2 p = rot2(rT) * uv * gSc;

  float d = 1e5, ln = 1e5, prevD = 1e5;

  const float ew = .0025;
  const float cR = .3;
  const float cAp = cR * cos(6.2831 / 24.);
  const float sL = cR * acos(6.2831 / 24.) / 2.;

  vec2 hSc = sL * vec2(.5, .8660254);
  vec2[6] svV = vec2[6] (vID[0] * hSc, vID[1] * hSc, vID[2] * hSc, vID[3] * hSc, vID[4] * hSc, vID[5] * hSc);

  vec2 offs = vec2(fbm(p * 16.), fbm(p * 16. + .35));
  vec2 offs2 = vec2(fbm(p * 1. + iTime / 4.), fbm(p * 1. + .5 + iTime / 4.));
  const float oFct = .007;
  const float oFct2 = .05;
  p -= (offs - .5) * oFct;
  p -= (offs2 - .5) * oFct2;

  float lnL = -cR - sL * .75;
  float a0 = 6.2831 / 24.;
  float dA = 6.2831 / 12.;
  float inR = cR - sL / 2. * .8660254;

  float gHex = 1e5, gD = 1e5, qLn = 1e5, dSh = 1e5;

  float zBuf = 0.;

  vec3 lp = vec3(-.75, 3, -1.5);
  vec3 ld = normalize(lp - vec3(uv, 0));
  ld.xy = rot2(rT) * ld.xy;

  vec3 col = vec3(.95, .975, 1);

  vec2 q;

  float dim = 9.;
  q = p;
  q = abs(mod(q, 1. / dim) - .5 / dim);
  float ln3 = abs(max(q.x, q.y) - .5 / dim);
  col = mix(col, vec3(.35, .65, 1), (1. - smoothstep(0., sf * 2., ln3)) * .8);
  dim *= 2.;
  q = p;
  q = abs(mod(q, 1. / dim) - .5 / dim);
  ln3 = max(abs(max(q.x, q.y) - .5 / dim), -ln3);
  col = mix(col, vec3(.35, .65, 1), (1. - smoothstep(0., sf * 2., ln3)) * .65);

  col *= clamp(dot(normalize(vec3(p, -1.5)), ld), 0., 1.) * .2 + .8;
  col *= fbm(p * 8.) * .1 + .9;

  vec3[3] n = vec3[3] (vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, -1));

  for (int i = 0; i < 12; i++)
  {
    q = p;

    vec2 a, b, nA;
    a = rot2(a0 + dA * float(i)) * vec2(0, inR);
    b = rot2(a0 + dA * float(i + 3)) * vec2(0, inR);
    nA = normalize(b - a) * sL * 1.6;
    ln = min(ln, lBox(q, a - nA, b + nA, 0.));

    vec2 hCtr = rot2(6.2831 / 12. * float(11 - i)) * vec2(0, cR);
    q -= hCtr;
    q = rot2(-6.2831 / 12. * float(11 - i)) * q;

    float dH = sdHexagon(q, sL / 2.);
    d = dH;

    vec2 qSh = p + ld.xy * .03;
    qSh -= hCtr;
    qSh = rot2(-6.2831 / 12. * float(11 - i)) * qSh;
    dSh = sdHexagon(qSh, sL / 2.);

    gHex = min(gHex, d);

    ln = min(ln, abs(length(q) - sL / 2. / .8660254));

    d = max(d, -(prevD - ew / 3.));

    prevD = d;

    col = mix(col, mix(vec3(0), col, zBuf), (1. - smoothstep(0., sf * 5., dSh - ew / 2.)) * .35);
    zBuf = mix(zBuf, 1., (1. - smoothstep(0., sf * 5., dSh - ew / 2.)));

    gD = min(gD, min(min(ln, d), dSh));

    col = mix(col, vec3(0), (1. - smoothstep(0., sf * 6., d - ew / 2.)) * .35);
    col = mix(col, vec3(0), (1. - smoothstep(0., sf, d - ew / 2.)) * .8);

    for (int j = 0; j < 6; j += 2)
    {
      vec2[4] v = vec2[4] (svV[(j + 1) % 6].yx, svV[(j + 2) % 6].yx, svV[(j + 3) % 6].yx, vec2(0));
      vec2 qCtr = (v[0] + v[1] + v[2] + v[3]) / 4.;
      vec2 qID = hCtr + qCtr;

      float quad = max(sdPoly4(q, v), d);

      gD = min(gD, quad);

      vec3 rnbCol = .6 + .4 * cos(6.2831 * float(i) / 12. + vec3(0, 1, 2) * 1.5 + 3.14159 / 6.);

#ifdef BLINKING_LIGHTS
      float rndI = hash21(hCtr); rndI = cos(rndI*6.2831 + iTime/1.25 + .5);
      rndI = smoothstep(.9, .95, sin(rndI*6.2831 + iTime*3.)*.5 + .5);

      vec3 rCol = vec3(.55) + float(i%3)/12.;//vec3(hash21(hCtr + .6)*.35 + .5);
      rCol = mix(rCol, dot(rCol, vec3(.299, .587, .114))*vec3(4, 1, .5), rndI);
      rCol = mix(rCol, rCol.xzy, sin(hash21(hCtr + .44)*6.2831 + iTime)*.35 + .35);
#else
      vec3 rCol = rnbCol;
#endif

      vec3 sn = n[j / 2];
      sn.xy = rot2(6.2831 / 12. * float(11 - i)) * sn.xy;

      float sh = clamp(.35 - quad / .03, 0., 1.) * .3 + .7;

      float dif = max(dot(ld, sn), 0.);

      rCol = rCol * (dif + .5) * sh;

      col = mix(col, rCol * .5, (1. - smoothstep(0., sf, quad)));
      col = mix(col, vec3(rCol), (1. - smoothstep(0., sf, quad + ew)));

#ifdef SUBDIV_LINES
      qLn = lBox(q, mix(v[0], v[1], .5), mix(v[2], v[3], .5), .0);
      qLn = min(qLn, lBox(q, mix(v[1], v[2], .5), mix(v[3], v[0], .5), .0));
      qLn = max(qLn, (prevD - ew/3.));

      vec3 svCol = col;
      col = mix(col, col*1.35, (1. - smoothstep(0., sf*3., qLn - .003)));
      col = mix(col, svCol*.65, (1. - smoothstep(0., sf*2., qLn - .001)));
#endif
    }
  }

  float lAlpha = mix(.25, .125, 1. - smoothstep(0., sf * 2., gHex));
  col = mix(col, vec3(0), (1. - smoothstep(0., sf * 2., ln - .001 * fbm(p * 32. + .5))) * lAlpha);
  col = mix(col, vec3(.35, .65, 1) / 4., (1. - smoothstep(0., sf * 2., max(gHex, ln3))) * .25);

  q = p * 8.;
  vec3 colP = pencil(col, q * iResolution.y / 450.);
#ifdef GREYSCALE
  col = mix(dot(col, vec3(.299, .587, .114))*vec3(1), colP, .6);
#else
  col = mix(col, 1. - exp(-(col * 2.) * (colP + .25)), .85);
#endif

  vec2 pp = q;
  vec3 rn3 = vec3(hash21(pp), hash21(pp + 2.37), hash21(pp + 4.83));
  vec3 pg = .8 + (rn3.xyz * .35 + rn3.xxx * .65) * .4;
  col *= min(pg, 1.);

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
