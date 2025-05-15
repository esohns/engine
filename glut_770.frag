uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  o*=0.;
  vec3 R = iResolution.xyx;
  R.z = 0;
  for ( o.z++; R.z++ < 64. ; )
    o += vec4((u+u-R.xy)/R.x,1,0)*(length(vec2(o.a=length(o.xz)-.7,o.y) )-.5);
  o += sin( 21.* ( atan(o.y,o.w) - atan(o.z,o.x) - iTime ) );
  
  gl_FragColor = o;
}
