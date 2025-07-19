#version 330

// glut_923_common.glsl
vec2 skewXY (vec2 p, vec2 s) { return mat2(1, -s.yx, 1)*p; }
vec2 unskewXY (vec2 p, vec2 s) { return inverse(mat2(1, -s.yx, 1))*p; }

const float scale = 1./3.;
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

  for(int i = 0; i<3; i++) v[i] = unskewXY(vID[i]*rect/3., sk);

  vec2 ctr = (v[0] + v[1] + v[2])/3.;
  p -= ctr;
  v[0] -= ctr; v[1] -= ctr; v[2] -= ctr;

  vec2 ctrID = (vID[0] + vID[1] + vID[2])/3.;
  vec2 tID = id*3. + ctrID;

  vID[0] -= ctrID; vID[1] -= ctrID; vID[2] -= ctrID;

  return vec4(p, id);
}

float
distLineS (vec2 p, vec2 a, vec2 b)
{
  b -= a; 
  return dot(p - a, vec2(-b.y, b.x)/length(b));
}
// glut_923_common.glsl

uniform vec2 iResolution;
uniform float iTime;

#define MOBIUS

// Greyscale: 0, Green: 1, Yellow: 2
#define FACE_COLOR 2

// Greyscale: 0, Pink: 1, Yellow: 2
#define LINK_COLOR 1

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
Mobius (vec2 p, vec2 z1, vec2 z2)
{
  z1 = p - z1;
  p -= z2;
  return vec2(dot(z1, p), z1.y*p.x - z1.x*p.y)/dot(p, p);
}

vec2
spiralZoom (vec2 p, vec2 offs, float n, float spiral, float zoom, vec2 phase)
{
  p -= offs;
  float a = atan(p.y, p.x)/6.2831;
  float d = log(length(p));
  p = vec2(a*n + d*spiral, -d*zoom + a) + phase;
    
#ifdef MOBIUS
  p *= vec2(1./1.732, 1);
#endif

  return p;
}

float
quadP (vec2 p, mat4x2 q)
{
  float d = -1e5;

  for(int i = 0; i<4; i++)
    d = max(d, distLineS(p, q[(i + 1)%4], q[i]));
    
  return d;
}

void
main ()
{
  float res = min(iResolution.y, 800.);
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/res;

#ifdef MOBIUS
  uv *= .75 + dot(uv, uv)*.5;
  const float sc = 1.25;
#else
  const float sc = 1.35;    
#endif
  float sf = sc/res;

#ifdef MOBIUS
  mat2 sRot = rot2(-3.14159/4.);
#else
  mat2 sRot = rot2(-3.14159/6.); //mat2(1, 0, 0, 1);
#endif

  vec2 camDir = sRot*normalize(vec2(1.732, 1));
  vec2 ld = sRot*normalize(vec2(1, -1));
  vec2 p = sRot*uv*sc;

#ifndef MOBIUS
  p -= camDir*iTime*scale/3.;
#endif

  float r = 1.;

#ifdef MOBIUS
  r = length(p - vec2(.5, 0));
  p = Mobius(p, vec2(-1, -.5), vec2(.5, 0));

  r = min(r, length(p - vec2(-.5))); 
  p = spiralZoom(p, vec2(-.5), 5., 3.14159*.2, .5, vec2(-1, 1)*iTime*.25);
#endif

  mat3x2 vID, v;
  vec4 p4 = getTriVerts(p, vID, v);
  p = p4.xy;
  vec2 ctrID = p4.zw; 

  float sL = length(v[0] - v[1]);

  float ln = 1e5;

#ifdef MOBIUS
  float ew = .0045;
#else
  float ew = .006;
#endif

  mat3x2 e, eID;
  for(int i = 0; i<3; i++)
  {
    int ip1 = (i + 1)%3;
    eID[i] = mix(vID[i], vID[ip1], .5);
    e[i] = mix(v[i], v[ip1], .5);
  }

  vec3 quad = vec3(1e3);

  float poly = -1e5;

  for(int i = 0; i<3; i++)
  {
    float ed = distLineS(p, v[i], v[(i + 1)%3]);
    poly = max(poly, -ed);   
  }
    
  float th = sL/6.*.8660254;
  float fL = sL/2.;

  for(int j = 0; j<3; j++)
  {
    int i = j;

    float dir = 1.;
    if(gTri<0.)
    {
      i = 2 - j;        
      dir *= -1.;
    }

    int ip1 = (i + 1)%3;
    int ip2 = (i + 2)%3;

    vec2 eN0 = -normalize(e[i])*th;
    vec2 eN1 = -normalize(e[ip1])*th;
    float innerLn0 = distLineS(p, v[i] + eN0, v[ip1] + eN0);
    float innerLn1 = distLineS(p, v[ip1] + eN1, v[ip2] + eN1);

    quad[i] = max(poly, max(dir*innerLn0, -dir*innerLn1));
  }
      
  vec3 sCol = .5 + .45*cos(6.2831*hash21(p4.zw)/1. + vec3(0, 1, 2)*2. - .85);

  vec3 sh = vec3(.6, .3, .9);

  mat3x3 fCol = mat3x3(vec3(1, .4, .2), vec3(.4, .2, .1), vec3(1, 1, .3));

  vec3 col = vec3(.025);

  if(gTri<0.)
  {
    sh = sh.zxy;

    fCol = mat3x3(vec3(1, 1, .3), vec3(1, .4, .2), vec3(.4, .2, .1));
  }

  vec3 face;
  vec3 faceEnd;

  mat4x2 pp, ppS;

  float shadow = 1e5;

  for(int i = 0; i<3; i++)
  {
    int ip1 = (i + 1)%3;
    int ip2 = (i + 2)%3;

    vec2 t0 = normalize(v[ip1] - v[i])*fL;
    vec2 t1 = normalize(v[ip2] - v[ip1])*fL;
    vec2 t2 = normalize(v[i] - v[ip2])*fL;

    pp[0] = v[i];
    pp[1] = v[i] + t0;
    pp[2] = v[i] + t0 + t1;
    pp[3] = v[i] + t1;

    if(gTri>0.)
    {
      pp[3] = v[i];
      pp[2] = v[i] - t2;
      pp[1] = v[i] - t2 - t1;
      pp[0] = v[i] - t1;
            
      ppS = mat4x2(pp[0], mix(pp[0], pp[1], .5), pp[2], pp[3]);
             
      if(i==2) shadow = quadP(p, ppS);
    }
    else
    {
      ppS = mat4x2(pp[0], pp[1], mix(pp[1], pp[2], .5), pp[3]);
      if(i==0) shadow = quadP(p, ppS);
    }

    face[i] = quadP(p, pp);

    float divLn = distLineS(p, v[i], v[ip2]);
    float smCDist = (fL - th)/2.*.75;
    float eD = (smCDist);
    quad[i] = max(quad[i], (divLn + eD));
    quad[i] = max(quad[i], -(divLn + (sL - fL)*.8660254));
  }

  float tempR = r; 
  float sR = sqrt(tempR);
  float rimW = scale/10.;

  for(int i = 0; i<3; i++)
  {
    int j = i;
    if(gTri<0.) j = (i + 1)%3;

    vec3 oCol = fCol[(i + 2)%3];

#if FACE_COLOR == 0
    oCol = vec3(1)*dot(oCol, vec3(.299, .587, .114));
#elif FACE_COLOR == 1
    oCol = fCol[i].yxz;
#endif

    col = mix(col, oCol*.05, 1. - smoothstep(0., sf, face[i]*r));
    vec3 eCol = mix(oCol*1.3, vec3(1), .1);
    col = mix(col, eCol, 1. - smoothstep(0., sf, (face[i] + ew/sR)*r));

    col = mix(col, oCol*.05, 1. - smoothstep(0., sf, (face[i] + rimW)*r));
    col = mix(col, oCol, 1. - smoothstep(0., sf, (face[i] + (rimW + ew*1./sR))*r));
  }

  rimW = scale/13.;

  float shF = iResolution.y/450.;

  for(int i = 0; i<3; i++)
  {
    int j = i;
    if(gTri<0.)
      j = (i + 2)%3;

    vec3 oCol = fCol[i];

#if LINK_COLOR == 0
    oCol = vec3(1)*dot(oCol, vec3(.299, .587, .114));
#elif LINK_COLOR == 1
    oCol = fCol[i].xzy;
#endif

    col = mix(col, vec3(0), (1. - smoothstep(0., sf*shF*12., max(quad[i], face[j] + .03)))*.5);
    col = mix(col, oCol*.05, 1. - smoothstep(0., sf, quad[i]*r));
    vec3 eCol = mix(oCol*1.3, vec3(1), .1);
    col = mix(col, eCol, (1. - smoothstep(0., sf, (quad[i] + ew/sR)*r)));

    col = mix(col, oCol*.05, 1. - smoothstep(0., sf, (quad[i] + rimW)*r));
    col = mix(col, oCol, 1. - smoothstep(0., sf, (quad[i] + (rimW + ew*1./sR))*r));
  }

  col = mix(col, vec3(0), (1. - smoothstep(0., sf*shF*4., shadow))*.5);

  uv = gl_FragCoord.xy/iResolution - .5;
  vec3 oCol = vec3(1.2, .8, .6);
#if FACE_COLOR == 0
  oCol = vec3(1)*dot(oCol, vec3(.299, .587, .114));
#elif FACE_COLOR == 1
  oCol = oCol.yxz/(1. + oCol.yxz)*2.;
#endif

#ifdef MOBIUS
  col *= oCol*smoothstep(.025, .5, tempR);
#if 0
  col = mix(col, col.zyx, 1. - smoothstep(.3, .7, r));
  col = mix(col, col.yxz, smoothstep(.3, 1., -uv.y + uv.x/2. + .4)); 
#else
  col = mix(col, col.zyx, smoothstep(.3, .8, -uv.y + uv.x/2. + .5)); 
#endif
#else
  col *= oCol;
  col = mix(col, col.zyx, smoothstep(.1, .9, -uv.y + uv.x/2. + .5));  
#endif

#ifdef SHOW_GRID
  vec3 svCol = col;
  col = mix(col, vec3(0), 1. - smoothstep(0., sf, (abs(poly*r) - ew*1.5)));
  col = mix(col, svCol*4. + .8, 1. - smoothstep(0., sf, (abs(poly*r) - ew/3.*sqrt(r))));
#endif

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
