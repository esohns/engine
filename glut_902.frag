uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define N true
#define rot(a)        mat2(cos(a+vec4(0,11,33,0)))
#define rot3(P,A,a)  ( mix( A*dot(P,A), P, cos(a) ) + sin(a)*cross(P,A) )
  
#define L(q) length(q.xy - vec2(1.2,-1.5*sin(clamp(q.z,-1.57,1.57))) ) -.2
#define d(q)  ( min(min( r= L((q).yzx), N ? g= max(length(q)-1.5,length(q-dot(q,vec3(1./3.))))-.1 : 1e2 ), min( w= L((q).zxy), b= L((q).xyz))))

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  float t=9., w=0.,r=w,b=w,g=w;
  vec3 R = iResolution.xyx,
       D = normalize(vec3(U+U, -12.*R.y) - R),
       p = vec3(0,0,90), q,a,
       M = iMouse.z > 0. ? 6.*iMouse.xyz/R -3.
                         : vec3(-.8,0,0);
  p.yz *= rot(-M.y),
  p.xz *= rot(-M.x), 
  D.yz *= rot(-M.y),
  D.xz *= rot(-M.x); 

  for ( O=vec4(1); O.x > 0. && t > .01; O-=.005 )
      q = p,
      a = vec3(sin(0.),0,cos(0.)),
      a = rot3( a, vec3(.71,0,-.71), -3.14/2. ),
      q = rot3( q,a, -3.14 *smoothstep(.05, .6, 1./length(p)) ),
      a = vec3(sin(iTime),0,cos(iTime)),
      a = rot3( a, vec3(.71,0,-.71), 3.14/2. ),
      q = rot3( q,a,
                3.14 *smoothstep(.05, .6, 1./length(p)) ),
      q = rot3( q,vec3(.71,0,-.71), .6 ),
      t = d(q),
      p += .2*t*D;

  O *= O*O*2.;
  O *= vec4( t==r, t==g, t==b, 1)*vec4(2,1,2,1) + float(t==w);
  if (O.a < 0.) O += .9;

  gl_FragColor = O;
}
