#version 130

uniform vec2 iResolution;
uniform bvec4 iKeyboard; // space
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define TIMESTEP 0.01

bool
reset ()
{
  return iKeyboard.x; // space
}

#define T(d) n += texture(iChannel0, fract(vUv+d)).xyz;

void
main ()
{
  vec2 vUv = gl_FragCoord.xy / iResolution.xy;
  vec4 t = vec4(1. / iResolution.xy, -1. / iResolution.y, 0.0);
  vec3 p = texture(iChannel0, vUv).xyz;

  vec3 n = vec3(0);
  T(t.wy) T(t.xy) T(t.xw) T(t.xz) T(t.wz) T(-t.xy) T(-t.xw) T(-t.xz)

  vec3 result = p + TIMESTEP * vec3(n.z - n.y, n.x - n.z, n.y - n.x);

  if(p.xyz == vec3(0) || reset())
    gl_FragColor = texture(iChannel1, vUv);
  else
    gl_FragColor = vec4(clamp(result, 0.0, 1.0), 0.0);
}
