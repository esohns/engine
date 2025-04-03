#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 15.

//#define NO_WEAVE
//#define SHOW_GRID

float objID = 0.;

mat2 r2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n), 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;

  return (tx*tx*n.x + ty*ty*n.y + tz*tz*n.z);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip;
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

vec2
path (float z)
{
  return vec2(sin(z * 0.15)*2.4, 0);
}

float hash (float n) { return fract(sin(n)*43758.5453); }
float hash21 (vec2 p) { return fract(sin(dot(p, vec2(141.187, 289.973)))*43758.5453); }
vec2 hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  return fract(vec2(262144, 32768)*n); 

  /*
    p = fract(vec2(262144, 32768)*n); 
    return sin( p*6.2831853 + iTime )*.5 + .5; 
  */
}

float
smax (float a, float b, float s)
{
  float h = clamp(.5 + .5*(a - b)/s, 0., 1.);
  return mix(b, a, h) + h*(1. - h)*s;
}

const vec2 s = vec2(1, 1.7320508);

vec4
getHex (vec2 p)
{
    vec4 hC = floor(vec4(p, p - vec2(.5, 1))/s.xyxy) + vec4(.5, .5, 1, 1.5);

    vec4 h = vec4(p - hC.xy*s, p - (hC.zw + vec2(0, -.5))*s );

    return dot(h.xy, h.xy)<dot(h.zw, h.zw) ? vec4(h.xy, hC.xy) : vec4(h.zw, hC.zw);
}

float
sTruchet (vec2 p)
{
  vec2 ip = floor(p);

  float rnd = hash21(ip); // Random ID.

  p -= ip + .5;

  p.y *= (rnd >.5)? -1. : 1.;

  p = p.x>-p.y ? p : -p; 

  float d = abs(length(p - .5) - .5) - .15;

  return mix(max(-d, 0.)*.35, smoothstep(0., .125, d), .5);
}

float
polDist (vec2 p)
{
  return length(p);

/*
  // Square cross section.
  p = abs(p);
  return max(p.x, p.y);
*/

/*
  // Hexagonal cross section.
  //p = abs(p);
  //return max(p.x*.8660254 + p.y*.5, p.y);
*/

/*
  // Octagonal cross section.
  p = abs(p);
  return max(max(p.x, p.y), (p.x + p.y)*.7071);
*/

/*
  // Dodecahedral cross section.
  p = abs(p);
  vec2 p2 = p.xy*.8660254 + p.yx*.5;
  return max(max(p2.x, p2.y), max(p.y, p.x));
*/
}

float
torDist (vec2 p)
{
  return length(p);

/*
  // Hexagon arcs.
  p = abs(p);
  return max(p.x*.8660254 + p.y*.5, p.y);
*/

/*
  // Dodecahedral arcs.
  p = abs(p);
  vec2 p2 = p.xy*.8660254 + p.yx*.5;
  return max(max(p2.x, p2.y), max(p.y, p.x));
*/
}

vec4 gHgt, gA, gA2, gD;
vec4
Truchet (vec2 p)
{
  gHgt = vec4(0);

  vec4 h = getHex(p);

#ifdef SHOW_GRID
  vec4 h2 = getHex(p - vec2(0, .8660254*2./3.));
#endif

  vec2 rnd = hash22(h.zw + .673);

  p = h.xy;

  const float rSm = .8660254/3.;
  const float rLg = .8660254;

  float a;

  float rFactor = floor(rnd.x*6.)*3.14159265/3.;
  p = r2(rFactor)*p;

  float hDir = rnd.y>.5? -1.: 1.;

  vec4 d;

  vec2 p1, p2, p3;

#ifdef NO_WEAVE
  p1 = p - vec2(.5, .8660254/3.);
  d.x = torDist(p1) - rSm;

  d.x = abs(d.x);
  a = atan(p1.y, p1.x);
  gA.x = a;

  p2 = p - vec2(-.5, .8660254/3.);
  d.y = torDist(p2) - rSm;

  d.y = abs(d.y);
  a = atan(p2.y, p2.x);
  gA.y = a;

  p3 = p - vec2(0, -.8660254*2./3.);
  d.z = torDist(p3) - rSm;

  d.z = abs(d.z);
  a = atan(p3.y, p3.x);
  gA.z = a;
#else
  p1 = p - vec2(1, 0); 
  a = atan(p1.y, p1.x); // Longer arc.
  gA.x = a*3.;
  d.x = torDist(p1) - rLg;
  d.x = abs(d.x);
  gHgt.x = (cos(a*6.) + 1.)*.05*hDir;

  p2 = p - r2(3.14159265/3.)*vec2(1, 0);
  a = atan(p2.y, p2.x); // Longer arc.
  gA.y = a*3.;
  d.y = torDist(p2) - rLg;
  d.y = abs(d.y);
  gHgt.y = -(cos(a*6.) + 1.)*.05*hDir;

  p3 = p - r2(-3.14159265/3.)*vec2(0, .57735);
  a = atan(p3.y, p3.x); // Smaller arc.
  gA.z = a;
  d.z = torDist(p3) - rSm;
  d.z = abs(d.z);
#endif

#ifdef SHOW_GRID
  vec2 q = h2.xy;
  float blc = abs(length(q) - .8660254/3.);// - th;

  q = r2(3.14159/6.)*q;
  a = atan(q.y, q.x);
  float ia = floor(a/6.283*3.) + .5;
  q = r2(ia*6.283/3.)*q;
  q.x -= .8660254/3.;

  q = abs(q);// - vec2(th, .04);

  d.w = max(q.x - .2, q.y - .02);//
#endif

  return d;
}

float
m (vec3 p)
{
  p.y += 1.5;

  float fl = .25 + p.y;// + (.5 - sf)*.5;

  const float sc = 1.;
  vec4 d = Truchet(p.xz*sc);

  d.x = polDist(vec2(d.x/sc, p.y + gHgt.x));
  d.y = polDist(vec2(d.y/sc, p.y + gHgt.y));    
  d.z = polDist(vec2(d.z/sc, p.y + gHgt.z));  

  d.xyz -= .16/sc;

  gD = d;

  float ob = min(min(d.x, d.y), d.z);

  fl = smax(fl, -ob*2., .5) - .2;

#ifdef SHOW_GRID
  d.w = max(d.w/sc, p.y - .25);
  fl = min(fl, d.w);
#endif

  objID = fl<ob? 0. : 1.;

  return min(fl, ob);
}

float
m2 (vec3 p)
{
  p.y += 1.5;

  float fl = .25 + p.y;// + (.5 - sf)*.5;

  const float sc = 1.;
  vec4 d = Truchet(p.xz*sc);

  gA2.x = atan(p.y + gHgt.x, d.x/sc);
  gA2.y = atan(p.y + gHgt.y, d.y/sc);
  gA2.z = atan(p.y + gHgt.z, d.z/sc);

  d.x = polDist(vec2(d.x/sc, p.y + gHgt.x));
  d.y = polDist(vec2(d.y/sc, p.y + gHgt.y));    
  d.z = polDist(vec2(d.z/sc, p.y + gHgt.z));  

  d.xyz -= .16/sc;

  gD = d;

  float ob = min(min(d.x, d.y), d.z);

  fl = smax(fl, -ob*2., .5) - .2;

#ifdef SHOW_GRID
  d.w = max(d.w/sc, p.y - .25);
  fl = min(fl, d.w);
#endif

  objID = fl<ob? 0. : 1.;

  return min(fl, ob);
}

float
bumpFunc (vec3 p, vec3 n)
{
  float d = m2(p);

  float c = 0.;

  if(objID<.5)
    c =  sTruchet(p.xz*6.);//*.95 +  sTruchet(p.xz*12.)*.05;
  else
  {
    float a;
    float a2;

    if(gD.x<gD.y && gD.x<gD.z)
    {
      a = gA.x;
      a2 = gA2.x;
    }
    else if(gD.y<gD.z)
    {
      a = gA.y;
      a2 = gA2.y;
    }
    else
    {
      a = gA.z;
      a2 = gA2.z;
    }

    c = sTruchet(vec2(a2*8., a*12.)/6.283);
    //c = sTruchet(vec2(a2*12., a*24.)/6.283);
    //c = sTruchet(vec2(a2*8., a*12.)/6.283)*.975 + sTruchet(vec2(a2*16., a*24.)/6.283)*.025;
  }

  return c;
}


vec3
doBumpMap (vec3 p, vec3 n, float bumpfactor, inout float edge, inout float crv)
{
  vec2 e = vec2(1./iResolution.y, 0); 

  float f = bumpFunc(p, n);

  float fx = bumpFunc(p - e.xyy, n); // Nearby sample in the X-direction.
  float fy = bumpFunc(p - e.yxy, n); // Nearby sample in the Y-direction.
  float fz = bumpFunc(p - e.yyx, n); // Nearby sample in the Y-direction.

  float fx2 = bumpFunc(p + e.xyy, n); // Sample in the opposite X-direction.
  float fy2 = bumpFunc(p + e.yxy, n); // Sample in the opposite Y-direction.
  float fz2 = bumpFunc(p + e.yyx, n);  // Sample in the opposite Z-direction.

  vec3 grad = vec3(fx - fx2, fy - fy2, fz - fz2)/(e.x*2.);  

  edge = abs(fx + fy + fz + fx2 + fy2 + fz2 - 6.*f);
  edge = smoothstep(0., 1., edge/e.x*2.);

  grad -= n*dot(n, grad);          

  return normalize(n + grad*bumpfactor);
}

vec3
nr (vec3 p)
{
  const vec2 e = vec2(0.002, 0);
  return normalize(vec3(m(p + e.xyy) - m(p - e.xyy), m(p + e.yxy) - m(p - e.yxy),	m(p + e.yyx) - m(p - e.yyx)));
}

float
shad (vec3 ro, vec3 lp, float k, float t)
{
  const int maxIterationsShad = 24; 

  vec3 rd = lp - ro;

  float shade = 1.;
  float dist = .001*(t*.125 + 1.);
  float end = max(length(rd), 0.0001);
  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = m(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist)); // Subtle difference. Thanks to IQ for this tidbit.
    dist += clamp(h, .01, .2); 

    if (h<0.0 || dist > end)
      break;
  }

  return min(max(shade, 0.), 1.); 
}

float
cAO (vec3 p, vec3 n)
{
  float sca = 1., occ = 0.;
  for(float i=0.; i<5.; i++)
  {
    float hr = .01 + i*.5/4.;
    float dd = m(n * hr + p);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp(1.0 - occ, 0., 1.);
}

vec3
rotHue (vec3 p, float a)
{
  vec2 cs = sin(vec2(1.570796, 0) + a);

  mat3 hr = mat3(0.299,  0.587,  0.114,  0.299,  0.587,  0.114,  0.299,  0.587,  0.114) +
            mat3(0.701, -0.587, -0.114, -0.299,  0.413, -0.114, -0.300, -0.588,  0.886) * cs.x +
            mat3(0.168,  0.330, -0.497, -0.328,  0.035,  0.292,  1.250, -1.050, -0.203) * cs.y;

  return clamp(p*hr, 0., 1.);
}

vec3
eMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd;

  rd.xy -= iTime*.25;
  rd *= 3.;

  float c = n3D(rd)*.57 + n3D(rd*2.)*.28 + n3D(rd*4.)*.15;
  c = smoothstep(0.5, 1., c);

  vec3 col = pow(vec3(1.5, 1, 1)*c, vec3(1, 2.5, 12)).zyx;

  return mix(col, col.yzx, sRd*.25 + .25); 
}

void
main ()
{
  vec4 fCol;
  vec2 u = (gl_FragCoord.xy - iResolution.xy*.5) / iResolution.y;

  vec3 lk = vec3(0, 0, iTime*1.);
  vec3 o = lk + vec3(0, .3, -.25);

  vec3 l = o + vec3(0, .5, 2.);

  lk.xy += path(lk.z);
  o.xy += path(o.z);
  l.xy += path(l.z);

  float FOV = 3.14159/3.;
  vec3 forward = normalize(lk-o);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 r = normalize(forward + FOV*u.x*right + FOV*u.y*up);

  float d, t = 0.;
  for(int i=0; i<96;i++)
  {
    d = m(o + r*t);
    if(abs(d)<.001*(t*.125 + 1.) || t>FAR)
      break;
    t += d;
  }
  t = min(t, FAR);

  float svID = objID;

  vec3 col = vec3(0);
  if(t<FAR)
  {
    vec3 p = o + r*t, n = nr(p);

    float edge2 = 0., crv2 = 1., bf = .25; 
    n = doBumpMap(p, n, bf, edge2, crv2);

    float sh = shad(p + n*.002, l, 16., t);
    float ao = cAO(p, n);

    l -= p;
    d = max(length(l), 0.001);
    l /= d;

    float txSc = .5;
    vec3 tx = tex3D(iChannel0, (p*txSc), n);
    tx = smoothstep(.0, .5, tx);

    col = tx;

    float fBm = n3D(p*128.)*.66 + n3D(p*256.)*.34;
    col *= mix(vec3(0), vec3(1), fBm*2.*.5 + .5);
 
    if(svID>.5)
      col *= mix(vec3(2, 1, .3), vec3(.1, 0, 0), bumpFunc(p, n)*1.5);
    else
      col *= vec3(.8, .6, .4);

    float df = max(dot(l, n), 0.);
    df = pow(df, 4.)*2.;
    float sp = pow(max(dot(reflect(-l, n), -r), 0.), 32.);

    col = col*(df + .5*ao) + vec3(1, .97, .92)*sp*2.;

    vec3 em = eMap(reflect(r, n), n);
    col += em*1.5;

    col *= 1. - edge2*.65;

    col *= 1./(1. + d*d*.1);

    col *= (sh + ao*.3)*ao;
  }

  vec3 fogCol = vec3(0);
  col = mix(col, fogCol, smoothstep(0., .95, t/FAR));

  u = gl_FragCoord.xy/iResolution.xy;
  col = mix(col, vec3(0), (1. - pow(16.*u.x*u.y*(1.-u.x)*(1.-u.y), 0.25))*.5);

  fCol = vec4(sqrt(clamp(col, 0., 1.)), 1.);
  gl_FragColor = fCol;
}
