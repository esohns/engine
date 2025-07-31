#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel3;

vec2 R;

float
ln (vec2 p, vec2 a, vec2 b)
{
  return length(p-a-(b-a)*clamp(dot(p-a,b-a)/dot(b-a,b-a),0.,1.));
}

vec4 A (vec2 U) { return texture(iChannel0,U/R); }
vec4 B (vec2 U) { return texture(iChannel1,U/R); }
vec4 T (vec2 U) { return A(B(U).xy); }

void
main ()
{
  vec4 C;
  vec2 U = gl_FragCoord.xy;

  R = iResolution.xy;
  C = T(U);
  vec4 n = T(U+vec2(0,1)),
       e = T(U+vec2(1,0)),
       s = T(U-vec2(0,1)),
       w = T(U-vec2(1,0));
  C.x -= 0.25*(e.z-w.z+(n.w*C.w-s.w*C.w));
  C.y -= 0.25*(n.z-s.z+(e.w*C.w-w.w*C.w));
  C.z  = 0.25*((s.y-n.y+w.x-e.x)+(n.z+e.z+s.z+w.z));
  C.w  = 0.25*((s.x-n.x+w.y-e.y)-(n.w+e.w+s.w+w.w));

  C.xy += exp(-length(U.xy-0.5*R))*(0.9*vec2(sin(0.2*iTime),cos(0.2*iTime))-C.xy);
  if (U.x < 1.||R.x-U.x<1.)C.xy*=0.;
  if (U.y < 1.||R.y-U.y<1.)C.xy*=0.;
  if (iFrame < 1) C = vec4(0);
  vec4 mo = texture(iChannel3,vec2(0));
  float l = ln(U,mo.xy,mo.zw);
  if (mo.z > 0. && l < 2.) C += vec4(3.*(2.-l)*(mo.xy-mo.zw)/R.y,0,0);

  gl_FragColor = C;
}
