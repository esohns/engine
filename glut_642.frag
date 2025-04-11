uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  vec2 R = iResolution.xy;
  U = ( U+U - R ) /R.y * 8.;  

  float Pi = 3.14159265359,
         t = 16.*iTime,
         e = 35./R.y, v;
  U *= mat2(sin(Pi/3.*ceil(t/2./Pi) + Pi*vec4(.5,1,0,.5)));

  U.y /= .866; 
  U -= .5;   
  v = ceil(U.y);
  U.x += .5*v;
  U.x += sin(t) > 0. ? (1.-cos(t)) * (mod(v,2.)-.5) : 0.;

  U = 2.*fract(U)-1.;
  U.y *= .866;
  O = vec4( smoothstep(e,-e, length(U)-.6) );

  gl_FragColor = O;
}
