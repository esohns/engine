//precision highp float;

varying vec2 vTexCoord;
uniform vec2 u_resolution;
const float WIDTH = 900.0;
const float HEIGHT = 900.0;
uniform vec3 balls[45];
uniform float lifespan[45];
uniform float mouseX;
uniform float mouseY;
uniform float u_time;

vec3 permute (vec3 x)
{
  return mod (((x*34.0)+1.0)*x, 289.0);
}

float snoise (vec2 v)
{
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
            -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

float random2d (vec2 coord)
{
  return fract(tan(dot(coord.xy, vec2(12.9898, 72.233))) * 43758.5453);
}

void main ()
{
  vec2 coord = vTexCoord.xy; // / u_resolution.xy;
  float x = vTexCoord.x * WIDTH;
  float y = vTexCoord.y * HEIGHT;
  float v = 0.0;
  vec3 color = vec3(0.0);
  coord += snoise(coord + u_time * 0.2 + snoise(coord * 2.0 + u_time * 0.1 + snoise(coord * snoise(coord * 10.0 + u_time * 0.5) * 0.1)) * 0.05) * 0.1;
  coord += snoise(coord * 4.0 + u_time * 0.1 + snoise(coord * 4.0 + u_time + snoise(coord * snoise(coord * 5.0 + u_time * 0.44) * 0.1)) * 0.1) * 0.1;
  coord += snoise(coord * 2.0 + u_time * 0.1 + snoise(coord * 1.0 - u_time * 0.1 + snoise(coord * snoise(coord * 15.0 + u_time * 0.2) * 0.1)) * 0.02) * 0.1;
  coord += snoise(coord * 3.0 - u_time * 0.4 - snoise(coord * 1.0 - u_time * 0.5 + snoise(coord * snoise(coord * 15.0 + u_time * 1.2) * 0.1)) * 0.02) * 0.1;

  color.x += snoise(coord - u_time * 0.3 + 0.55) * 0.2 + 0.2;
  color.y += snoise(coord - u_time * 0.3) * 0.3 + 0.3;
  color.z += snoise(coord - u_time * 0.3 - 0.1) * 0.5 + 0.5;
  color += snoise(coord + snoise(coord + u_time * 0.5) * 0.5 + snoise(coord * 10.0 + u_time) * 0.02) * 0.2;
  color += snoise(coord + snoise(coord + 0.5 - u_time * 0.1) * 0.5 + snoise(coord * 7.0 - u_time) * 0.02) * 0.2;
  color += snoise(coord + snoise(coord + 1.5 + u_time * 0.3) * 0.5 + snoise(coord * 5.0 + u_time) * 0.02) * 0.2;
  color += snoise(coord + snoise(coord * 4.0 + 0.9 - u_time * 0.3) * 0.1 + snoise(coord * 5.0 + u_time + 1.5) * 0.02) * 0.2;
  color += snoise(coord + snoise(coord * 2.0 + 1.9 - u_time * 0.3) * 0.1 + snoise(coord * 5.0 + u_time + 5.5) * 0.02) * 0.2;
  gl_FragColor = vec4(color, 1.0);
}
