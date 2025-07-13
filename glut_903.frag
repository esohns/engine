uniform vec2 iResolution;
uniform float iTime;

void
main (  )
{
  vec4 o;
  vec2 C = gl_FragCoord.xy;
  
  o=vec4(0);
  vec3 r=iResolution.xyx,q=vec3(0,0,1e4),v=vec3(C,0).gbr-r.yxx*.3,p;
  for(float i,s,d;i++<53.;d=.2*iTime)
  {
    for(p=q,s=6e2;.4<s;p=p.zxy-s*sin(p/s)*.3,s*=d=.8)p.yz*=mat2(cos(d-vec4(0,33,11,0)));
    d=length(vec2(length(p.yz)-9e3,p.x))-3e3;
    v.x*=7e2<q.x?.1+sin(C.y*3.14):1.;
    q+=sin(i)+v/r.x*d;
    o+=exp(-d*d/vec4(3,2,1,1))/i;
  }

  gl_FragColor = o;
}
