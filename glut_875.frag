#version 130

uniform vec2 iResolution;
uniform float iTime;

float
segment (vec2 p, vec2 a, vec2 b)
{
  p -= a;
  b -= a;
  return length(p - b * clamp(dot(p, b) / dot(b, b), 0., 1.));
}

#define rot(a) mat2(cos(a+vec4(0.,1.57,-1.57,0.)))

float t;
vec2
T (vec3 p)
{
  p.xy *= rot(-t);
  p.xz *= rot(.785);
  p.yz *= rot(-.625);
  return p.xy;
}

void
main ()
{
  vec4 O;
  vec2 u = gl_FragCoord.xy;
  
  vec2 R = iResolution, X,
       U = 10. * u / R.y,
       M = vec2(2,2.3),
       I = floor(U/M)*M, J;  
  U = mod(U, M);
  O *= 0.;
  for (int k = 0; k<4; k++)
  {
    X = vec2(k%2,k/2)*M;
    J = I+X; 
    if ( int(J/M)%2 > 0 )
      X.y += 1.15;
    t = tanh( -.2*(J.x+J.y) + mod(2.*iTime,10.) -1.6 )*.785; 
    for (float a = 0.; a < 6.; a += 1.57)
    {
      vec3 A = vec3(cos(a),sin(a),.7), 
           B = vec3(-A.y,A.x,.7);
#define L(A,B) O += smoothstep(15./R.y, 0., segment( U-X, T(A), T(B) ) )
      L(A,B);
      L(A,A*vec3(1,1,-1));
      A.z=-A.z; B.z=-B.z; L(A,B);
    }
  }

  gl_FragColor = O;
}
