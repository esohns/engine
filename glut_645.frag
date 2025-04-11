uniform vec2 iResolution;
uniform float iTime;

#define l(a)  2.6*(a)*(a)

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  vec2 R = iResolution.xy, V;
  U = 5.* ( U+U - R ) / R.y;
  U = vec2( atan(U.y,U.x)/6.283 +.5, length(U) );
  U.y -= U.x;
  U.x = l( ceil(U.y)+U.x ) - iTime;
  O   = vec4(1.- pow( abs( 2.*fract(U.y)-1.),10.));
  V   = ceil(U); U = fract(U)-.5;
  U.y = dot( U, cos( vec2(-33,0)
                      +  .3*( iTime + V.x )
                      * max( 0., .5 - length(U) )
            )       );
  O *= smoothstep( -1., 1., U/fwidth(U) ).y;

  gl_FragColor = O;
}
