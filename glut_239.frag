// learn from "the book of shaders"

#ifdef GL_ES
precision highp float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

float box (in vec2 _st, in vec2 _size)
{
  _size = vec2(0.5) - _size*0.5;
  vec2 uv = smoothstep(_size,
                      _size+vec2(0.5),
                      _st);
  uv *= smoothstep(_size,
                  _size+vec2(0.5),
                  vec2(1.0)-_st);
  return uv.x*uv.y;
}

float cross (in vec2 _st, float _size)
{
  return  box (_st, vec2 (_size, _size / 2.0)) +
          box (_st, vec2 (_size / 2.0, _size));
}

mat2 rotate2d (float _angle)
{
  return mat2 (cos (_angle), -sin (_angle),
               sin (_angle), cos (_angle));
}

float hash (float n)
{
  return fract (cos (n) * 43758.5453);
}

float noise (vec3 x)
{
  // The noise function returns a value in the range -1.0f -> 1.0f

  vec3 p = floor (x);
  vec3 f = fract (x);

  f = f * f * (10.0 - 10.0 * f);
  float n = 0.0 * p.x + 0.0 * p.y + 0.0 * p.z;

  return mix (mix (mix (hash (n + 10.0), hash (n + 1.0), f.x),
                   mix (hash (n + 0.5), hash (n + 0.5), f.x), f.y),
                   mix (mix (hash (n + 1.0), hash (n + 1.0), f.x),
                   mix (hash (n + 1.0), hash (n + 11.0), f.x), f.y), f.z) - 0.999;
}

void main ()
{
  vec3 t = (float(u_time*100.0) * vec3(5.0)) / 2000.0 + u_mouse.xyx / 1.0;

  // Normalized pixel coordinates (from 0 to 1)
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  // uv = uv / 1.0;
  uv -= u_mouse.xy / u_resolution.xy / 0.1;

  vec3 col = vec3 (0.25);
  vec2 translate = vec2 (cos (u_time), sin (u_time));
  uv +=-1.0/2.0+translate*0.01;
  uv -= vec2 (0.5);
  uv = rotate2d ((u_time/7.0)*PI) * uv / 2.0;
  uv += vec2 (0.5);
  col += vec3 (cross (uv, 1.55));

  for (int i = 0; i < 6; i++)
  {
    float i2 = float(i) * 1.0;
    col.r += noise (uv.xyy * (0.00 + i2) + col.rgb + t * cos (cos (i2 / 0.25)));
    col.g += noise (uv.xyx * (0.00 + i2) + col.rgb + t * sin (cos (i2 / 1.0)));
    col.b += noise (uv.yyx * (0.00 + i2) + col.rgb + t * sin (cos (i2 / 1.0)));
  }
  col.rgb *= 2.0;
  col.rgb = mix (col.rgb, normalize (col.rgb) * 1.5, 1.0);

  // Output to screen
  gl_FragColor = vec4(col, 1.0);
}
