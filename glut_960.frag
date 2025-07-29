uniform vec2 iResolution;
uniform float iTime;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define Time iTime/2.

void
main ()
{
  vec2 uv = (gl_FragCoord.xy*2.0-iResolution)/iResolution.y;
  vec2 st = uv;
  vec3 col = vec3(0.0);
  uv *= rot(Time);
  for(int i=0;i<3;i++)
  {
    uv = fract(uv*(2.+2.*sin(Time)))-0.5;
    uv *= rot(-Time);
    vec3 col0 = vec3(0.3 , 0.5 , 0.2);
    float d = length(uv)*exp(-length(st));
    col0 = vec3(.1, 1.4 ,.514) + vec3(0.54 ,0.188 ,-.520)*cos(Time);
    d = abs(sin(d*9. + iTime))/9.;
    d = pow(0.01/d,1.2);
    col += (col0 + d)*d;
  }

  gl_FragColor = vec4(col,1.0);
}
