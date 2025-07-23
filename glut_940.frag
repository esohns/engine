uniform vec2 iResolution;
uniform float iTime;

float gyroid (vec3 p) { return dot(cos(p),sin(p.yzx)); }

float
fbm (vec3 p)
{
  float result = 0.;
  float a = .5;
  for (float i = 0.; i < 7.; ++i)
  {
    p += result*.1;
    p.z += iTime*.1;
    result += abs(gyroid(p/a)*a);
    a /= 1.7;
  }

  return result;
}

void
main ()
{
  vec2 uv = (2.*gl_FragCoord.xy-iResolution)/iResolution.y;
  vec3 ray = normalize(vec3(uv,.3));

  vec3 e = vec3(0.1*vec2(iResolution.x/iResolution.y), 0.);

#define T(u) fbm(ray+u)
  vec3 normal = normalize(T(0.)-vec3(T(e.xzz),T(e.zyz),1.));
  vec3 color = 0.5 + 0.5 * cos(vec3(1,2,3)*5.4 - normal.x+.5);
  color *= smoothstep(-1.,1.,-normal.z);

  gl_FragColor = vec4(color,1.0);
}
