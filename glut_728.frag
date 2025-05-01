#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

vec3
subImg (vec2 fragCoord, float xs, float ys, float zs)
{
  vec2 xy = fragCoord.xy / iResolution.xy;
  xy -= 0.5;
  xy += vec2(sin(iTime * xs) * 0.1, cos(iTime * ys) * 0.1); //move
  xy *= (1.1 + sin(iTime * zs) * 0.1); //scale
  xy += 0.5;
  return texture(iChannel0, xy).xyz;
}

vec3
drawCircle (vec2 xy)
{
  float l = length(xy);
  return (l > .233 || l < .184) ? vec3(0) : vec3(sin(l * 128.0) * .5 + 0.5);
}

void
main ()
{
  vec2 xy = iResolution.xy;
  xy = -.5 * (xy - 2.0 * gl_FragCoord.xy) / xy.x;
  xy *= 1.0 + sin(iTime * 4.0) * 0.2;
  xy.x += sin(xy.x * 32.0 + iTime * 16.0) * 0.01;
  xy.y += sin(xy.y * 16.0 + iTime * 8.0) * 0.01;
    
  vec3 c = drawCircle(xy);
 
  vec3 fC =
        subImg(gl_FragCoord.xy, 3.3, 3.1, 2.5) * vec3(0.3, 0.7, 1.0) +
        subImg(gl_FragCoord.xy, 2.4, 4.3, 3.3) * vec3(0.3, 1.0, 0.7) +
        subImg(gl_FragCoord.xy, 2.2, 4.2, 4.2) * vec3(1.0, 0.7, 0.3) +
        subImg(gl_FragCoord.xy, 3.2, 3.2, 2.1) * vec3(1.0, 0.3, 0.7) +
        subImg(gl_FragCoord.xy, 2.2, 1.2, 3.4) * vec3(0.3, 0.5, 0.7) +
        subImg(gl_FragCoord.xy, 5.2, 2.2, 2.2) * vec3(0.8, 0.5, 0.1);
    
  gl_FragColor = vec4((fC / 3.6 + c) * 0.95, 1.0);
}
