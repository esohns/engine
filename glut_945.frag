#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform bvec4 iKeyboard; // p

#define A(v) mat2(cos((v*3.1416) + vec4(0, -1.5708, 1.5708, 0)))
#define s(a, b) c = max(c, .006/abs(L( u, K(a, v, h), K(b, v, h) )+.02));

float
L (vec2 p, vec3 A, vec3 B)
{
  vec2 a = A.xy, 
       b = B.xy - a;
       p -= a;
  float h = clamp(dot(p, b) / dot(b, b), 0., 1.);

  return length(p - b*h) + .01*mix(A.z, B.z, h);
}

vec3
K (vec3 p, mat2 v, mat2 h)
{
  p.zy *= v;
  p.zx *= h;
  if (iKeyboard.x)
    p *= 4. / (p.z+4.);

  return p;
}

void
main ()
{
  vec4 C = vec4(0);
  vec2 U = gl_FragCoord.xy;

  vec2 R = iResolution,
       u = (U+U-R)/R.y*3.,
       m = (iMouse.xy*2.-R)/R.y;

  float t = iTime/120.,
        a = 1.618;
    
  if (iMouse.z < 1.)
    m = vec2(sin(t*6.2832)*2., sin(t*6.2832*2.)*.7);

  mat2 v = A(m.y),
       h = A(m.x);

  vec3 c = vec3(0);

  s( vec3(-1,  a,  0), vec3( 0, -1, -a) )
  s( vec3(-1,  a,  0), vec3( 0, -1,  a) )
  s( vec3(-1,  a,  0), vec3( a,  0, -1) )
  s( vec3(-1,  a,  0), vec3( a,  0,  1) )
  s( vec3( 1,  a,  0), vec3( 1, -a,  0) )
  s( vec3( 1,  a,  0), vec3( 0, -1, -a) )
  s( vec3( 1,  a,  0), vec3( 0, -1,  a) )
  s( vec3( 1,  a,  0), vec3(-a,  0, -1) )
  s( vec3( 1,  a,  0), vec3(-a,  0,  1) )
  s( vec3(-1, -a,  0), vec3(-1,  a,  0) )
  s( vec3(-1, -a,  0), vec3( 0,  1, -a) )
  s( vec3(-1, -a,  0), vec3( 0,  1,  a) )
  s( vec3(-1, -a,  0), vec3( a,  0, -1) )
  s( vec3(-1, -a,  0), vec3( a,  0,  1) )
  s( vec3( 1, -a,  0), vec3( 0,  1, -a) )
  s( vec3( 1, -a,  0), vec3( 0,  1,  a) )
  s( vec3( 1, -a,  0), vec3(-a,  0, -1) )
  s( vec3( 1, -a,  0), vec3(-a,  0,  1) )
  s( vec3( 0,  1, -a), vec3( 0,  1,  a) )
  s( vec3( 0,  1, -a), vec3( a,  0,  1) )
  s( vec3( 0,  1, -a), vec3(-a,  0,  1) )
  s( vec3( 0, -1, -a), vec3( 0, -1,  a) )
  s( vec3( 0, -1, -a), vec3( a,  0,  1) )
  s( vec3( 0, -1, -a), vec3(-a,  0,  1) )
  s( vec3(-a,  0, -1), vec3( a,  0, -1) )
  s( vec3(-a,  0,  1), vec3( a,  0,  1) )
  s( vec3(-a,  0, -1), vec3( 0,  1,  a) )
  s( vec3(-a,  0, -1), vec3( 0, -1,  a) )
  s( vec3( a,  0, -1), vec3( 0,  1,  a) )
  s( vec3( a,  0, -1), vec3( 0, -1,  a) )

  C = vec4(min(c, 1.), 1);

  gl_FragColor = C;
}
