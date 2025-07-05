#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define T texture(iChannel0, p

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  vec2 r = iResolution,
       p = ( I+I - r ) / r.y,
       R;
  p *= 1. - .5 / vec2( 1./p.y  , 1. + p*p );

  float t = iTime, 
        F = R.x = 11.;
  for( p.y -= t ; F < 50.; F *= 1.2 )
    p += .4* sin( F* dot( p, sin(++R) ) + 6.*t ) * cos(R)/F;

  F = T/8.).r;
  t = length(p+vec2(0,t+.5)) / .8 - F;
  F += T/4.).g;

  O = tanh( F / ( .2 - .1*F 
                  + t*t * (t < 0. ? 5. : 50. )
                ) / vec4(1,3,9,1)
          );

  gl_FragColor = O;
}
