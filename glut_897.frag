#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define sampNum 12

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 f)
{
  return texture (iChannel1, f*vec2(.2483, .1437)).x;
}

float
hash31 (vec3 f)
{
  uvec3 p = floatBitsToUint(f);
  p = 1103515245U*((p >> 2U)^(p.yzx>>1U)^p.zxy);
  uint h32 = 1103515245U*(((p.x)^(p.y>>3U))^(p.z>>6U));

  uint n = h32^(h32 >> 16);
  return float(n & uint(0x7fffffffU))/float(0x7fffffff);
}

vec2 seed = vec2(.183, .257);
vec2
hash22 ()
{
  seed = fract(seed + vec2(.7123, .6457));
  uvec2 p = floatBitsToUint(seed);

  p = 1103515245U*((p>>1U)^(p.yx));
  uint h32 = 1103515245U*((p.x)^(p.y>>3U));
  uint n = h32^(h32>>16);

  uvec2 rz = uvec2(n, n*48271U);
  return vec2((rz>>1)&uvec2(0x7fffffffU))/float(0x7fffffff);
}

float
sBox (vec2 p, vec2 b, float r)
{
  vec2 d = abs(p) - b + r;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - r;
}

vec3
cosDir (float seed, vec3 n)
{
  vec2 rnd = hash22();
  float u = rnd.x;
  float v = rnd.y;

  float a = 6.2831853*v;
  u = 2.*u - 1.;
  return normalize(n + vec3(sqrt(1. - u*u)*vec2(cos(a), sin(a)), u));
}

vec3
sphereNorm (vec3 p, float id, vec4 sph)
{
  return (p - sph.xyz)/sph.w;
}
 
vec4
boxIntersect (vec3 ro, vec3 rd, vec3 dim)
{
  const float maxT = 1e8;

  vec3 minD = (ro + dim)/rd, maxD = (ro - dim)/rd;
  minD = -(minD - step(vec3(-1e-6), minD)*(minD + maxT));
  maxD = -(maxD - step(vec3(-1e-6), maxD)*(maxD + maxT));
  minD = min(minD, maxD);

  vec4 res = vec4(maxT, 0, 0, 0);

  if (minD.x<maxT)
  {
    float pd = abs(ro.y + rd.y*minD.x) - dim.y;
    if(pd<0.) res = vec4(minD.x, -sign(rd.x), 0, 0);
  }

  if (minD.y<maxT)
  {
    float pd = abs(ro.x + rd.x*minD.y) - dim.x;
    if(pd<0.) res = vec4(minD.y, 0., -sign(rd.y), 0.);
  }

  return res;
}

vec2
sphereIntersect (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot(oc, rd);
  if(b > 0.) return vec2(1e8, 0.);
  float c = dot(oc, oc) - sph.w*sph.w;
  float h = b*b - c;
  if(h<0.) return vec2(1e8, 0.);
  return vec2(-b - sqrt(h), 1.); 
}

const float ballRad = .5;
vec4 sph4 = vec4(0, ballRad - 1., 1., ballRad);
vec3 boxNrm;
vec3
getNorm (vec3 p, float id)
{
  return (id<.5)? sphereNorm(p, id, sph4) : boxNrm; 
}

vec3
intersect (vec3 ro, vec3 rd)
{
  vec2[2] q;

  q[0] = sphereIntersect(ro, rd, sph4);

  vec4 bx = boxIntersect(ro - vec3(0, 1.5 - 1., -.5*0.), rd, vec3(2, 1.5, 1e8));
  q[1] = vec2(bx.x, 1);
  boxNrm = bx.yzw; 

  return q[0].x<q[1].x? vec3(q[0], 0) : vec3(q[1], 1);
}

vec3
distField2 (vec2 p, float scl, float rndZ, float oID)
{
  float ew = .0125; 

  vec2 pp = abs(fract(p) - .5);
  float sq = abs(max(pp.x, pp.y) - .5) - ew*2.;

  vec2 sc = vec2(1, 1)/scl; 

  vec2 ip = floor(p/sc);
  p -= (ip + .5)*sc;

  float d = sBox(p, sc/2. - ew, .1*min(sc.x, sc.y)*0.);

  if (hash21(ip + rndZ*.123 + oID + .055)<.65)
  {
    float f = scl*4.;
    float n = floor(hash21(ip + rndZ*.401 + oID + .043)*64.)*3.14159/4.;
    vec2 uv = rot2(n)*(p + vec2(f/2., 0));
    float g = (abs(fract(uv.x*f + n*f*0.) - .5) - .175)/f;
    d = max(d, g);
  }

  if(oID==0.) d = max(d, -sq);

  return vec3(d, ip*sc);
}

vec3
distField (vec2 p, float scl, float oID)
{
#define N(u) hash21(floor(u) + oID*.123) //texture(iChannel0, (u)/64.).x
  p *= scl;
  float rnd = N(p);

  vec2 p2 = p/(rnd + .1);

  vec2 pp = fract(p2) - .5;
  float sq = (abs(max(abs(pp.x), abs(pp.y)) - .5) - 1./40.)/2.*(rnd + .1);;
  rnd = N(p2);

  float n = floor(rnd*64.)*3.14159/4.;
  vec2 id = rnd + ceil(p2);

  float f = 1./N(id)/3.14159; 
  float g = (abs(fract((rot2(n)*p).x*f) - .5) - .25)/f; 

  g = max(g, -sq); 

  return vec3(g, id);
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
}

mat3
rot (vec3 ang)
{
  vec3 c = cos(ang), s = sin(ang);

  return mat3(c.x*c.z - s.x*s.y*s.z, -s.x*c.y, -c.x*s.z - s.x*s.y*c.z,
              c.x*s.y*s.z + s.x*c.z, c.x*c.y, c.x*s.y*c.z - s.x*s.z,
              c.y*s.z, -s.y, c.y*c.z);
}

vec4
cubeMap (vec3 p)
{
  vec3 f = abs(p); f = step(f.zxy, f)*step(f.yzx, f); 

  ivec3 idF = ivec3(p.x<.0? -1 : 1, p.y<.0? -1 : 1, p.z<0.? -1 : 1);

  ivec3 faceID = (idF + 1)/2 + ivec3(0, 2, 4);

  return f.x>.5? vec4(p.yz/p.x, idF.x, faceID.x) : 
         f.y>.5? vec4(p.xz/p.y, idF.y, faceID.y) : vec4(p.xy/p.z, idF.z, faceID.z); 
}
 
void
main ()
{
  float sf = 1./iResolution.y;

  float iRes = iResolution.y;
  vec2 seed0 = fract(iTime/vec2(111.13, 57.61))*vec2(-.143, .457);
  vec2 uv0 = (gl_FragCoord.xy - iResolution*.5)/iRes;

  float FOV = 1.;
  vec3 ro = vec3(0, 0, iTime*2. + sin(iTime)*.125);
  vec3 lk = ro + vec3(0, -.01, .25);
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt); 

  mat3 mCam = mat3(rgt, up, fwd);
  mCam *= rot(vec3(0, .05, 0)); 
  mCam *= rot(vec3(0, 0, -sin(iTime/2.)*.25)); 
  mCam *= rot(vec3(-cos(iTime/2.)*.25, 0, 0)); 

  if(iMouse.z>0.)
  {
    vec2 ms = (iMouse.xy/iResolution.xy - .5)*vec2(3.14159/2.);
    mCam *= rot(vec3(0, ms.y/2., -ms.x));
  }

  sph4.x -= cos(iTime/2.)*.25; // Left to right.
  sph4.z = ro.z + 4.; // In front of the camera.

  vec3 aCol = vec3(0);

  float gT = 1e8;
  float avgT = 0.;

  for(int j = min(0, iFrame); j<sampNum; j++)
  {
    seed = uv0 + seed0 + vec2(j*57, j*27)/1321.;
    vec2 jit = hash22()*2. - 1.;

    vec2 uv = uv0 - jit/iResolution.y;

    vec3 rd = mCam*normalize(vec3(uv, 1./FOV));

    vec3 cam = ro;
    vec3 sp = ro;

    vec3 col = vec3(0);

    vec3 emissive = vec3(0);
    vec3 through = vec3(1);
    vec3 sCol = vec3(0);

    float fogD = 1e8;

    for(int i = min(0, iFrame); i<3; i++)
    {
      vec3 scene = intersect(sp, rd);

      float t = scene.x;
      float retVal = scene.y;
      float id = scene.z;

      if(i==0)
      {
        fogD = t;
        avgT += t/float(sampNum);
        if(j==0) gT = fogD;
      }

      sp += rd*t;
  
      if(t<1e8)
      {
        vec3 sn = getNorm(sp, id); // Normal.

        vec3 oCol = vec3(0), emissive = vec3(0); // Object color, and emissivity.

        emissive = vec3(0);
        float rough = 0.;

        if(id<.5)
        {
          vec3 txP = sp - sph4.xyz;
          txP.xy *= rot2(sph4.x/(sph4.w)/2.);
          txP.yz *= rot2(-sph4.z/(sph4.w));

          vec4 q3 = cubeMap(txP);
          float faceID = q3.w;

          vec3 d3 = distField2((q3.xy/2. + .5), 6., q3.z, faceID);
          d3.x = smoothstep(0., sf, d3.x);

          float rnd2 = hash21(d3.yz + q3.z*.051 + faceID + .024);;
          float sRnd = rnd2;
          rnd2 = smoothstep(.4, .45, sin(6.2831*rnd2 + iTime/1.));
 
          vec3 wCol = .5 + .5*cos(6.2831*hash21(d3.yz + q3.z*5.51 + faceID + .374)/2. + vec3(0, 1, 2)*1.1 - 0.);
          oCol = mix(vec3(.9, .95, 1)*(hash21(d3.yz + q3.z*2.035 +  faceID + .144)*.5 + .5), vec3(.1), d3.x);

          wCol = wCol*vec3(4, 2, 1);  

          emissive = mix(wCol*(rnd2*.785 + .015)*3.*vec3(1, .97, .92), vec3(.005), d3.x);

          rough = hash21(d3.yz + q3.z + .11);
          rough = rough*rough*.3 + .025;

          rough *= hash31(sp + .51)*.5 + .75;

          if(hash21(d3.yz + faceID + .063)<.5)
          {
            oCol = vec3(1)*dot(oCol, vec3(.299, .587, .114));
            emissive = vec3(1)*dot(emissive, vec3(.299, .587, .114));
          }
        }
        else
        {
          float sgn = (abs(sn.z)>.5)? 1. : -1.;

          float wID = sgn < 0.? sp.y<0.? 0. : 2. : sp.z<0.? 1. : 3.;

          vec2 q = sgn>.5? sp.xy : abs(sn.x)>.5? sp.yz : sp.xz;

          float strip = abs(mod(sp.z, 4.) - 2.) - 3./6.;
          float yStrip = abs(sp.y - .5) - 1.5 + 1./6.;

          vec3 d3;

          if(strip<0. && yStrip<0.) d3 = distField2(q, 6., wID, id);
          else if(abs(sn.y)>.5) d3 = distField(q, 4., wID);
          else d3 = distField(q, 2., wID);

          d3.x = smoothstep(0., sf, d3.x);

          vec3 wCol = .5 + .5*cos(6.2831*hash21(d3.yz + wID*.054 + .274)/2. + vec3(0, 1, 2)*1.1 - 1.5*1.);
          vec3 wCol2 = .5 + .5*cos(6.2831*hash21(d3.yz + wID*.054 + .273)/2. + vec3(0, 1, 2)*1.1);

          if(strip>0.) wCol = vec3(1)*dot(wCol2, vec3(.299, .587, .114));
          else wCol = wCol2*vec3(4, 2, 1);

          oCol = mix(vec3(.9, .95, 1)*(hash21(d3.yz + wID*.054 + .174)*.5 + .5), vec3(.1), d3.x);

          float rnd2 = hash21(d3.yz + .067); 
          rnd2 = smoothstep(.4, .47, sin(6.2831*rnd2 + iTime/1.)*.5);

          if(abs(sn.y)<.5 && yStrip<0.)
            emissive = mix(wCol*rnd2*4.*vec3(1, .97, .92), vec3(.005), d3.x);

          if(abs(sn.x)>.5 && abs(yStrip - .015) - .015<0.){ oCol *= 0.; }   

          rough = hash21(d3.yz + wID*.021 + .11);
          rough = rough*rough*.3 + .025;
          rough *= hash31(sp + .41)*.5 + .75;
        }

        sCol += emissive*through;

        through *= oCol;
 
        vec3 ref = reflect(rd, sn);
        vec3 rrd = cosDir(0., sn);

        rd = normalize(mix(ref, rrd, rough));

        if(dot(rd, sn)<0.) rd = -rd; 

        sp += sn*1e-5;
      } 

      if(aCol.x>1e5)
        break;
    }

    sCol = mix(vec3(0), sCol, 1./(1. + fogD*fogD*.02));

    aCol += sCol;

    if(sCol.x>1e5)
      break;
  }

  aCol /= float(sampNum);

  vec4 preCol = texelFetch(iChannel0, ivec2(gl_FragCoord.xy), 0);
  float blend = (iFrame < 2) ? 1. : 1./2.;
  gl_FragColor = mix(preCol, vec4(max(aCol, 0.), avgT), blend);
}
