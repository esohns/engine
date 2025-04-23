#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

//#define PENTAGON
//#define HALF_PLANE

#define PI		3.14159265
#define TAU	    6.28318531

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

void swap (inout int a, inout int b) { int tmp = a; a = b; b = tmp; }

float
floorSm (float x, float c)
{
  float ix = floor(x); x -= ix;
  return (pow(x, c) - pow(1.- x, c))/2. + ix;
}

#ifdef PENTAGON
const int N = 5;
const int Q	= 4;
#else
const int N = 3;
const int Q	= 8;
#endif

float count;

vec3
initDomain ()
{
  float piDivN = PI/float(N), piDivQ = piDivN + PI/float(Q) - PI/2.;
    
  vec2 trig1 = vec2(cos(piDivN), sin(piDivN));
  vec2 trig2 = vec2(cos(piDivQ), sin(piDivQ));

  float dist = trig1.x - trig2.x*trig1.y/trig2.y;
  float rad = length(vec2(dist, 0.) - trig1);

  float d = max(dist*dist - rad*rad, 0.);

  return vec3(dist, rad, 1)/sqrt(d);
}

vec2
transform (vec2 p, vec2 circ)
{
  float ia = (floor(atan(p.x, p.y)/TAU*float(N)) + .5)/float(N);

  vec2 vert = rot2(ia*TAU)*vec2(0, circ.x);

  float rSq = circ.y*circ.y;

  vec2 pc = p - vert;
  float lSq = dot(pc, pc);
  if(lSq<rSq)
  {
    p = pc*rSq/lSq + vert;
    p = rot2(TAU/float(N)*(count + float(Q)))*p;

    count++; 
  }

  return p;
}

float
sdBezier (vec2 pos, vec2 A, vec2 B, vec2 C)
{
  vec2 a = B - A;
  vec2 b = A - 2.0*B + C;
  vec2 c = a * 2.0;
  vec2 d = A - pos;

  float kk = 1./max(dot(b,b), 1e-6);
  float kx = kk * dot(a,b);
  float ky = kk * (2.0*dot(a,a)+dot(d,b)) / 3.0;
  float kz = kk * dot(d,a);      

  float res = 0.0;

  float p = ky - kx*kx;
  float p3 = p*p*p;
  float q = kx*(2.0*kx*kx - 3.0*ky) + kz;
  float h = q*q + 4.0*p3;

  if(h >= 0.0) 
  { 
    h = sqrt(h);
    vec2 x = (vec2(h, -h) - q) / 2.0;
    vec2 uv = sign(x)*pow(abs(x), vec2(1.0/3.0));
    float t = uv.x + uv.y - kx;
    t = clamp( t, 0.0, 1.0 );

    vec2 qos = d + (c + b*t)*t;
    res = length(qos);
  }
  else
  {
    float z = sqrt(-p);
    float v = acos( q/(p*z*2.0) ) / 3.0;
    float m = cos(v);
    float n = sin(v)*1.732050808;
    vec3 t = vec3(m + m, -n - m, n - m) * z - kx;
    t = clamp( t, 0.0, 1.0 );

    vec2 qos = d + (c + b*t.x)*t.x;
    float dis = dot(qos,qos);
        
    res = dis;

    qos = d + (c + b*t.y)*t.y;
    dis = dot(qos,qos);
    res = min(res,dis);

    qos = d + (c + b*t.z)*t.z;
    dis = dot(qos,qos);
    res = min(res,dis);

    res = sqrt( res );
  }
    
  return res;
}

float
doSeg (vec2 p, vec4 a, vec4 b, float r)
{
  vec2 mid = (a.xy + b.xy)/2.;

  float l = length(b.xy - a.xy)*1.732/6.;

  if(abs(length(b.zw - a.zw))<.01)
    l = r;

  mid += (a.zw + b.zw)/2.*l;

  float b1 = sdBezier(p, a.xy, a.xy + a.zw*l, mid);
  float b2 = sdBezier(p, mid, b.xy + b.zw*l, b.xy);
    
  return min(b1, b2);
}

float
Polygon (vec2 p, float vertices, float radius)
{
  float segmentAngle = TAU/vertices;
     
  float angleRadians = atan(p.x, p.y);
  float repeat = mod(angleRadians, segmentAngle) - segmentAngle/2.;
  float inradius = radius*cos(segmentAngle/2.);
  float circle = length(p);
  return cos(repeat)*circle - inradius;
}

vec2
mouseInversion (vec2 p)
{
  vec2 m = vec2((2.*iMouse.xy - iResolution.xy)/iResolution.y);
  if(length(m) < 1e-3)
    m += 1e-3;
  if(abs(m.x)>.98*.7071 || abs(m.y)>.98*.7071)
    m *= .98;

  float k = 1./dot(m, m);
  vec2 invCtr = k*m; 
  float t = (k - 1.)/dot(p -invCtr, p - invCtr);
  p = t*p + (1. - t)*invCtr;
  p.x = -p.x;

  return p;
}

void
main ()
{
  vec2 uv = (2.*gl_FragCoord.xy - iResolution.xy)/iResolution.y;

#ifdef HALF_PLANE
  const float sc = 4.;
  uv.y += sc/2. + 1.;
  uv /= dot(uv, uv)/sc;
  uv.y -= 1.; 
#else   
  uv *= 1.05;
#endif

  vec2 p = uv;

  p = mouseInversion(p);
  p = rot2(iTime/16.)*p;

  count = 0.;

  vec3 domInfo = initDomain();

  for(int i=0; i<24; i++)
    p = transform(p, domInfo.xy);

  if(length(p)> 1.)
    p /= dot(p, p);

  p /= (domInfo.z);

  float sf = .015;

  float shape = Polygon(p, float(N), .8);

  vec2 vp[N];
  float ln[N]; 

  const int N2 = N*2;
  vec4 mp[N2];
  int shuff[N2];

  float vert = 1e5;
  vec2 v0 = vec2(0, 1);

  for(int i = 0; i<N; i++)
  {
    vert = min(vert, length(p - v0) - .09);
    vp[i] = v0;        
    v0 = rot2(TAU/float(N))*v0;
  }

  vec2 rp = rot2(float(count + 2.)*TAU/float(N))*(p);
  float angl = mod(atan(rp.x, rp.y), TAU)*float(N2)/TAU;
  float triSeg = (floorSm(angl, .1) + .5)/float(N2);
  vec3 oCol = .55 + .45*cos(triSeg*TAU + vec3(0, 1, 2));

  vec3 col = vec3(1, .99, .98);

  col = mix(col, vec3(0), 1. - smoothstep(0., sf, shape));
  col = mix(col, oCol, 1. - smoothstep(0., sf, shape + .04));

  col = mix(col, vec3(0), 1. - smoothstep(0., sf, vert));

  float sLength = length(vp[0] - vp[1]);

  for(int i = 0; i<N; i++)
  {
    shuff[i*2] = i*2;
    shuff[i*2 + 1] = i*2 + 1;

    vec2 mpi = mix(vp[i], vp[(i + 1)%N], .5);
    vec2 tangenti = normalize(mpi - vp[i]);

    mp[i*2].xy = mpi - tangenti*sLength/6.;
    mp[i*2 + 1].xy = mpi + tangenti*sLength/6.;

    mp[i*2].zw = tangenti.yx*vec2(1, -1);
    mp[i*2 + 1].zw = tangenti.yx*vec2(1, -1);
  }

  for(int i = N2 - 1; i>0; i--)
  {
    if(i >= N2)
      continue;

    float fi = float(i);

    float rs = hash21(vec2(count) + domInfo.xy + domInfo.z + fi/float(N2));

    int j = int(floor(rs*(fi + .9999)));
    swap(shuff[i], shuff[j]);
  }

  for(int i = 0; i<N; i++)
  {
    int j = shuff[i*2], jp1 = shuff[i*2 + 1];

    float lnOff = sLength/4.;
    ln[i] = doSeg(p, mp[j], mp[jp1], lnOff) - .105;
  }

  float ringBlend = smoothstep(0., .235, abs(length(uv) - 1.) - .1);
  float pat2 = abs(fract(shape*16.) - .5)*2. - .05;
  col = mix(col, vec3(0), ringBlend*(1. - smoothstep(0., .5, pat2))*.7);

  for(int i = 0; i<N; i++)
  {
    pat2 = abs(fract(ln[i]*16. + .5) - .5)*2. - .05;
    pat2 = mix(1., 0., ringBlend*(1. - smoothstep(0., .5, pat2))*.7);

    vec3 bgC = col;
    col = mix(col, vec3(0), (1. - smoothstep(0., sf*5., ln[i]))*.3);
    col = mix(col, vec3(0), 1. - smoothstep(0., sf, ln[i]));
    col = mix(col, mix(bgC*1.2, vec3(1, .97, .26), .85)*pat2, 1. - smoothstep(0., sf, ln[i] + .04));
    col = mix(col, col*1.3, 1. - smoothstep(0., sf, ln[i] + .085));
  }

  uv*= rot2(iTime/16.);
  float lu = length(uv);
  float ring = abs(lu - 1.) - .05;

  float ang = atan(uv.x, uv.y)/TAU;
  float pat = abs(fract(ang*72.) - .5)*2.;
  pat = smoothstep(0., sf, pat - .125);
   
  col = mix(col, vec3(0), (1. - smoothstep(0., sf*5., ring-.01))*.6);
  col = mix(col, vec3(0), (1. - smoothstep(0., sf/2., ring)));

  col = mix(col, vec3(1, .99, .98)*pat*.35 + .65, 1. - smoothstep(0., .005, ring + .02));
  ring = max(ring + .02, lu - .995);
  col = mix(col, vec3(.4)*pat, (1. - smoothstep(0., sf/3., ring)));

  col = mix(col, min( mix(col, vec3(1)*dot(col, vec3(.299, .587, .114)), 
                 min(lu*.7, 1.))*1.15, 1.),  1. - smoothstep(0., sf, 1.05 - lu));

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1.);
}
