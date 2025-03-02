#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define T (iTime * 5.0)
#define A(v) mat2 (cos (m.v * 3.1416 + vec4 (0.0, -1.5708, 1.5708, 0.0)))
#define H(v) (cos (((v) + 0.5) * 6.2832 + radians (vec3 (60.0, 0.0, -60.0))) * 0.5 + 0.5)

float
map (vec3 u)
{
  int l = 5, i = 0;
  float t = T,
        w = 40.0,
        s = 0.4,
        f = 1e20, y, z;

  u.yz = -u.zy;
  u.xy = vec2 (atan (u.x, u.y), length (u.xy));
  u.x += t / 6.0;

  vec3 p;
  for (; i++ < l;)
  {
    p = u;
    y = round (max (p.y - float (i), 0.0) / float (l)) * float (l) + float (i);
    p.x *= y;
    p.x -= sqrt (y * t * t * 2.0);
    p.x -= round (p.x / 6.2832) * 6.2832;
    p.y -= y;
    p.z += sqrt (y / w) * w;
    z = cos (y * t / 50.0) * 0.5 + 0.5;
    p.z += z * 2.0;
    p = abs (p);
    f = min (f, max (p.x, max (p.y, p.z)) - s * z);
  }

  return f;
}

void
main ()
{
  int l = 50, i = 0;
  float d = 0.0, s, r;

  vec2 R = iResolution.xy,
       m = iMouse.z > 0. ?  // clicking?
              (iMouse.xy - R/2.)/R.y : // mouse coords
              vec2(0.0, -.17);         // default (noclick)
    
  vec3 o = vec3(0.0, 20.0, -120.0), // camera
       u = normalize (vec3 (gl_FragCoord.xy - R / 2.0, R.y)),  // 3d coords
       c = vec3 (0.0), p;

  mat2 v = A (y),  // pitch
       h = A (x);  // yaw

  for (; i++ < l;)  // raymarch
  {
    p = u * d + o;
    p.yz *= v;
    p.xz *= h;

    s = map (p);
    r = (cos (round (length (p.xz)) * T / 50.) * 0.7 - 1.8) / 2.0;  // color gradient
    c += min (s, exp (-s / 0.07)) * H (r + 0.5) * (r + 2.4);

    if (s < 1e-3 || d > 1e3)
      break;
    d += s * 0.7;
  }

  gl_FragColor = vec4 (exp (log (c) / 2.2), 1.0);
}
