#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;
  vec2 v = iResolution.xy;
       u = .2*(u+u-v)/v.y;

  vec4 z = o = vec4(1,2,3,0);

  for (float a = .5, t = iTime, i; 
       ++i < 19.; 
       o += (1. + cos(z+t)) 
         / length((1.+i*dot(v,v)) 
                  * sin(1.5*u/(.5-dot(u,u)) - 9.*u.yx + t))
      )  
    v = cos(++t - 7.*u*pow(a += .03, i)) - 5.*u,
    u += tanh(40. * dot(u *= mat2(cos(i + .02*t - vec4(0,11,33,0)))
                        ,u)
                  * cos(1e2*u.yx + t)) / 2e2
       + .2 * a * u
       + cos(4./exp(dot(o,o)/1e2) + t) / 3e2;

    o = 25.6 / (min(o, 13.) + 164. / o) 
      - dot(u, u) / 250.;
  gl_FragColor = o;
}
