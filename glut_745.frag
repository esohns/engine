#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

vec2 R;
vec4
T (vec2 U)
{
  return texture(iChannel0, U / R);
}

float
X (vec2 U, vec2 u, inout vec4 Q, vec2 r)
{
  vec2 v = T(U + r).xy;
  float P = T(U + r - v).z;
  Q.xy -= 0.25 * r * (P - Q.z);
  return (0.5 * (length(r - v + u) - length(r + v - u)) + P);
}

void
main ()
{
  vec4 Q;
  vec2 U = gl_FragCoord.xy;
  
  R = iResolution.xy;
  vec2 u = T(U).xy, e = vec2(1, 0);
  float P = 0.;
  Q = T(U - u);
  Q.z = 0.25 * (
     X(U, u, Q, e) +
 	   X(U, u, Q, -e) +
 	   X(U, u, Q, e.yx) +
 	   X(U, u, Q, -e.yx));

  if (iFrame < 1)
    Q = vec4(0);

  if (U.x < 1. || U.y < 1. || R.x - U.x < 1. || R.y - U.y < 1.)
    Q.xy *= 0.;

  if (length(U - vec2(0.1, 0.5) * R) < 9.) 
    Q.xy = Q.xy * .9 + .1 * vec2(1.5, 0.);
  if (length(U - vec2(0.7, 0.3) * R) < 9.) 
    Q.xy = Q.xy * .9 + .1 * vec2(-1.75, .7);

  gl_FragColor = Q;
}
