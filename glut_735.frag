#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define _BufferA iChannel0 

float
GetVelocity(sampler2D _Tex, vec2 p)
{
  return texture(_Tex, p).x;
}

void
main ()
{
  vec4 C;
  vec2 U = gl_FragCoord.xy;
  
  vec2 iScreenSize = iResolution.xy;
  vec3 e = vec3(vec2(1.) / iScreenSize.xy, 0.);
  vec2 q = U / iScreenSize.xy;
  vec4 c = texture(_BufferA, q);
  float p00 = c.y;
  float p0_1 = GetVelocity(_BufferA, q - e.zy);
  float p_10 = GetVelocity(_BufferA, q - e.xz);
  float p10 = GetVelocity(_BufferA, q + e.xz);
  float p01 = GetVelocity(_BufferA, q + e.zy);

  float p11 = GetVelocity(_BufferA, q + e.xy);
  float p1_1 = GetVelocity(_BufferA, q + vec2(e.x, -e.y));
  float p_1_1 = GetVelocity(_BufferA, q - e.xy);
  float p_11 = GetVelocity(_BufferA, q + vec2(-e.x, e.y));

  float d = 0.;
  if (iMouse.z > 0.)
  {
    d = length(iMouse.xy - U);
    d = smoothstep(5.5, 0.5, d);
  }
  d += -p00 + (p0_1 + p_10 + p10 + p01 + (p11 + p1_1 + p_1_1 + p_11)) * .25;
  d *= 0.95;

  C = vec4(d, c.xy, 0.);

  gl_FragColor = C;
}
