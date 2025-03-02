uniform vec2 u_resolution;
uniform float u_mouseX;
uniform float u_mouseY;
uniform float u_time;
//uniform sampler2D u_tex;

varying vec2 var_vertTexCoord;

vec3
permute (vec3 x)
{
  return mod (((x * 34.0) + 1.0) * x, 289.0);
}

float
snoise (vec2 v)
{
  const vec4 C = vec4 ( 0.211324865405187, 0.366025403784439,
                       -0.577350269189626, 0.024390243902439);
  vec2 i  = floor (v + dot (v, C.yy));
  vec2 x0 = v -    i + dot (i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2 (1.0, 0.0) : vec2 (0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod (i, 289.0);
  vec3 p = permute (permute (i.y + vec3 (0.0, i1.y, 1.0)) + i.x + vec3 (0.0, i1.x, 1.0));
  vec3 m = max (0.5 - vec3 (dot (x0, x0), dot (x12.xy, x12.xy), dot (x12.zw, x12.zw)), 0.0);
  m = m * m;
  m = m * m;
  vec3 x = 2.0 * fract (p * C.www) - 1.0;
  vec3 h = abs (x) - 0.5;
  vec3 ox = floor (x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot (m, g);
}
  
float
random2d (vec2 coord)
{
  return fract (sin (dot (coord.xy, vec2 (1212.9898, 72.233))) * 43758.5453);
}

void
main ()
{
  vec2 coord = var_vertTexCoord.xy / u_resolution.xy;
  vec2 center = 0.5 - vec2 (coord);

  vec3 color = vec3 (0.0, 0.0, 0.0);

  center += snoise (vec2 (u_mouseX * 0.005, u_mouseY * 0.005) * 0.2 + u_time * 0.2 + snoise (center + snoise (vec2 (u_mouseX * 0.001, u_mouseY * 0.001) + u_time * 0.05) * 0.2) * 0.2) * 0.2;
  center += snoise (vec2 (u_mouseX * 0.005, u_mouseY * 0.005) * 0.2 + u_time * 0.3 + snoise (center + snoise (vec2 (u_mouseX * 0.001, u_mouseY * 0.001) + u_time * 0.02) * 0.2) * 0.2) * 0.3;
  color.x += (0.04 + (sin (u_time) * 0.01)) / length (snoise (center) * 0.2 + center + vec2 (u_mouseX * 0.002 - 0.5, -u_mouseY * 0.002 + 0.5 + 0.05)) * 1.0;
  color.y += (0.04 + (sin (u_time) * 0.01)) / length (snoise (center) * 0.3 + center + vec2 (u_mouseX * 0.002 - 0.5, -u_mouseY * 0.002 + 0.5)) * 1.0;
  color.z += (0.04 + (sin (u_time) * 0.01)) / length (snoise (center) * 0.5 + center + vec2 (u_mouseX * 0.002 - 0.5, -u_mouseY * 0.002 + 0.5 - 0.05)) * 1.0;
  //center += snoise(center * 1.0 - u_time * 0.2) * 0.5 ;
  color.x += (0.04 + (sin (u_time) * 0.01)) / length (snoise (center + 0.2) * 0.2 + center + vec2 (u_mouseX * 0.004 - 0.5, -u_mouseY * 0.002 + 0.5 + 0.02)) * 0.7;
  color.y += (0.04 + (sin (u_time) * 0.01)) / length (snoise (center + 0.2) * 0.3 + center + vec2 (u_mouseX * 0.004 - 0.5, -u_mouseY * 0.002 + 0.5)) * 0.7;
  color.z += (0.04 + (sin (u_time) * 0.01)) / length (snoise (center + 0.2) * 0.5 + center + vec2 (u_mouseX * 0.004 - 0.5, -u_mouseY * 0.002 + 0.5 - 0.02)) * 0.7;

  color -= 0.1;

  gl_FragColor = vec4 (color, 1.0);
}
