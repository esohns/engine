uniform vec2 iResolution;
uniform float iTime;

void main ()
{
  vec4 O;
  vec2 F = gl_FragCoord.xy;
  float i = 0.0;
  vec2 r = iResolution.xy,
       p = ( F+F - r ) / r.y / .7,
       d = vec2(-1,1), q = 5.*p - d, 
       c = p * mat2(1, 1, d/(.1 + 5./dot(q,q)) ),
       v = c * mat2(cos( log(length(c)) +iTime*.2 + vec4(0,33,11,0))) * 5.;

  for (O*=i; i++ < 9.; O += 1. + sin(v.xyyx) )
    v += .7* sin(v.yx*i+iTime) / i + .5;

  i = length( sin(v/.3)*.2 + c*vec2(1,2) ) - 1.;
  O = 1. - exp( -exp( c.x * vec4(.6,-.4,-1,0) ) 
                /  O
                / ( 1. + i*i )
                / ( .5 + 3.5 *exp( .3*c.y - dot(c,c) ) )
                / ( .03 + abs( length(p)-.7 ) )
              );

  gl_FragColor = O;
}
