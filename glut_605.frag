uniform vec2 iResolution;
uniform float iTime;

float
stepping (float t)
{
  return (t<0.) ? -1.+pow(1.+t,2.) : 1.-pow(1.-t,2.);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy*2.-iResolution.xy)/iResolution.y;
  gl_FragColor = vec4(0);
  uv = normalize(uv) * length(uv);
  for(int i=0;i<12;i++)
  {
    float t = iTime + float(i)*3.141592/12.*(5.+1.*stepping(sin(iTime*3.)));
    vec2 p = vec2(cos(t),sin(t));
    p *= cos(iTime + float(i)*3.141592*cos(iTime/8.));
    vec3 col = cos(vec3(0,1,-1)*3.141592*2./3.+3.141925*(iTime/2.+float(i)/5.)) * 0.5 + 0.5;
    gl_FragColor += vec4(0.05/length(uv-p*0.9)*col,1.0);
  }
  gl_FragColor.xyz = pow(gl_FragColor.xyz,vec3(3.));
  gl_FragColor.w = 1.0;
}
