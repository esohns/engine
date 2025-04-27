#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

float
fbm (vec2 at)
{
  float s = sin(0.45);
  float c = cos(0.45);
  mat2 r = mat2(vec2(s, c), vec2(-c, s));
    
  float f = 0.0;
  float amp = 1.0;
  for (int i = 0; i < 5; i++)
  {
    f += amp * texture(iChannel0, at).r;
        
    at *= 2.0;
    at = r * at;
        
    amp *= 0.5;
  }

  return f;
}

float
fbm_lowres (vec2 at)
{
  float s = sin(0.45);
  float c = cos(0.45);
  mat2 r = mat2(vec2(s, c), vec2(-c, s));
    
  float f = 0.0;
  float amp = 1.0;
  for (int i = 0; i < 3; i++)
  {
    f += amp * texture(iChannel0, at).r;
        
    at *= 2.0;
    at = r * at;
        
    amp *= 0.6;
  }
  return f;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  uv.x *= 2.5;
  uv.x += sin(uv.y * 14.0) * 0.01;
  uv.y -= 0.5;

  vec2 off = vec2(0.004, -0.04) * iTime * sign(uv.y);

  float distX = fbm_lowres(uv * 0.05 + iTime * 0.01);
  float distY = fbm_lowres(uv * 0.05 + iTime * 0.01 + 50.0);
  vec2 distortion = (vec2(distX, distY * 0.7) - 0.5) * 0.005;

  float bottom = pow(abs(1.0 - uv.y), 1.3);

  float f = fbm(uv * 0.03 + off * 0.5 + distortion * bottom);
  f += fbm(uv * 0.03 + off) * 0.2;;

  f -= pow(abs(uv.y), 0.4);
  f = max(f, 0.0);

  float deg0 = pow(f - 0.1, 28.0);
  float deg1 = pow(f - 0.05, 12.0);
  float deg2 = pow(f, 1.7);

  deg2 -= deg1;
  deg1 -= deg0;

  vec3 col = vec3(0.9, 0.8, 0.7) * deg0;
  col += vec3(0.7, 0.6, 0.2) * deg1;
  col += vec3(0.5, 0.5, 0.5) * deg2;

  col.rgb += vec3(1.0, 0.9, 0.7) * pow((1.0 - abs(uv.y)), 24.0);

  gl_FragColor = vec4(col, 1.0);
}
