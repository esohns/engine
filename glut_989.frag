uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define m0 .0001
#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))

vec4 clr;
int obj;
ivec3 mri;

void
bk (out vec4 O, vec2 p)
{
	vec2 R = iResolution;
	float c = 1.5 - length(p = (p + p - R) / R.y * .7),
	      f, i = 0., s = 1.;
  while(i++<8.)
    s /= f = dot(p,p),
    p = -p.yx / f,
    p.x=fract(p.x)-.5,
    f = min( 9., (length(p) - .15) / s * 226. );
  O=.4+vec4(clamp(f,0.,1.)-.5)*.1*(mri.x==2?-1.:1.);
  if(mri.z==15)
    O=clamp(1.-abs(.4-O)*25.,0.,1.)+O;
  O=clamp(O,0.,1.);
}

float
dlian (vec3 p)
{
  p.x = p.x / 3.1415926;
  p.yz *= rot(3.14159265/4.);
  float r = .05, R=.15, a, b, sz=3.;
  p.yz /= sz;
  if(r>.25) r=.25;
  if(R>.25-r) R=.25-r;

  vec4 o,x;
  o.y=.5;
  x = p.x+o - round(p.x+o);
  x = max(abs(x) - (.25-R+r), 0.);
  x.zw=p.yz;

  a = length(vec2(length(x.xz)-R, x.w))-r,
  b = length(vec2(length(x.yw)-R, x.z))-r;

  return min(a,b)*sz;
}

vec3
braid3 (vec3 p)
{
  vec3 y = p.y - .6*cos(    p.x+vec3(0,2,4)),
       z = p.z - .1*cos(p.x+p.x+vec3(1,5,3)),
       r = y*y+z*z, 
       e = vec3(1),
       a;

  r=step(r,r.yzx)*step(r,r.zxy);

  a = vec3(dot(e, y*r),
           dot(e, z*r),
           p.x);

  return p.yzx-a;
}

float
braidx (vec3 p)
{
  vec3 v,q=p;
  float i=0.,n=2., a=2.;
  if(mri.x==2) n=3.;
  for(;i++ < n;)
  {
    v=braid3(q);
    q.yz -= v.xy;
    q *= a;
  }

  return (length(q.yz)-.5)/(a*++n);
}

float
txb (vec3 a, vec3 b, vec3 r, float x)
{
  vec2 u = vec2(a.x, b.x);
  float m = r.x, y;
  if (r.y < m) m = r.y, u = vec2(a.y, b.y);
  if (r.z < m) m = r.z, u = vec2(a.z, b.z);
  y = atan(u.y, u.x)/6.28+.5;
  y = y * 20.*1.;
  x = x * 20.*1.;

  return 1.-((cos(x)*cos(y))/2.+.5)* 1.;
}

float
braid (vec3 p)
{
  if(mri.y > 1) return braidx(p);
  if(mri.x == 1) return dlian(p);

  vec3 y = p.y - .6*cos(    p.x+vec3(0,2,4)),
       z = p.z - .1*cos(p.x+p.x+vec3(1,5,3)),
       r = sqrt(y*y+z*z)-.2;

  return min(r.x, min(r.y, r.z)) * (mri.x>0 && mri.x<3 ? txb(y,z,r,p.x) : 1. );
}

float
sd12 (vec3 p)  
{
  obj = -1;
  float rx=length(p)-1.25;
  if(rx>.1) return rx;
  vec3 a = normalize(sqrt(vec3(.5-.1*sqrt(5.), 0., .5+.1*sqrt(5.)))),
       b = a.yzx, 
       c = a.zxy,
       e = normalize(vec3(1)),  
       ac = normalize(a-c),
       ba = normalize(b-a),         
       n = normalize(cross(c-b, b+c));

  float sg = 1., x, y, ang,r = .1, d, d1, d2, d3;

  for(int i = 0;i++<9;n=n.yzx)
        if(dot(p, n)<0.) p = reflect(p, n), sg = -sg;
  n=ba; if(dot(p, n)<0.) p = reflect(p, n), sg = -sg;
  n=ac; if(dot(p, n)<0.) p = reflect(p, n), sg = -sg;
  n=ba; if(dot(p, n)<0.) p = reflect(p, n), sg = -sg;

  float aa=2.,  bb=1.08, md;
  if (mri.y%2<1) aa=.97, bb=3.;
  md = max(dot(p-e*aa,e),  length(p) -bb*length(a));
  obj=3;
  md += length(p*15.*sg-round(p*15.*sg))*(.02-0.);
  if(md<m0) return md;

  for(int i = 0;i++<2;p=p.zxy)
  {
    vec3 az = c,
         ay = ba,
         ax = normalize(cross(az, ay)),
         w = vec3(dot(p, ax), dot(p, ay),dot(p, az));

    ang = atan(w.y, w.x);
    y = w.z-.8;
    x = length(w.xy);

    x = x - .7 - pow(2., .5+.5*sin(sg * ang * 5.+.4))*r;

    d = sqrt(x*x+y*y)-r*1.3;
    if(mri.x>2) if(abs(d)<md) md=abs(d), obj = 1;
    if (d<.1)
    {
      vec3 c3=(a+b+c)/3.;
      vec2 n=normalize(abs(vec2(length(c-c3), length(c3)))),
      ct = vec2(x,y) * mat2(n.x, -n.y, n.y, n.x);                                                
      d1 =braid(vec3(ang*sg*10.+iTime*7.2368, ct/r))*r;
      if(d1<md)md=d1, obj=2;  
    }
    else
      md = min(md, d+.1);
  }

  return md; 
}

float
map (vec3 p)
{
  float t = (iTime+6.)*.15;
  p.xy *= rot(t   );
  p.yz *= rot(t*.7);
  p.zx *= rot(t*.3);

  return sd12(p);
}

float
softshadow (vec3 ro, vec3 rd)
{
  float res = 1.;
  float t=.05, k = 1.5;
  for(int i =0;i<126 && t<.5;i++)
  {
    float h = map(ro + rd * t);
    if(obj<2) h=999.;
    if(h<.003)
      return 0.;
    res = min(res, k*h/t);               
    t += h;
  }

  return (res+.3)/1.3;
}

void
drawms (inout vec4 O, vec2 U, vec2 M)
{
  float r = .2, p = .1, d, md=1e8;
  bool inr = false;
  vec2 R = iResolution,
       k = R/R.y,
       u = (U+U-R)/R.y,v,
       m = (M+M-R)/R.y,
       c = (vec2(1,-1)*(1.-(r+p)/k ))*k;

  md = abs(d=length(u-c)-r);
  if(length(m-c)<r)
  {
    v = abs((u/k*2.-round(u/k*2.)) /2.*k);
    md = min(md, min(v.x,v.y));
    if(d<0.)O.x+=.4;
    mri *= -1;
    inr=true;
  }

  if(d<0.) O.y+=.4;
  if(inr == true) O +=  1e2*exp(-1e3 * md);
  else            O.x  += exp(-200. * md);
}

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  mri = ivec3(iMouse.xy / iResolution * 4., 0.);
  mri.z = mri.y * 4 + mri.x;
  drawms(O,U,iMouse.xy);
  O = vec4(.5); clr++;

  vec2 R = iResolution,
       u = (U+U - R) / R.y;
  vec3 eye = vec3(0, 0, -3),
       dir = normalize(vec3(u, 2)),
       sun = 5. * ( .12*cos((iTime*0.+11.)*1.+ 0.+vec3(7,11,13)) + vec3(-1,2,-3)/3. ),
       eps = vec3(0, .0001, 0),
       nor, p, 
       sp, ep;
  float d , t, ccc, ln, er, sha=1., sha2=1., lnu = length(u);
  bool ht = false;
  if(lnu>.8) bk(O, U);
  if(lnu<1.)
  {
    for(int i = 0; i++<128 && t<4.;)
    {
      p = eye + dir * t;
      d = map(p);
      if( d<m0 && obj>1)
      {
        sp = normalize(sun-p), ep=normalize(eye-p);
        sha = softshadow(p-dir*d, sp);

        nor = normalize(vec3(map(p+eps.yxx), map(p+eps), map(p+eps.xxy))-d); 
        ln = max(0., dot(nor,sp)),
        er = max(0., dot(ep, reflect(-sp,nor)));

        float ambt=.3, difu=.6, spec=80.;
        vec4  lightClr = vec4(er*er,ln*.9, .55, 1);
        if(obj==3)lightClr=vec4(1.5,ln*.75, er*.75, 1)*1.+.3;
        O = lightClr *sha*(ambt + (difu*ln + pow(er, spec))); O *= clr;                        
        if(mri.z==5 && obj==3) O=sqrt(O);
        if(mri.z==1)O=sqrt(O);
        ht=true;
        break;
      }

      if(d<m0 && obj==1) 
        ccc+=.1-ln*.1, sha2=sha;
      t += max(d * .8, m0);
    }
    O.yz+=ccc*.2*sha2;
  }
  if(mri.z== 9)O*=ln*0.+sha+0.*er+.1,O=pow(O,O)*.99;
  if(mri.z==14)O=sqrt(O);
  if(mri.z==13 && ht==true)O=pow(vec4(sha),1.-O);

  if(mri.z==0) O = pow(max(O,0.), vec4(.71));
  drawms(O,U,iMouse.xy);

  gl_FragColor = O;
}
