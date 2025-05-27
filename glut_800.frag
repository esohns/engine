#version 140

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

#define GLOW
#define FACE_DECO
#define OUTER_FACE_DECO
#define OFFSET_TRIS
#define TEXTURE
#define ANIMATE
#define COLOR 0

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }
float hash21 (vec2 p) { return fract(sin(dot(p, vec2(27.619, 57.583)))*43758.5453); }

vec2
hash22B (vec2 p)
{
  float n = sin(dot(p, vec2(1, 113)));
  p = fract(vec2(262144, 32768)*n)*2. - 1.; 
#ifdef ANIMATE
  return sin(p*6.2831853 + iTime/2.);
#else
  return p;
#endif
}

vec2
hash22C (vec2 p)
{
  float n = sin(dot(p, vec2(289, 41)));
  return fract(vec2(262144, 32768)*n)*2. - 1.;
  //p = fract(vec2(262144, 32768)*n)*2. - 1.; 
  //return sin(p*6.2831853 + iTime/2.); 
}

float
n2D3G (vec2 p)
{
  vec2 i = floor(p); p -= i;

  vec4 v;
  v.x = dot(hash22C(i), p);
  v.y = dot(hash22C(i + vec2(1, 0)), p - vec2(1, 0));
  v.z = dot(hash22C(i + vec2(0, 1)), p - vec2(0, 1));
  v.w = dot(hash22C(i + 1.), p - 1.);

  p = p*p*(3. - 2.*p);

  return mix(mix(v.x, v.y, p.x), mix(v.z, v.w, p.x), p.y);
}

float fBm (vec2 p) { return n2D3G(p)*.57 + n2D3G(p*2.)*.28 + n2D3G(p*4.)*.15; }

float
sdTri (vec2 p, vec2 p0, vec2 p1, vec2 p2)
{
  vec2 e0 = p1 - p0, e1 = p2 - p1, e2 = p0 - p2;

  vec2 v0 = p - p0, v1 = p - p1, v2 = p - p2;

  vec2 pq0 = v0 - e0*clamp( dot(v0, e0)/dot(e0, e0), 0., 1.);
  vec2 pq1 = v1 - e1*clamp( dot(v1, e1)/dot(e1, e1), 0., 1.);
  vec2 pq2 = v2 - e2*clamp( dot(v2, e2)/dot(e2, e2), 0., 1.);

  float s = sign( e0.x*e2.y - e0.y*e2.x);
  vec2 d = min( min( vec2(dot(pq0, pq0), s*(v0.x*e0.y - v0.y*e0.x)),
                     vec2(dot(pq1, pq1), s*(v1.x*e1.y - v1.y*e1.x))),
                     vec2(dot(pq2, pq2), s*(v2.x*e2.y - v2.y*e2.x)));

  return -sqrt(d.x)*sign(d.y);
}

vec3
inCentRad (vec2 p0, vec2 p1, vec2 p2)
{
  float bc = length(p1 - p2), ac = length(p0 - p2), ab = length(p0 - p1);
  vec2 inCir = (bc*p0 + ac*p1 + ab*p2)/(bc + ac + ab);   

  float p = (bc + ac + ab)/2.;
  float area = sqrt(p*(p - bc)*(p - ac)*(p - ab));

  return vec3(inCir, area/p);
}

vec2 skewXY (vec2 p, vec2 s) { return mat2(1, -s.yx, 1)*p; }
vec2 unskewXY (vec2 p, vec2 s) { return inverse(mat2(1, -s.yx, 1))*p; }

struct triS
{
  vec2[3] v;
  vec2 p;
  vec2 id;
  float dist;
  float triID;
};

const float tf = 2./sqrt(3.);
const vec2 scale = vec2(tf, 1)*vec2(1./3.);
const vec2 dim = vec2(scale);
const vec2 s = dim*2.;
const vec2 sk = vec2(tf/2., 0);

triS
blocks (vec2 q)
{
  float d = 1e5;

  vec2 p, ip;

  vec2 id = vec2(0), cntr;

  const vec2[4] ps4 = vec2[4](vec2(-.5, .5), vec2(.5), vec2(.5, -.5), vec2(-.5)); 

  float triID = 0.; // Triangle ID. Not used in this example, but helpful.

  const float hs = .5;

  triS gT, tri1, tri2;

  for(int i = min(0, iFrame); i<4; i++)
  {
    cntr = ps4[i]/2.;// -  ps4[0];
    p = skewXY(q.xy, sk);// - cntr*s;
    ip = floor(p/s - cntr) + .5;
    p -= (ip + cntr)*s;
    p = unskewXY(p, sk);

    vec2 idi = ip + cntr;

    vec2[4] vert = ps4;  

#ifdef OFFSET_TRIS
    vert[0] += hash22B((idi + vert[0]/2.))*.2;
    vert[1] += hash22B((idi + vert[1]/2.))*.2;
    vert[2] += hash22B((idi + vert[2]/2.))*.2; 
    vert[3] += hash22B((idi + vert[3]/2.))*.2;
#endif

    vert[0] = unskewXY(vert[0]*dim, sk);
    vert[1] = unskewXY(vert[1]*dim, sk);
    vert[2] = unskewXY(vert[2]*dim, sk);
    vert[3] = unskewXY(vert[3]*dim, sk); 

    idi = unskewXY(idi*s, sk);  

    tri1.v = vec2[3](vert[0], vert[1], vert[2]); 
    tri1.id = idi + inCentRad(tri1.v[0], tri1.v[1], tri1.v[2]).xy;
    tri1.triID = float(i);
    tri1.dist = sdTri(p, tri1.v[0], tri1.v[1], tri1.v[2]);
    tri1.p = p;

    tri2.v = vec2[3](vert[0], vert[2], vert[3]);
    tri2.id = idi + inCentRad(tri2.v[0], tri2.v[1], tri2.v[2]).xy;
    tri1.triID = float(i + 4);
    tri2.dist = sdTri(p, tri2.v[0], tri2.v[1], tri2.v[2]);
    tri2.p = p;

    //triS gTi = tri1.dist<tri2.dist? tri1 : tri2;
    triS gTi; 
    if(tri1.dist<tri2.dist) gTi = tri1; 
    else gTi = tri2;

    if(gTi.dist<d)
    {
      d = gTi.dist;
      gT = gTi;
      gT.id = idi;
    }
  }

  return gT;
}

void
main ()
{
  float iRes = min(iResolution.y, 834.);
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iRes;
  uv.xy *= rot2(-3.14159/9.);

  vec3 rd = normalize(vec3(uv, 1));
  vec3 ro = vec3(0);

  const float gSc = 1.;
  vec2 p = uv*gSc - vec2(0, iTime/32.).yx;

  float sf = gSc/iResolution.y;

  triS gT = blocks(p);

  vec2[3] v = gT.v;
  vec2 rp = gT.p;
  vec2 svID = gT.id;

  vec3 col = vec3(0);  

  vec3 tCol = vec3(0);

  vec3 cntr = inCentRad(v[0], v[1], v[2]);

  vec3 lp = vec3(-.5, 1, -1);
  vec3 ld = lp - vec3(uv, 0);
  float lDist = length(ld);
  ld /= lDist;

  float atten = 2./(1. + lDist*lDist*.5);

  float triCell = sdTri(rp, v[0], v[1], v[2]);
  //float triCell2 = sdTri(rp, v[0] - ld.xy*.01, v[1] - ld.xy*.01, v[2] - ld.xy*.01);

  float cir = length(rp - cntr.xy);

  float opening = triCell;

  vec3 glCol = vec3(1, .35, .2);
#if COLOR == 1
  glCol = mix(glCol.zyx, glCol.zxy, clamp(-uv.y*1.25 + .5, 0., 1.));
#endif
  glCol = mix(glCol, glCol.xzy, dot(sin(p*2. - cos(p.yx*4.)*3.), vec2(.125)) + .25);
  //tx = glCol;
  glCol *= (fBm((rp -svID*s)*128.)*.25 + .75)*1.25;

  col += glCol*max(.2 - triCell/scale.x*6., 0.);

  float edge = 1e5, openMax = 0.;

  for(int j = min(0, iFrame); j<3; j++)
  {
    float rnd = hash21(svID + .08);
    float rndJ = hash21(svID + float(j)/9. + .13);

    float open = smoothstep(.9, .966, sin(rnd*6.2831 + rndJ/6. + iTime/1.)*.5 + .5);
#ifndef GLOW
    open = 0.;
#endif

    vec3 p0 = vec3(v[j], 0);
    vec3 p1 = vec3(v[(j + 1)%3], 0);
    vec3 p2 = vec3(cntr.xy, -.2);
    p2.xy -= normalize(vec3(v[(j + 2)%3], 0) - p2).xy*.065*scale.x*open;

    float triJ = sdTri(rp, v[j], v[(j + 1)%3], p2.xy);

    float z = 1./(1. - p2.z);

    p2.xy += normalize(v[(j + 2)%3] - cntr.xy)*.008*open;

    float triEdge = sdTri(rp, v[j], v[(j + 1)%3], p2.xy);

    vec3 nJ = normalize(cross(p1 - p0, p2 - p0));

    float diff = max(dot(ld, nJ), 0.);
    diff = pow(diff, 4.)*2.;
    float spec = pow(max(dot(reflect(ld, nJ), rd ), 0.), 8.); 
 
#ifdef TEXTURE
    vec3 tx2 = texture(iChannel0, (rp - svID*s)*z).xyz; tx2 *= tx2;

    vec3 tCol = smoothstep(-.5, 1., tx2)*.1;
#else
    vec3 tCol = vec3(.035);
#endif

    tCol = tCol*(diff + .25 + spec*4.);

    if(open>1e-5)
    {
      col = mix(col, vec3(0), open*(1. - smoothstep(0., sf*4., triEdge - .00)));
      col = mix(col, mix(tCol*vec3(1.5, 1.25, 1), vec3(0), open), (1. - smoothstep(0., sf*2., triEdge)));
      col = mix(col, mix(tCol, glCol, .5)*(open), (1. - smoothstep(0., sf, triEdge + .005)));
    }

    col = mix(col, tCol*vec3(1.5, 1.25, 1)*(1. - open), 1. - smoothstep(0., sf*2., triJ));
    col = mix(col, tCol, 1. - smoothstep(0., sf*2., triJ + .005));

#ifdef OUTER_FACE_DECO
    col = mix(col, mix(tCol, glCol*1., open), 1. - smoothstep(0., sf*4., abs(triJ + .035) - .002));
    col = mix(col, mix(tCol, tCol/3., open), 1. - smoothstep(0., sf, abs(abs(triJ + .035) - .006) - .00125));
#endif
        
#ifdef FACE_DECO
    col = mix(col, mix(tCol, (diff + .25 + spec*4.)*glCol, open), (1. - smoothstep(0., sf*3., triJ + .035))*.2);
    col = mix(col, mix(tCol, vec3(0), open), 1. - smoothstep(0., sf*2., triJ + .035));         
    col = mix(col, mix(tCol, (diff + .25 + spec*4.)*glCol*2., open), 1. - smoothstep(0., sf, triJ + .035 + .005));          
#endif

    edge = min(edge, abs(triJ));

    p0.xy -= (p2.xy - p0.xy)*1.;
    p1.xy -= (p2.xy - p1.xy)*1.;
    float eTri = sdTri(rp, p0.xy*8., p1.xy*8., p2.xy);

    openMax = max(openMax, open);
    opening = max(opening, -eTri);
  }

  cir = mix(cir, opening, .65);
  col = mix(col, col + col*glCol*(openMax*2.5 + .5), (1. - smoothstep(0., sf*24., cir)));

  col *= clamp(.5 - triCell/scale.x*4., 0., 1.);

  float ns = fBm((rp - cntr.xy)*128.)*.5 + .5;
  col *= .5 + ns*.75;

  col *= atten;

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
