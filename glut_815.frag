uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform bvec4 iKeyboard; // a, i, p, s
uniform bvec4 iKeyboard2; // m, b, r, t

bool doAO = false;
bool doInversion = true;
bool doPlane = false;
bool doSoftShadow = false;
bool doMorph = true;
bool doPulse = false;
bool doRotate = true;
bool doTranslate = true;

const float PI = 3.141592654;

float
apollonian (vec3 p, float r2, out vec4 orb)
{
  float scale = 1.0;
  orb = vec4(1000); 
  for( int i=0; i < 10;i++ )
  {
    p -= 2.0*round(0.5*p);
    float p2 = dot(p,p);
    orb = min( orb, vec4(abs(p),p2) );
    float k = r2/p2;
    p *= k;
    scale *= k;
  }

  float res;
  if (doPlane)
    res = abs(p.y + float(doPulse)*0.1*sin(iTime));
  else
    res = min(abs(p.z)+abs(p.x), 
              min(abs(p.x)+abs(p.y),
                  abs(p.y)+abs(p.z)));
  return res/scale;
}

float
map (vec3 p, float r2, out vec4 orb)
{
  vec3 q=p;
  float s = 1.0;
  if (doInversion)
  {
    s = 4.0/dot(p,p);
    p *= s;
    p += vec3(1.0);
  }
  if (doTranslate)
    p.y += 0.1*iTime;
  return 0.25*apollonian(p,r2,orb)/s;
}

float
map (vec3 p, float r2)
{
  vec4 orb;
  return map(p,r2,orb);
}

vec3
get_normal (vec3 p, float r2)
{
  const vec2 e = vec2(0.002, 0);
  return normalize(vec3(map(p + e.xyy,r2)-map(p - e.xyy,r2), 
                        map(p + e.yxy,r2)-map(p - e.yxy,r2),	
                        map(p + e.yyx,r2)-map(p - e.yyx,r2)));
}

float
intersect (vec3 ro, vec3 rd, float r2)
{
  float pixel_size = 1.0/(iResolution.y * 2.0);

  float t = 1.0;
  for( int i=0; i<128; i++ )
  {
    float c = map(ro + rd*t, r2);
    if( c<0.5*pixel_size*t )
      break;
    t += c;
    if( t>100.0 )
      return -1.0;
  }
  return t;
}

float
softshadow (vec3 ro, vec3 rd, float r2, float mint, float k)
{
  vec4 orb;
  float res = 1.0;
  float t = mint;
  float h = 1.0;
  for( int i=0; i<100; i++ )
  {
    h = map(ro + rd*t,r2,orb);
    res = min( res, k*h/t );
    if( res<0.001 )
      break;
    t += clamp( h, 0.01, 0.05 );
  }
  return clamp(res,0.0,1.0);
}

vec3
tonemap (vec3 x)
{
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return (x * (a * x + b)) / (x * (c * x + d) + e);
}

float
calcAO (vec3 pos, vec3 nor, float r2)
{
  float dd, hr, totao = 0.0;
  float sca = 1.0;
  vec3 aopos; 
  for( int aoi=0; aoi<5; aoi++ )
  {
    hr = 0.01 + 0.05*float(aoi);
    aopos =  nor * hr + pos;
    totao += -(map( aopos,r2)-hr)*sca;
    sca *= 0.75;
  }
  return clamp( 1.0 - 4.0*totao, 0.0, 1.0 );
}

vec2
rotate (vec2 p, float t)
{
  return p * cos(-t) + vec2(p.y, -p.x) * sin(-t);
}

vec3
transform (vec3 p)
{
  if (iMouse.x > 0.0)
  {
    float theta = -(2.0*iMouse.y-iResolution.y)/iResolution.y*PI;
    float phi = -(2.0*iMouse.x-iResolution.x)/iResolution.y*PI;
    p.yz = rotate(p.yz,phi);
    p.zx = rotate(p.zx,phi);
  }
  if (doRotate)
    p.zx = rotate(p.zx,0.1*iTime);
  return p;
}

void
main ()
{
  doAO = iKeyboard.x; // 'a'
  doInversion = !iKeyboard.y; // 'i'
  doPlane = iKeyboard.z; // 'p'
  doSoftShadow = iKeyboard.w; // 's'
  doMorph = !iKeyboard2.x; // 'm'
  doPulse = iKeyboard2.y; // 'b'
  doRotate = !iKeyboard2.z; // 'r'
  doTranslate = !iKeyboard2.w; // 't'

  float K = 0.2;
  if (doMorph)
    K *= cos(0.123*iTime);
  float r2 = 1.0+K;

  vec3 ld0=normalize(vec3(-12., 2.0, -7.));

  vec2 q = gl_FragCoord.xy / iResolution.xy;
  vec2 p = -1.0 + 2.0 * q;
  p.x *= iResolution.x/iResolution.y;
  
  vec3 lookat = vec3(0);
  vec3 ro = 1.4*vec3(-1., 1.5,-4.5);
  if (iMouse.y > 0.0)
    ro *= 2.0*iMouse.y/iResolution.y;
  vec3 forward=normalize(lookat-ro);
  vec3 right=normalize(cross(forward, vec3(0.0, 1.0, 0.0)));
  vec3 up=normalize(cross(right, forward));

  vec3 rd=normalize(p.x*right + p.y*up + 2.*forward);

  ro = transform(ro);
  rd = transform(rd);
  ld0 = transform(ld0);
  float t=intersect(ro,rd,r2);
  vec3 col=vec3(0.);
  if(t>-0.5)
  {
    vec3 pos=ro+t*rd;
    vec3 n=get_normal(pos,r2);
    vec4 orb;
    map(pos,r2,orb); // Just for orb
    float ao = pow(clamp(orb.w*2.0,0.0,1.0), 1.2);
    if (doAO) ao = calcAO(pos,n,r2);
    float c0=pow(clamp(orb.w, 0.0, 1.0), 2.);
    float c1=pow(clamp(orb.x, 0.0, 1.0), 2.);
    vec3 col0=c0*vec3(0.0,1.0,1.0);
    vec3 col1=vec3(1.);
    col1 = mix(col1, vec3(0.4,0.0,0.0), clamp(3.5*orb.y,0.,1.));

    float amb=0.5+0.5*n.y;
    float diff=max(0.0, dot(n,ld0));
    float bac=max(0.0,dot(n,-ld0));
    float bac2=max(0.0, dot(n, -vec3(0.,1.,0.)));
    float spec=max(0.0, pow(clamp(dot(ld0, reflect(rd, n)), 0.0, 1.0), 64.0));
    float sha=1.0;
    if (doSoftShadow)
      sha = softshadow( pos+0.01*n, ld0,r2,0.005, 32.0 );
    vec3 lin=vec3(0.0);

    lin+=vec3(0.5)*amb*ao;
    lin+=vec3(5.,5.,3.)*diff*1.0*ao*sha;
    lin+=vec3(1.0,0.8,0.3)*bac*1.0*ao;
    lin+=vec3(1.,0.8,0.3)*bac2*ao;
    lin+=vec3(spec*6.2)*sha;

    col=(lin*col1-col0)*.2;
    col=mix(col,vec3(0.), 1.0-exp(-0.001*t*t)); 
    col *= min(2000.0*exp(-0.74*t),1.0);
  }
  col=tonemap(col);
  col=pow(clamp(col,0.0,1.0),vec3(0.45));
  col*=pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1);
  gl_FragColor = vec4(col, 1.0);
}
