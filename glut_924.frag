uniform vec2 iResolution;
uniform float iTime;

vec4 light;
float ui;
mat2 m,n,nn;

float
map (vec3 p)
{
  float d = length(p-light.xyz)-light.w;
  d = min(d,max(10.-p.z,0.));
  float t = 2.5;
  for (int i = 0; i < 13; i++)
  {
    t = t*0.66;
    p.xy = m*p.xy;
    p.yz = n*p.yz;
    p.zx = nn*p.zx;
    p.xz = abs(p.xz) - t;
  }
  d = min(d,length(p)-1.4*t);

  return d;
}

vec3
norm (vec3 p)
{
  vec2 e = vec2 (.001,0.);
  return normalize(vec3(
    map(p+e.xyy) - map(p-e.xyy),
    map(p+e.yxy) - map(p-e.yxy),
    map(p+e.yyx) - map(p-e.yyx)
  ));
}

vec3
dive (vec3 p, vec3 d)
{
  for (int i = 0; i < 20; i++)
    p += d*map(p);

  return p;
}

void
main ()
{
  vec2 v = gl_FragCoord.xy/iResolution*2.-1.;
  v.x *= iResolution.x/iResolution.y;
  ui = 100.*iTime;
  float y = -0.001*ui;
  m = mat2(sin(y),cos(y),-cos(y),sin(y));
  y = 0.0035*ui;
  n = mat2(sin(y),cos(y),-cos(y),sin(y));
  y = 0.0023*ui;
  nn = mat2(sin(y),cos(y),-cos(y),sin(y));
  vec3 r = vec3(0,0,-15.+2.*sin(0.01*ui));
  light = vec4(10.*sin(0.01*ui),2,-23,1);
  vec3 d = normalize(vec3(v,5.));
  vec3 p = dive(r,d);
  d = normalize(light.xyz-p);
  vec3 no = norm(p);
  vec3 col = vec3(.7,.8,.9);
  vec3 bounce = dive(p+0.01*d,d);
  col = mix(col,vec3(0),dot(no, normalize(light.xyz-p)));
  if (length(bounce-light.xyz) > light.w+0.1) col *= 0.2;

  gl_FragColor = vec4(col,1.0);
}
