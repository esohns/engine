#version 330

// glut_993_common.glsl
mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }
vec2 skewXY (vec2 p, vec2 s) { return mat2(1, -s.yx, 1)*p; }
vec2 unskewXY (vec2 p, vec2 s) { return inverse(mat2(1, -s.yx, 1))*p; }

const float scale = 1./4.;
float gTri;

vec4
getTriVerts (vec2 p, inout mat3x2 vID, inout mat3x2 v)
{
  const vec2 rect = (vec2(1./.8660254, 1))*scale;

  const vec2 sk = vec2(rect.x*.5, 0)/scale;

  p = skewXY(p, sk);

  vec2 id = floor(p/rect) + .5; 

  p -= id*rect; 

  gTri = dot(p, 1./rect)<0.? 1. : -1.;

  p = unskewXY(p, sk);

  if(gTri<0.)
    vID = mat3x2(vec2(-1.5, 1.5), vec2(1.5, -1.5), vec2(1.5));
  else
    vID = mat3x2(vec2(1.5, -1.5), vec2(-1.5, 1.5), vec2(-1.5));
    
  for(int i = 0; i<3; i++)
    v[i] = unskewXY(vID[i]*rect/3., sk);

  vec2 ctr = v[2]/3.;
  p -= ctr;
  v[0] -= ctr; v[1] -= ctr; v[2] -= ctr;

  vec2 ctrID = vID[2]/3.;
  id = id*3. + ctrID;

  vID[0] -= ctrID; vID[1] -= ctrID; vID[2] -= ctrID;

  return vec4(p, id);
}

float
sdEqTri (vec2 p, float r)
{
  const float k = sqrt(3.0);
  p.x = abs(p.x) - r;

  p.y = p.y + r/k; 
  if(p.x + k*p.y>0.) p = vec2(p.x - k*p.y, -k*p.x - p.y)/2.;
  p.x -= clamp(p.x, -2.*r, 0.);

  return -length(p)*sign(p.y);
}

float
distLineS (vec2 p, vec2 a, vec2 b)
{
   b -= a; 

   return dot(p - a, vec2(-b.y, b.x)/length(b));
}
// glut_993_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define LOG_SPHERICAL
//#define SHOW_GRID
//#define SHOW_VERTICES
//#define SHOW_CIRCLES
// Gold: 0, Green: 1, Blue: 2, Greyscale: 3
#define COLOR 0
// Minimal bevel: 0, Normal: 1, Beveled round tops: 2, Beveled pointed tops: 3
#define BUMP 1

float
hash21 (vec2 f)
{
#ifdef LOG_SPHERICAL
  f = mod(f, 5.);
#endif

  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);

  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;

  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
}

void
main ()
{
  float res = iResolution.y;
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/res;

  const float sc = 1.;

  float sf = sc/res;

#ifdef LOG_SPHERICAL
  mat2 sRot = mat2(1, 0, 0, 1);
#else
  mat2 sRot = rot2(3.14159/18.);
#endif
  vec2 camDir = sRot*normalize(vec2(1.732, 1));
  vec2 ld = sRot*normalize(vec2(1, -1));
  vec2 p = sRot*uv*sc;
#ifndef LOG_SPHERICAL
  p += camDir*iTime*scale/3.;
#endif

  float r = 1.;
#ifdef LOG_SPHERICAL
  r = length(uv);
  p = vec2(log2(1./r)/2. + iTime/3., fract(atan(p.y, p.x)/6.2831853)*5. + iTime/6.);
#endif

  vec2 oP = p;

  mat3x2 vID, v;

  vec4 p4 = getTriVerts(p, vID, v);

  p = p4.xy;

  vec2 ctrID = p4.zw; 

  float sL = length(v[0] - v[1]);

  float ln = -1e5, vert = 1e5, mid = 1e5;

  float ew = .004;

  mat3x2 e, eID;
  for(int i = 0; i<3; i++)
  {
    int ip1 = (i + 1)%3;
    eID[i] = mix(vID[i], vID[ip1], .5);
    e[i] = mix(v[i], v[ip1], .5);
  }

  float sz = 1./(1. + sqrt(3.))*sL; 
  float szTri = sz;

  vec2 vertID;

#ifdef SHOW_CIRCLES
  float vertSz = sz/3.*.8660254;
  float vertSz2 = sz/4.*.8660254;
#else
  float vertSz = .06*scale;
  float vertSz2 = .06*scale;
#endif

  float cir = 1e5, cirB = 1e5, cirBV = 1e5;

  for(int i = 0; i<3; i++)
  {
    int ip1 = (i + 1)%3;

    float vI = length(p - v[i]) - vertSz;
    if(vI<vert)
    {
      vert = vI;            
      vertID = ctrID + vID[i];

      cirBV = min(cirBV, length(p - v[i])); 
    }

    float lnI = distLineS(p, v[i], v[ip1]);
    ln = max(ln, -lnI);

    float sz2 = hash21(ctrID + eID[i]);
    float eI = length(p - e[i]) - .05*scale;
    mid = min(mid, eI);
  }

  vec2 polyID = ctrID;

  float d = 1e5;

  float smF = .08*scale;

  vec2 a2 = gTri<0.? -p : p;
  float ang = floor(fract(atan(a2.x, a2.y)/6.2831853)*3.);
  int i = ang==0.? 0 : ang==2.? 1 : 2; 

  int ip1 = (i + 1)%3;

  vec2 q = p - e[i];
  vec2 nrm = normalize(e[i]);
  float a = atan(nrm.y, nrm.x);
  q = rot2(a)*q;
  float quad = sBoxS(q, vec2(sz/2.), smF);

  if(quad<d)
  {
    d = quad;
    polyID = ctrID + eID[i];
           
    cirB = min(cir, length(q));
    cir = min(cir, length(q) - sz/2.);
  }

  float edf = sqrt(1./3.)/(1. + sqrt(3.));
#ifdef LOG_SPHERICAL
  float edf2 = floor(edf*1024.)/1024.;
#else
  float edf2 = edf;
#endif

  q = p - mix(v[i], v[ip1], edf);
  q = rot2(-a)*q;
  float tri = sdEqTri(q, szTri/2. - smF) - smF/2.;
  if(tri<d)
  {
    d = tri;

    polyID = ctrID + mix(vID[i], vID[ip1], edf2);

    cirB = min(cir, length(q)); 
    cir = min(cir, length(q) - szTri/3.*.8660254);
  } 

  q = p - mix(v[ip1], v[i], edf);
  q = rot2(a)*q;
  tri = sdEqTri(q, szTri/2. - smF) - smF/2.;
  if(tri<d)
  {
    d = tri;
    polyID = ctrID + mix(vID[ip1], vID[i], edf2);

    cirB = min(cirB, length(q)); 
    cir = min(cir, length(q) - szTri/3.*.8660254);
  } 

  float mVL = 1./(1. + sqrt(3.));
  float vI = length(p - v[i]*mVL) - vertSz2;
  if(vI<vert)
  {
    vert = vI;
    vertID = ctrID + vID[i]*mVL;

    cirBV = min(cirBV, length(p - v[i]*mVL)); 
  }
  vI = length(p - v[ip1]*mVL) - vertSz2;
  if(vI<vert)
  {
    vert = vI;
    vertID = ctrID + vID[ip1]*mVL;
    cirBV = min(cirBV, length(p - v[ip1]*mVL)); 
  }

  q = gTri<0.? p*vec2(1, -1) : p;
  tri = sdEqTri(q, szTri/2. - smF) - smF/2.;
  if(tri<d)
  {
    d = tri;
    polyID = ctrID;

    cirB = min(cir, length(q)); 
    cir = min(cir, length(q) - szTri/3.*.8660254);
  }

  vec2 tuv2 = unskewXY(polyID, vec2(.5, 0));
  vec2 svTUV = tuv2;

  float mdTUV = mod(floor(tuv2.y/6. + .5*0.), 2.);    
  if(mdTUV==0.) tuv2.x += 3.;

  tuv2 = mod(tuv2*vec2(1, 1) + 3. + vec2(iTime*0., 0), 6.) - 3.;

  float rnd = hash21(polyID);

  float rnd2 = sin(6.2831853*length(tuv2)/6.)*.5 + .5;

#if 1
  vec3 sCol = .5 + .45*cos(6.2831853*rnd/6. + vec3(0, 1, 2)*1.25);
  if(rnd2<.9) sCol = (.5 + .45*cos(6.2831853*rnd/12. + vec3(1.02, 1, .98)*3.14159));
#else
  vec3 sCol = .5 + .45*cos(6.2831853*rnd/1. + vec3(1, 2, 3)*3.14159/2.);
  if(rnd2<.9) sCol = .5 + .45*cos(6.2831853*rnd/12. + vec3(1)*3.14159);
#endif

  sCol /= (.65 + sCol)/(1. + .65);

  tuv2 = unskewXY(vertID, vec2(.5, 0));

  if(mod(floor(tuv2.y/6. + .5*0.), 2.)==0.) tuv2.x += 3.;
  tuv2 = mod(tuv2*vec2(1, 1) + 3. + vec2(iTime*0., 0), 6.) - 3.;
  rnd = hash21(vertID);
  rnd2 = sin(6.2831853*length(tuv2)/6.)*.5 + .5;

#if 1
  vec3 vCol = .5 + .45*cos(6.2831853*rnd/6. + vec3(0, 1, 2)*1.25);
  if(rnd2<.75) vCol = (.5 + .45*cos(6.2831853*rnd/12. + vec3(1.02, 1, .98)*3.14159));
#else
  vec3 vCol = .5 + .45*cos(6.2831853*rnd/1. + vec3(1, 2, 3)*3.14159/2.);
  if(rnd2<.75) vCol = .5 + .45*cos(6.2831853*rnd/12. + vec3(1)*3.14159);
#endif    

#if COLOR == 1
  sCol = sCol.yxz;
  vCol = vCol.yxz;
#elif COLOR == 2
  sCol = sCol.zyx;
  vCol = vCol.zyx;
#endif

  vec3 tx = texture(iChannel0, oP).xyz; tx *= tx;

  sCol *= tx*3. + .2;

  cirB *= r;
  cirBV *= r;

  vec3 col = vec3(.03);

#ifdef SHOW_GRID
#ifdef SHOW_CIRCLES
  cir = max(cir, ln);   
#else
  d = max(d, ln);
#endif
#endif 
    
#ifndef SHOW_CIRCLES
  d *= r;
  col = mix(col, sCol*.05, (1. - smoothstep(0., sf, d)));
  col = mix(col, sCol, (1. - smoothstep(0., sf, d + ew)));
#else
  d = 1e5;
#endif
    
#ifdef SHOW_CIRCLES
  cir *= r;
  d = min(d, cir); 
  col = mix(col, vec3(0), (1. - smoothstep(0., sf, cir)));
  col = mix(col, sCol, (1. - smoothstep(0., sf, cir + ew)));
#endif

#ifdef SHOW_VERTICES
  vert *= r;
  if(vert<d)
  {
    cirB = cirBV;
    d = vert;
  }

  vec3 svCol = col;
  col = mix(col, vec3(0), (1. - smoothstep(0., sf, vert)));
  col = mix(col, mix(vCol, vec3(1), .25), (1. - smoothstep(0., sf, vert + ew)));
#endif

#if COLOR == 3
  col = vec3(1)*dot(col, vec3(.299, .587, .114));
#endif

  uv = gl_FragCoord.xy/iResolution;
  col *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , 1./16.);

#if BUMP == 0
  d = clamp(-d/(scale), 0., .03)/8.; // Very subtle edge bump.
  col *= 1.25; // Lightening the color a little.
#elif BUMP == 1
  d = max(-d/(scale), .0);
#elif BUMP == 2
  d = clamp(-d/(scale), 0., .05) - max(cirB*cirB/(scale)/(scale), 0.);
#else
  d = clamp(-d/(scale), 0., .05) + clamp(-d/(scale), .0, 1.)/2.;
#endif

  gl_FragColor = vec4((max(col, 0.)), d);
}
