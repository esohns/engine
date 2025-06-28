uniform vec2 iResolution;
uniform float iTime;

vec3
hsv (float h, float s, float v)
{
  vec4 t = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
  vec3 p = abs(fract(vec3(h) + t.xyz) * 6.0 - vec3(t.w));
  return v * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), s);
}

mat3
rotate3D (float angle, vec3 axis)
{
  axis = normalize(axis);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;
  
  return mat3(
    oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c
  );
}

void
main ()
{
  vec2 r = iResolution;
  vec2 FC = gl_FragCoord.xy;

  float t = iTime;

  vec4 o = vec4(0, 0, 0, 1);

  for(float i=0.,g=0.,e=0.,s=0.;++i<18.;)
  {
    vec3 p=vec3((FC-.5*r)/r.y*3.5,g+.5)*rotate3D(t*.5,vec3(1,1,0));

    s=1.;

    for(int i=0;i++<40;p=vec3(0,3.01,3)-abs(abs(p)*e-vec3(2.2,3,3)))
      s*=e=max(1.,10./dot(p,p));

    g-=mod(length(p.yy-p.xy*.3),p.y)/s*.4;

    o.rgb+=hsv(.08,.8+.3*p.x,s/4e3);
  }

  gl_FragColor = o;
}
