#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 C = gl_FragCoord.xy;
  
  float i=0.,d,z,k,c,t=iTime;
  vec4 o,p;
  for (mat2 R = mat2(cos(.2*t+vec4(0,11,33,0))); ++i<80.; z += .7*d+1e-3)
    p = vec4(z*normalize(vec3(C+C,0)-iResolution.xyy),0)
  , p.z += 3.5
  , p.xw *= R
  , p.wy *= R
  , p.zw *= R
  , c = length(p)-3.
  , p *= k = 9./dot(p,p)
  , p += .5*t
  , p -= round(p)
  , p *= p
  , p *= p
  , d = max(abs(pow(dot(p,p),.125)-.4)/k, c)
  , o += (p = 1. + sin(vec4(5,6,7,5)+2.*log2(k))).w/max(d, 1e-3)*p;

  O = tanh(o/2e4);

  gl_FragColor = O;
}
