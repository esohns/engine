#version 130

// glut_581_common.glsl
#define R iResolution.xy
#define A(U) texture(iChannel0, (U)/R)
#define B(U) texture(iChannel1, (U)/R)
#define C(U) texture(iChannel2, (U)/R)
#define D(U) texture(iChannel3, (U)/R)

#define S vec4(2,4,6,8)
#define M .1*vec4(4,3,2,1)
#define O .5/S/S
#define I 12.

vec4
hash (float p)
{
	vec4 p4 = fract(vec4(p) * vec4(.1031, .1030, .0973, .1099));
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx)*2.-1.;
}
// glut_581_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel3;

vec4 T (vec2 U) { return A(U-A(U).xy); }

void
main ()
{
  gl_FragColor = T(gl_FragCoord.xy);
  vec4
      n = T(gl_FragCoord.xy+vec2(0,1)),
      e = T(gl_FragCoord.xy+vec2(1,0)),
      s = T(gl_FragCoord.xy-vec2(0,1)),
      w = T(gl_FragCoord.xy-vec2(1,0));

  gl_FragColor.x -= (0.25*(e.z-w.z-gl_FragColor.w*(n.w-s.w)));
  gl_FragColor.y -= (0.25*(n.z-s.z-gl_FragColor.w*(e.w-w.w)));
  gl_FragColor.z += (0.25*((s.y-n.y+w.x-e.x)+(n.z+e.z+s.z+w.z))-gl_FragColor.z);
  gl_FragColor.w += (0.25*(s.x-n.x+w.y-e.y)-gl_FragColor.w);

  n = D(gl_FragCoord.xy+vec2(0,1));
  e = D(gl_FragCoord.xy+vec2(1,0));
  s = D(gl_FragCoord.xy-vec2(0,1));
  w = D(gl_FragCoord.xy-vec2(1,0));

  vec4 b = B(gl_FragCoord.xy);

  vec4 h = hash(b.w);

  vec2 v = vec2(0);
  v += h.x*vec2(e.x-w.x,n.x-s.x);
  v += h.y*vec2(e.y-w.y,n.y-s.y);
  v += h.z*vec2(e.z-w.z,n.z-s.z);
  v += h.w*vec2(e.w-w.w,n.w-s.w);

  gl_FragColor.xy += v*smoothstep(1.,0.,length(gl_FragCoord.xy-b.xy));

  if (fract(.1*iTime)<.2)
    gl_FragColor.xy += .03*smoothstep(20.,0.,length(gl_FragCoord.xy-0.5*R+0.3*R*hash(floor(.1*iTime)).xy))*vec2(cos(floor(.1*iTime)),sin(floor(.1*iTime)));
  if (gl_FragCoord.x<1.||gl_FragCoord.y<1.||R.x-gl_FragCoord.x<1.||R.y-gl_FragCoord.y<1.||iFrame<1)
    gl_FragColor.xyw = vec3(0);
}
