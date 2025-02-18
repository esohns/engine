#version 330
precision highp float;

//#include "glut_486_common.glsl"
#define R iResolution.xy

//a way to sample pixels shorthand 
#define A(U) texture(iChannel0,(U)/R)

//the time step
#define dt 0.9
//#include "glut_486_common.glsl"

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
  gl_FragColor = A (gl_FragCoord.xy);

  vec4 avg = 0.25 * (A (gl_FragCoord.xy + vec2 (0.0, 1.0)) + A (gl_FragCoord.xy - vec2 (0.0, 1.0)) +
                     A (gl_FragCoord.xy + vec2 (1.0, 0.0)) + A (gl_FragCoord.xy - vec2 (1.0, 0.0)));
  float force = avg.x - gl_FragColor.x;

  gl_FragColor.y += force * dt;
  gl_FragColor.x += gl_FragColor.y * dt;

  if (iFrame < 2)
  {
    gl_FragColor.x = exp (-0.01 * dot (gl_FragCoord.xy - 0.4 * R, gl_FragCoord.xy - 0.4 * R)) +
                     exp (-0.01 * dot (gl_FragCoord.xy - 0.6 * R, gl_FragCoord.xy - 0.6 * R));
    gl_FragColor.y = 0.0;
  }
}
