#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define pi 3.14159265359
#define radius .1

void
main ()
{
  float aspect = iResolution.x / iResolution.y;

  vec2 uv = gl_FragCoord.xy * vec2(aspect, 1.) / iResolution.xy;
  vec2 mouse = iMouse.xy  * vec2(aspect, 1.) / iResolution.xy;
  vec2 mouseDir = normalize(abs(iMouse.zw) - iMouse.xy);
  vec2 origin = clamp(mouse - mouseDir * mouse.x / mouseDir.x, 0., 1.);

  float mouseDist = clamp(length(mouse - origin) + (aspect - (abs(iMouse.z) / iResolution.x) * aspect) / mouseDir.x, 0., aspect / mouseDir.x);

  if (mouseDir.x < 0.)
    mouseDist = distance(mouse, origin);

  float proj = dot(uv - origin, mouseDir);
  float dist = proj - mouseDist;

  vec2 linePoint = uv - dist * mouseDir;

  if (dist > radius)
  {
    gl_FragColor = texture(iChannel1, uv * vec2(1. / aspect, 1.));
    gl_FragColor.rgb *= pow(clamp(dist - radius, 0., 1.) * 1.5, .2);
  }
  else if (dist >= 0.)
  {
    float theta = asin(dist / radius);
    vec2 p2 = linePoint + mouseDir * (pi - theta) * radius;
    vec2 p1 = linePoint + mouseDir * theta * radius;
    uv = (p2.x <= aspect && p2.y <= 1. && p2.x > 0. && p2.y > 0.) ? p2 : p1;
    gl_FragColor = texture(iChannel0, uv * vec2(1. / aspect, 1.));
    gl_FragColor.rgb *= pow(clamp((radius - dist) / radius, 0., 1.), .2);
  }
  else
  {
    vec2 p = linePoint + mouseDir * (abs(dist) + pi * radius);
    uv = (p.x <= aspect && p.y <= 1. && p.x > 0. && p.y > 0.) ? p : uv;
    gl_FragColor = texture(iChannel0, uv * vec2(1. / aspect, 1.));
  }
}
