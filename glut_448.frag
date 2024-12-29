//precision mediump float;

uniform vec2 u_resolution;
uniform float u_time;

#define PI 3.14159265358979323846

/*float
random (vec2 _st)
{
  return fract (sin (dot (_st.xy, vec2 (12.9898, 78.233))) * 43758.5453123);
}*/

vec2
rotate2D (vec2 _st, float _angle)
{
  _st -= 1.0;
  _st =  mat2 (cos (_angle),-sin (_angle),
               sin (_angle), cos (_angle)) * _st;
  _st += 0.25;
  return _st;
}

vec2
tile (vec2 _st, float _zoom)
{
  _st *= _zoom;
  return fract (_st);
}

vec2
rotateTilePattern (vec2 _st)
{
  _st *= 4.0;

  float index = 1.0;
  index += step (1.0, mod (_st.x, 12.0)) * 2.0;
  index += step (1.0, mod (_st.y, 1.0)) * 1.0;

  _st = floor (_st);

  if (index == 1.0)
    _st = rotate2D (_st, PI * 0.25);
  else if (index == 2.0)
    _st = rotate2D (_st, PI * -1.0);
  else if (index == 3.0)
    _st = rotate2D (_st, PI * 0.5);
  else if (index == 4.0)
    _st = rotate2D (_st, PI * -0.5);

  return _st;
}

void
main ()
{
  vec2 st = gl_FragCoord.xy / u_resolution.xy;
  // st.y *= u_resolution.x / u_resolution.y;
  st.x *= u_resolution.x / u_resolution.y;
  st = tile (st, 1.0);
  st *= rotateTilePattern (st);

  st = tile (st, 12.0);
  st = rotate2D (1.0 - st, PI * u_time * 0.1);
  st = rotateTilePattern (1.0 - st * 2.0);
  st /= rotate2D (st, -PI * u_time * 0.0);

  gl_FragColor = vec4 (vec3 (step (st.x, st.y), step (st.x, st.y), step (st.x, st.y)), 1.0);
}
