#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define S(x) smoothstep(1., 0., abs(fract(x)-.5)/fwidth(x) )

float D = .5,
      n = 3.;

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  vec2 R = iResolution, 
       M = iMouse.xy / R;
       U = ( U+U - R ) / R.y;

  if (M!=vec2(0)) D = M.y, n = 1./(.01+M.x);
  float l = length(U), c;
  D /= l;
  c =  D<1. ?    pow( 1.-pow(D,n), 1./n )
            : 1./pow( 1.-pow(1./D,n), 1./n ); 
  U *= c; 

  O = texture(iChannel0, .5+.5*U*R.y/R); O *= O;
  U *= 16.;
  O *= 1. - ( S(U.x) + S(U.y) );
  O = mix(vec4(0,1,0,0),O,c);
  O = sqrt(O);

  gl_FragColor = O;
}
