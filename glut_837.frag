uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define rot(a) mat2(cos(a+vec4(0,11,33,0)))

float
map (vec3 q)
{
  vec3 a = abs(q);
  return min( length(q)-1.2, max(a.x,max(a.y,a.z)) - 1. );
}

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  float t=9.,h, T=3.-iTime; 

  vec2 R = iResolution;
  vec3 D = normalize(vec3((U+U-R)/R.y, -10.)),
       p = vec3(0.,0.,30.), q,r,a,
       M = iMouse.z > 0. ? vec3 (iMouse.xy/R -.5, 0.) * vec3(1.,.3,0.)
                         : vec3(-.3,0.,0.) +vec3(1.,1.,0.)/1e2 *cos(.5*iTime+vec3(0.,11.,0.)); 

  for (O=vec4(1); O.x > 0. && t > .005; O-=.005)
    q = p,
    q.yz *= rot(.5-6.3*M.y),
    q.xz *= rot(-6.3*M.x),
    q.x -= 1.274*T,

    r = q, r.x = mod(r.x,2.)-1., r.y+=.1,
    t = min(9., max(-map(r), q.y+1. ) ),
    q.xy -= 1.4 * cos( mod(T,1.57)+.785 + vec2(0.,11.) ),
    q.x += 2.*floor(T/1.57),
    q.y++,
    q.xy *= rot(T),
    t = min(t, map(q)),
    p += .2*t*D;

  gl_FragColor = O;
}
