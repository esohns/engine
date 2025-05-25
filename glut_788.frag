#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define A(COORD) texture(iChannel0,(COORD)/iResolution.xy)

void
main ()
{
  vec4 Q;
  vec2 U = gl_FragCoord.xy;

  U -= .5 * iResolution.xy;
  U *= .997;
  float a = .003 * sin(.5 * iTime - 2. * length(U - 0.5 * iResolution.xy) / iResolution.y);
  U *= mat2(cos(a), -sin(a), sin(a), cos(a));
  U += .5 * iResolution.xy;
  Q = A(U);

  vec4 pX = A(U + vec2(1,0));
  vec4 pY = A(U + vec2(0,1));
  vec4 nX = A(U - vec2(1,0));
  vec4 nY = A(U - vec2(0,1));
  vec4 m = 0.25 * (pX + nX + pY + nY);
  float b = mix(1., abs(Q.z), .8);
  Q.xyz += (1. - b) * (0.25 * vec3(pX.z - nX.z, pY.z - nY.z, -pX.x + nX.x - pY.y + nY.y) - Q.xyz);

  Q = mix(Q, m, b);

  if (length(Q.xy) > 0.)
    Q.xy = normalize(Q.xy);

  if (iFrame < 1)
    Q = sin(.01 * length(U - 0.5 * iResolution.xy) * vec4(1, 2, 3, 4));

  if (iMouse.z > 0. && length(U - iMouse.xy) < .1 * iResolution.y)
    Q *= 0.;

  gl_FragColor = Q;
}
