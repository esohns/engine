//precision highp float;
#version 330

// float rand(vec2 co){
//   return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
// }

const float PI = 3.14159265359;
uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_frames;

void
main ()
{
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  vec2 mouse = u_mouse.xy / u_resolution.xy;

  float offset = abs (20.0 * sin (u_frames / 500.0));
  float factor = 16.0;
  float bright = 1.0 - factor * mod (distance (mouse, uv), 1.0 / factor);
  bright += offset;
  bright = mod (bright, 1.0);

  //float angle = (PI+ atan(uv.y,uv.x)) / (PI*2.0);
  float angle = (PI + atan (uv.y - mouse.y, uv.x - mouse.x)) / (PI * 2.0);

  gl_FragColor = vec4 (bright, factor * mod (angle + offset * 0.1, 1.0 / factor), 0.0, 1.0);
}
