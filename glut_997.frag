uniform vec2 iResolution;
uniform float iTime;

#define R(p,a,r)mix(a*dot(p,a),p,cos(r))+sin(r)*cross(p,a)
void
main ()
{
  vec4 O=vec4(0);
  vec2 C = gl_FragCoord.xy;
  vec3 r=iResolution.xyy,p;
  for(float i=0.,g=0.,e,l,s;
      ++i<99.;
      e<.003?O.xyz+=mix(
              r/r,
              cos(vec3(8,3,12)+g*(3.+sin(iTime*8.)*.2))*.5+.5,
              .8
          )*.9/i:p
  )
  {
    p=vec3(g*(C-.5*r.xy)/r.y,g-3.5);
    p=R(p,normalize(vec3(1,3,3)),iTime*.2);
    p=abs(p)+.2;
    p.y>p.x?p=p.yxz:p;
    p.z>p.x?p=p.zyx:p;
    p.y>p.z?p=p.xzy:p;
    s=2.;
    for(int j=0;j++<4;)
      s*=l=2./min(dot(p,p),.8),
      p=abs(p)*l-vec3(2,1,3);
    g+=e=length(p.xz)/s;
  }

  O=pow(O,vec4(.8,.9,1.3,1));

  gl_FragColor = O;
}
