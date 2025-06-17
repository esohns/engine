#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

const float pi = 3.1415;
const float pi2 = pi/2.0;

float
random ()
{
  return fract(sin(dot(gl_FragCoord.xy, vec2(12.9898,78.233))) * 43758.5453);  
}

vec4
get_pixel (float x_offset, float y_offset)
{
  return texture(iChannel0, (gl_FragCoord.xy / iResolution.xy) + (vec2(x_offset, y_offset) / iResolution.xy));
}

float
step_simulation ()
{
  float val = get_pixel(0.0, 0.0).r;

  val += random()*val*0.15;

  val = get_pixel(sin(get_pixel(val, 0.0).r  - get_pixel(-val, 0.0) + pi).r  * val * 0.4, 
                  cos(get_pixel(0.0, -val).r - get_pixel(0.0 , val) - pi2).r * val * 0.4).r;
  val *= 1.0001;

  return val;
}

void
main ()
{
  float val = step_simulation();

  if(iFrame == 0)
    val = random() * length(iResolution.xy) / 100.0 + smoothstep(length(iResolution.xy) / 2.0, 0.5, length(iResolution.xy * 0.5 - gl_FragCoord.xy)) * 25.0;
  if (iMouse.z > 0.0) 
    val += smoothstep(length(iResolution.xy) / 10.0, 0.5, length(iMouse.xy - gl_FragCoord.xy));

  gl_FragColor.r = val;
}
