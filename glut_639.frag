uniform vec2 iResolution;
uniform float iTime;

#define R fract(1e2*sin(p.x*8.+p.y))

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  vec3 v=vec3(u,1)/vec3(iResolution.xy,1)-.5,
       s=.5/abs(v),
       i=ceil(8e2*(s.z=min(s.y,s.x))*(s.y<s.x?v.xzz:v.zyz)),
       j=fract(i*=.1),
       p=vec3(9,int(iTime*(9.+8.*sin(i-=j).x)),0)+i;
  o-=o,o.g=R/s.z;p*=j;o*=R>.5&&j.x<.6&&j.y<.8?1.:0.;

  gl_FragColor = o;
}
