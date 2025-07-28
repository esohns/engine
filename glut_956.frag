#version 140

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform samplerCube iChannel2;

#define FAR 20.

int objID;

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 p)
{
  return fract(sin(mod(dot(p, vec2(27.619, 57.583)), 6.2831853))*43758.5453); 
}

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= length(n);

  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;

  return mat3(tx*tx, ty*ty, tz*tz)*n;
}

float
hm (vec2 p)
{
  float rnd = hash21(p + .22);
  rnd = smoothstep(.84, .94, sin(6.2831*rnd + iTime/2.));
  float sn = dot(sin(p - cos(p.yx*1.25)*3.14159), vec2(.25)) + .5;
  //float sn = dot(getTex(iChannel0, p/4.), vec3(.299, .587, .114)); 
  return mix(sn, rnd, .2) + .05;
}

float
getTri (vec2 p, float r)
{
  const float k = sqrt(3.0);
  p.x = abs(p.x) - r;

  p.y = p.y + r/k; 
  if(p.x + k*p.y>0.) p = vec2(p.x - k*p.y, -k*p.x - p.y)/2.;
  p.x -= clamp(p.x, -2.*r, 0.);

  return -length(p)*sign(p.y);
}

float
opExtrusion (float sdf, float pz, float h)
{
  vec2 w = vec2( sdf, abs(pz) - h );

  return min(max(w.x, w.y), 0.) + length(max(w, 0.));
}

float
distLineS (vec2 p, vec2 a, vec2 b)
{
   b -= a; 

   return dot(p - a, vec2(-b.y, b.x)/length(b));
}

#define DIV_NUM 2

vec2 skewXY (vec2 p, vec2 s) { return mat2(1, -s.yx, 1)*p; }
vec2 unskewXY (vec2 p, vec2 s) { return inverse(mat2(1, -s.yx, 1))*p; }

const float scale = .8;
const vec2 rect = (vec2(1./.8660254, 1))*scale;
const vec2 sk = vec2(rect.x*.5, 0)/scale;

float gTri;

vec4
getTriVerts (vec2 p, inout mat3x2 vID, inout mat3x2 v)
{
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

  vec2 ctr = (v[0] + v[1] + v[2])/3.;
  p -= ctr;
  v[0] -= ctr; v[1] -= ctr; v[2] -= ctr;

  vec2 ctrID = (vID[0] + vID[1] + vID[2])/3.;
  vec2 tID = id*3. + ctrID;   

  vID[0] -= ctrID; vID[1] -= ctrID; vID[2] -= ctrID;

#if DIV_NUM > 0
  for(int j = 0; j<DIV_NUM; j++)
  {
    if(hash21(tID + float(j + 6)/32.)<.35)
    {
      mat3x2 mid, midID;
      vec3 dl;

      for(int i = 0; i<3; i++)
      {
        int ip1 = (i + 1)%3;
        mid[i] = mix(v[i], v[ip1], .5);
        midID[i] = mix(vID[i], vID[ip1], .5);

        dl[i] = distLineS(p, mid[i], mix(v[ip1], v[(i + 2)%3], .5));  
      }

      if(dl[0]<0.)
      {
        v[0] = mid[0]; vID[0] = midID[0];
        v[2] = mid[1]; vID[2] = midID[1];        
      }
      else if(dl[1]<0.)
      {
        v[1] = mid[1]; vID[1] = midID[1];
        v[0] = mid[2]; vID[1] = midID[2];        
      }
      else if(dl[2]<0.)
      {
        v[2] = mid[2]; vID[2] = midID[2];
        v[1] = mid[0]; vID[1] = midID[0];        
      }  
      else
      {
        v[0] = mid[0]; vID[0] = midID[0];
        v[1] = mid[1]; vID[1] = midID[1];
        v[2] = mid[2]; vID[2] = midID[2];  
        gTri = -gTri;
      }

      ctr = (v[0] + v[1] + v[2])/3.;

      p -= ctr;
      v[0] -= ctr; v[1] -= ctr; v[2] -= ctr;

      ctrID = (vID[0] + vID[1] + vID[2])/3.;
      tID += ctrID;   
      vID[0] -= ctrID; vID[1] -= ctrID; vID[2] -= ctrID;
    }
  }
#endif

  return vec4(p, tID);
}

vec2 gTriID;
mat3x2 gVert, gVertID;
vec2 gP;

float
tr (vec2 p)
{
  mat3x2 v, vID;
  vec4 p4 = getTriVerts(p, vID, v);
  vec2 triID = p4.zw;
  gTriID = p4.zw;

  gVert = v;
  gVertID = vID;
  gP = p4.xy;

  float ew = .01;
  vec2 q = p4.xy*vec2(1, gTri);

  float rad = length(v[0]);
  float d2D = getTri(q, (rad - ew - rad*.15*2.)*.8660254) - rad*.15;

  if (hash21(gTriID + .093) < .5)
    d2D = max(d2D, -(d2D + .3*sqrt(length(v[0]))));

  return d2D;
}

float
rayLine (vec2 ro, vec2 rd, vec2 p, vec2 n)
{
  float dn = dot(rd, n);

  return dn>0.? dot(p - ro, n)/dn : 1e8;   
} 

vec3 gRd;
float gCD;

float
m (vec3 p)
{
  float d2D = tr(p.xy);

  float fl = -p.z;

  vec2 gTriV = unskewXY(gTriID*rect/3., sk);
  float h = hm(gTriV);

  float obj = opExtrusion(d2D, p.z + h/2., h/2.);

  obj -= max(-(d2D + .02), .0)*.1;

  vec3 rC;
  rC.x = rayLine(gP.xy, gRd.xy, gVert[0], normalize(gVert[1] - gVert[0]).yx*vec2(1, -1));
  rC.y = rayLine(gP.xy, gRd.xy, gVert[1], normalize(gVert[2] - gVert[1]).yx*vec2(1, -1));
  rC.z = rayLine(gP.xy, gRd.xy, gVert[2], normalize(gVert[0] - gVert[2]).yx*vec2(1, -1));

  gCD = max(min(min(rC.x, rC.y), rC.z), 0.) + .0015;

  objID = fl<obj? 0 : 1 ;

  return min(fl, obj);
}

float
trace (vec3 ro, vec3 rd)
{
  float d, t = 0.;

  gRd = rd;

  for(int i = min(iFrame, 0); i<96; i++)
  {
    d = m(ro + rd*t);
    if(abs(d)<.001 || t>FAR)
      break;
    t += min(d, gCD); 
  }

  return min(t, FAR);
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int iter = 32; 

  ro += n*.0015;

  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.; 
  float end = max(length(rd), 0.0001);
  rd /= end;

  gRd = rd;

  for (int i = min(iFrame, 0); i<iter; i++)
  {
    float d = m(ro + rd*t);
    shade = min(shade, k*d/t);

    t += clamp(min(d, gCD), .01, .2); 

    if (d<0. || t>end)
      break;
  }

  return max (shade, 0.); 
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;

  for( int i = min(iFrame, 0); i<5; i++ )
  {
    float hr = float(i + 1)*.2/5.;        
    float d = m(p + n*hr);
    occ += (hr - d)*sca;
    sca *= .7;

    if(sca>1e5)
      break;
  }

  return clamp(1. - occ, 0., 1.);
}

vec3
nr (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.001, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += m(p + sgn*e)*sgn;
    sgn = -sgn;
    if((i&1)==1)
    {
      mp = mp.yzx;
      e = e.zxy;
    }
  }

  return normalize(mp);
}

void
main ()
{
  vec4 c;
  vec2 u = gl_FragCoord.xy;

  u = (u - iResolution.xy*.5)/iResolution.y;    

  vec3 r = normalize(vec3(u, 1)), o = vec3(iTime/4., iTime/8., -3.25),
       l = o + vec3(1, 2.5, 1);

  r.yz = rot2(.35)*r.yz;
  r.xz = rot2(-cos(iTime*3.14159/32.)/16.)*r.xz;
  r.xy = rot2(sin(iTime*3.14159/32.)/16. + .28)*r.xy; 

  r = normalize(vec3(r.xy, sqrt(max(r.z*r.z - dot(r.xy, r.xy)*.15, 0.))));

  float t = trace(o, r);

  int gObjID = objID;

  c = vec4(0);

  if(t<FAR)
  {
    vec3 p = o + r*t, n = nr(p); 

    vec3 ld = l - p;
    float lDist = length(ld);
    ld /= lDist;
    float at = 1.5/(1. + lDist*.05);

    float sh = softShadow(p, l, n, 8.);
    float ao = calcAO(p, n);

    vec2 uv = p.xy;

    mat3x2 v, vID;

    float d = tr(p.xy);

    vec4 oCol;

    if(gObjID == 1)
    {
      vec2 gTriV = unskewXY(gTriID*rect/3., sk);
      float h = hm(gTriV); 

      vec4 tx = tex3D(iChannel1, p/2., n).xyzx;

      vec4 col1 = tx*(tx*2. + .1)/2.;
      vec4 col2 = tx/2.;

      float bw = .05;
      float ew = .02;
      float sf = .007;
      oCol = col2*vec4(1, .6, .38, 0)/2.;
      oCol = mix(oCol, oCol*.05, 1. - smoothstep(0., sf, max(d, (p.z + h - bw))));

      vec4 fCol = mix(vec4(1), vec4(1, .7, .4, 0), hash21(gTriV + .08)*.5 + .1);
      col1 *= fCol;
      col2 *= fCol*.5 + .5;

      vec4 pearl = mix(vec4(1, .6, .2, 0), vec4(1, .6, .3, 0).zyxw, clamp(-n.y*8. + .5, 0., 1.))*2. + .1;
      col1 *= pearl;
      col2 *= pearl; 

      col1 += col1*vec4(.6, 1, .2, 0)/2.*(1. - smoothstep(0., sf*16., (d + .2)));

      oCol = mix(oCol, col2*vec4(1, .77, .5, 0), 1. - smoothstep(0., sf, max(d, (p.z + h - bw + ew))));
      oCol = mix(oCol, oCol*.05, 1. - smoothstep(0., sf, max(d, (p.z + h ))));

      oCol = mix(oCol, col2*vec4(1, .8, .6, 0), 1. - smoothstep(0., sf, d + ew));
      oCol = mix(oCol, oCol*.05, 1. - smoothstep(0., sf, d + ew + .02));
      oCol = mix(oCol, col1, 1. - smoothstep(0., sf, d + ew*2. + .02));
    }
    else
      oCol = vec4(.02); 

    float df = pow(max(dot(n, ld), 0.), 2. + 8.*oCol.x)*2.;
    float sp = pow(max(dot(reflect(r, n), ld), 0.), 16.);

    vec3 hv = normalize(ld - r);
    vec3 ref = reflect(r, n);
    vec4 refTx = texture(iChannel2, -ref.yzx); refTx *= refTx;
    float spRef = pow(max(dot(hv, n), 0.), 5.);
    float rf = mix(.5, 1., 1. - smoothstep(0., .01, d + .02));
    rf *= (gObjID == 0)? .1 : 1.;
    oCol += oCol*spRef*refTx.zyxw*rf*24.;

    c = oCol*(df*sh + sp*sh*8. + .3)*at*ao;
  }

  c = vec4(max(c.xyz, 0.), t);  

  gl_FragColor = c;
}
