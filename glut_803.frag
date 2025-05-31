uniform vec2 iResolution;
uniform vec4 iDate;

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

	U *= 12./iResolution.y;
  O *= 0.;
  vec2 f = floor(U),
       u = 2.*fract(U)-1.;
  float b = mod(f.x+f.y,2.), y;

  for(int i=0; i<4; i++) 
    u *= mat2(0,-1,1,0),
    y = 2.*fract(.2*iDate.w+U.x*.01)-1.,
    O += smoothstep(.55,.45, length(u-vec2(.5,1.5*y)));

  if (b>0.) O = 1.-O;

  gl_FragColor = O;
}
