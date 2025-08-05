#version 140

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

//#define NIGHT

#define FAR 60.

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }
float hash (vec3 p) { return fract(sin(dot(p, vec3(21.71, 157.97, 113.43)))*45758.5453); }

float
smax (float a, float b, float k)
{
  float f = max(0., 1. - abs(b - a)/k);

  return max(a, b) + k*.25*f*f;
}

float
smin (float a, float b, float k)
{
  float f = max(0., 1. - abs(b - a)/k);

  return min(a, b) - k*.25*f*f;
}

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= dot(n, vec3(1));

  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;

  return (tx*tx*n.x + ty*ty*n.y + tz*tz*n.z);
}

#define RIGID
vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(113, 1)));
  p = fract(vec2(2097152, 262144)*n)*2. - 1.;
#ifdef RIGID
  return p;
#else
  return cos(p*6.283 + iGlobalTime);
  //return abs(fract(p+ iGlobalTime*.25)-.5)*2. - .5;
  //return abs(cos(p*6.283 + iGlobalTime))*.5;
#endif
}

float
gradN2D (vec2 f)
{
  const vec2 e = vec2(0, 1);

  vec2 p = floor(f);
  f -= p;

  vec2 w = f*f*(3. - 2.*f);

  float c = mix(mix(dot(hash22(p + e.xx), f - e.xx), dot(hash22(p + e.yx), f - e.yx), w.x),
                mix(dot(hash22(p + e.xy), f - e.xy), dot(hash22(p + e.yy), f - e.yy), w.x), w.y);

  return c*.5 + .5;
}

float
fBm (vec2 p)
{
  return gradN2D(p)*.57 + gradN2D(p*2.)*.28 + gradN2D(p*4.)*.15;
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
n2DS (vec2 p)
{
  vec2 i = floor(p);
  p -= i;

  return dot(mat2(fract(sin(vec4(0, 1, 113, 114) + dot(i, vec2(1, 113)))*43758.5453)) * vec2(1. - p.y, p.y), vec2(1. - p.x, p.x));
}

vec2
path (float z)
{
  return vec2(6.*sin(z * .1), 0.);
}

float
surfFunc (vec3 p)
{
  p /= 2.5;

  return n2D(p.xz)*.67 + n2D(p.xz*2.)*.33;
}

vec2
hash22B (vec2 p)
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
}

float
hm (vec2 p)
{
  vec2 pth = p.xy - path(p.y);
  float camPath = abs(pth.x);

  p = p/2.5;// + iTime/4.;

  float n = n2DS(p.xy + 7.5);
  n = smoothstep(.2, 1., n)*clamp(camPath - .6, 0., 1.);

  return n;
}

float
sBox (vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;

  return min(max(max(d.x, d.y), d.z), 0.) + length(max(d, 0.));
}

float
sBoxS (vec2 p, vec2 b, float sf)
{
  vec2 d = abs(p) - b + sf;

  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - sf;
}

float
opExtrusion (float sdf, float pz, float h)
{
  vec2 w = vec2( sdf, abs(pz) - h );

  return min(max(w.x, w.y), 0.) + length(max(w, 0.));
}

vec2 skewXY (vec2 p, vec2 v) { return mat2(1, -v.y, v.x, 1)*p; }
vec2 unskewXY (vec2 p, vec2 v) { return inverse(mat2(1, -v.y, v.x, 1))*p; }

vec4
herringbone3D (vec3 q3)
{
  const float scale = 1./3.;

  const vec2 sk = vec2(1, -1)/5.;

  const vec2 dim = vec2(1.5, 1)*scale;

  vec2 s = (vec2(2.5, 2.5) - abs(sk)/2.)*scale;

  q3.y *= scale/2.;

  float d = 1e5;

  vec2 cntr, p, ip;

  vec2 id = vec2(0);
  vec2 l = dim;
  cntr = vec2(0);
  float boxID = 0.;

  for(int i = 0; i<4; i++)
  {
    if(i==2)
    {
      cntr = vec2((dim.x + dim.y)/2., -dim.y/4.);
      l = l.yx;
    }

    p = q3.xz - cntr;
    p = skewXY(p, sk);
    ip = floor(p/s) + .5;
    p -= (ip)*s;
    p = unskewXY(p, sk);

    float di2D = sBoxS(p, l/2., .04);

    vec2 idi = ip*s + cntr;

    idi = unskewXY(idi, sk);

    float h = max(floor(hm(idi)*24.999)/24., 0.)*.5 + .002;

    float di = opExtrusion(di2D + .01, q3.y - h, h);

    if(h>.0025)
      di = max(di, -(di2D + .1));

    if(di<d)
    {
      d = di;
      id = idi;
      boxID = float(i);
    }

    cntr -= -dim.y;
  }

  return vec4(d, id, boxID);
}

vec2 bID;
vec3 objID;

float
map (vec3 p)
{
  float ter = p.y + (.5 - surfFunc(p))*.8 - .05;

  vec4 her = herringbone3D(p); 

  bID = her.yz;

  objID = vec3(ter, her.x, 1e5);

  return smin(ter, her.x, .005);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0., d;

  for(int i = 0; i<128; i++)
  {
    d = map(ro + rd*t);

    if(abs(d)<.001*(t*.05 + 1.) || t>FAR)
      break;

    t += i<32? d*.75 : d*(1. + t*.05); 
  }

  return min(t, FAR);
}

vec3
normal (vec3 p, float ef)
{
  vec2 e = vec2(.001*ef, 0.);

  return normalize(vec3(map(p + e.xyy) - map(p - e.xyy), map(p + e.yxy) - map(p - e.yxy),	map(p + e.yyx) - map(p - e.yyx)));
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(113, 157, 1);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

float gT;

float
bumpSurf3D (vec3 p)
{
  p *= vec3(1.65, 2.2, 3.85);

  float ns = n3D(p)*.57 + n3D(p*2.)*.28 + n3D(p*4.)*.15;

  ns = (1. - abs(smoothstep(0., 1., ns) - .5)*2.);
  ns = mix(ns, smoothstep(0., 1., ns), .65);

  ns = ns*smoothstep(0., .2, p.y - .075);

  return ns/(1. + gT*gT*.015);
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  const vec2 e = vec2(0.001, 0); 

  float ref = bumpSurf3D(p);
  vec3 grad = (vec3(bumpSurf3D(p - e.xyy),
                    bumpSurf3D(p - e.yxy),
                    bumpSurf3D(p - e.yyx)) - ref)/e.x; 

  grad -= nor*dot(nor, grad);          

  return normalize(nor + grad*bumpfactor);
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int maxIterationsShad = 32; 

  ro += n*.0015;
  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.;
  float end = max(length(rd), 0.0001);

  rd /= end;

  for (int i = 0; i<maxIterationsShad; i++)
  {
    float d = map(ro + rd*t);
    shade = min(shade, k*d/t);

    t += clamp(d*.75, .05, .35); 

    if (d<0. || t>end)
      break;
  }

  return max(shade, 0.); 
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 3., occ = 0.;
  for( int i = 0; i<5; i++ )
  {
    float hr = float(i + 1)*.15/5.;        
    float d = map(p + n*hr);
    occ += (hr - d)*sca;
    sca *= .7;
  }

  return clamp(1. - occ, 0., 1.);  
}

vec3
getSky (vec3 ro, vec3 rd, vec3 ld)
{
  vec3 col = vec3(.8, .7, .5), col2 = vec3(.4, .6, .9);

  vec3 sky = mix(col, col2, pow(max(rd.y + .15, 0.), .5));

  float sun = clamp(dot(ld, rd), 0., 1.);
  sky += vec3(1, .7, .4)*vec3(pow(sun, 16.))*.2;
  sun = pow(sun, 32.);
  sky += vec3(1, .9, .7)*vec3(pow(sun, 32.))*.35/vec3(1.2, 1, .8);

  rd.z *= 1. + length(rd.xy)*.15;
  rd = normalize(rd);

  const float SC = 1e5;
  float t = (SC - ro.y - .15)/(rd.y + .15);
  vec2 uv = (ro + t*rd).xz;

  if(t>0.)
    sky =  mix(sky, vec3(2), smoothstep(.45, 1., fBm(1.5*uv/SC)) * smoothstep(.45, .55, rd.y*.5 + .5)*.4);

  return sky*vec3(1.2, 1, .8);
}

float
noise3D (vec3 p)
{
  const vec3 s = vec3(113, 157, 1);

  vec3 ip = floor(p);

  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);

  p -= ip;

  p = p*p*(3. - 2.*p);

  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);

  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

float hash31 (vec3 p) { return fract(sin(dot(p, vec3(127.1, 311.7, 74.7)))*43758.5453); }

float
getMist (vec3 ro, vec3 rd, vec3 lp, float t)
{
  float mist = 0.;

  float t0 = 0.;

  for (int i = 0; i<24; i++)
  {
    if (t0>t)
      break;

    float sDi = length(lp-ro)/FAR; 
    float sAtt = 1./(1. + sDi*.25);

    vec3 ro2 = (ro + rd*t0)*2.5;
    float c = noise3D(ro2)*.65 + noise3D(ro2*3.)*.25 + noise3D(ro2*9.)*.1;

    float n = c;
    mist += n*sAtt;

    t0 += clamp(c*.25, .1, 1.);
  }

  return max(mist/48., 0.);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  vec3 ro = vec3(0, 1.5, iTime*2.);
  vec3 lk = ro + vec3(0, -.1, .5);

  ro.xy += path(ro.z);
  lk.xy += path(lk.z);

  float FOV = 3.14159265/2.5;
  vec3 fw = normalize(lk - ro);
  vec3 rt = normalize(vec3(fw.z, 0, -fw.x )); 
  vec3 up = cross(fw, rt);

  vec3 rd = normalize(fw + (u.x*rt + u.y*up)*FOV);

  rd = normalize(vec3(rd.xy, rd.z*(1. - length(rd.xy)*.25)));

  rd.xy = rot2( path(lk.z).x/48.)*rd.xy;

  vec3 lp = vec3(FAR*.25, FAR*.35, FAR) + vec3(0, 0, ro.z);

  float t = trace(ro, rd);

  vec2 svBID = bID;
  vec3 oSvObjID = objID;

  float svObjID = objID.x < objID.y? 0. : 1.;

  gT = t;

  vec3 col = vec3(0);

  vec3 sp = ro + t*rd; 

  float pathHeight = sp.y;

  if (t < FAR)
  {
    vec3 sn = normal(sp, 1.);

    vec3 ld = lp - sp;
    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    lDist /= FAR;
    float atten = 1./(1. + lDist*lDist*.05);

    const float tSize = 1./8.;

    if(svObjID==0.)
      sn = doBumpMap(sp, sn, .1);

    float bf = .01;

    float sh = softShadow(sp, lp, sn, 8.);
    float ao = calcAO(sp, sn);

    sh = min(sh + .25, 1.); 

    float dif = max( dot( ld, sn ), 0.);
    float spe = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 5.);
    float fre = clamp(1.0 + dot(rd, sn), 0., 1.);

    float Schlick = pow( 1. - max(dot(rd, normalize(rd + ld)), 0.), 5.0);
    float fre2 = mix(.2, 1., Schlick);

    float amb = ao*.35;

    vec3 tx = tex3D(iChannel0, sp/2., sn);

    tx = smoothstep(-.2, .4, tx);

    vec3 sndTx = mix(vec3(1, .95, .7), vec3(.9, .6, .4), fBm(sp.xz*16.));

    vec3 col0 = col;
    vec3 col1 = col;

    float bordCol0Col1 = oSvObjID.x - oSvObjID.y;
    const float bordW = .1;

    if(svObjID==1. || abs(bordCol0Col1)<bordW)
    {
      col1 = tx*mix(vec3(1), sndTx*2., .6);

      col1 *= 1./(1. + t*t*.005);

      vec3 bTx = texture(iChannel0, svBID).xyz; bTx *= bTx;
      bTx =  smoothstep(0., .5, bTx);

      col1 = mix(col1, col1*bTx*2.4, .35);
    }

    if(svObjID==0. || abs(bordCol0Col1)<bordW)
    {
      col0 = sndTx;
      col0 = mix(col0*1.4, col0*.6, fBm(sp.xz*32. - .5));

      float bSurf = bumpSurf3D(sp);
      col0 *= bSurf*.75 + .5;
      col0 *= vec3(1.5, 1.45, 1.3)/2.;
    }

    col = mix(col0, col1, smoothstep(-bordW, bordW, bordCol0Col1));

    col = mix(col*.7 + (hash(floor(sp*96.))*.7 + hash(floor(sp*192.))*.3)*.3, col, min(t*t/FAR, 1.));
    col *= vec3(1.2, 1, .9);

    col = col*(dif + amb + vec3(1, .97, .92)*fre2*spe*2.)*atten;

    vec3 refSky = getSky(sp, reflect(rd, sn), ld);
    col += col*refSky*.05 + refSky*fre*fre2*atten*.15; 

    col *= sh*ao;
  }

  float dust = getMist(ro, rd, lp, t)*(1. - smoothstep(0., 1., pathHeight*.05));
  vec3 gLD = normalize(lp - vec3(0, 0, ro.z));
  vec3 sky = getSky(ro, rd, gLD);

  col = mix(col, sky, smoothstep(0., .95, t/FAR));

  vec3 mistCol = vec3(1, .95, .9);
  col += (mix(col, mistCol, .66)*.66 + col*mistCol*1.)*dust;

  col += vec3(1., .7, .4)*pow( max(dot(rd, gLD), 0.), 16.)*.4;

  col = col*.75 + (col + .25*vec3(1.2, 1, .9))*mistCol*dust*1.5;

#ifdef NIGHT
  col *= vec3(.4, .6, 1);
#endif

  u = gl_FragCoord.xy/iResolution;
  col = min(col, 1.)*pow( 16.*u.x*u.y*(1. - u.x)*(1. - u.y) , .0625);

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
