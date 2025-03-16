uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define A(v) mat2(cos(m.v+radians(vec4(0, -90, 90, 0))))  // rotate
#define W(v) length(vec3(p.yz-v(p.x+vec2(0, pi_2)+t), 0))-lt  // wave
//#define W(v) length(p-vec3(round(p.x*pi)/pi, v(t+p.x), v(t+pi_2+p.x)))-lt  // alt wave
#define P(v) length(p-vec3(0, v(t), v(t+pi_2)))-pt  // point

void
main ()
{
  float lt = .1, // line thickness
        pt = .3, // point thickness
        pi = 3.1416,
        pi2 = pi*2.,
        pi_2 = pi/2.,
        t = iTime*pi,
        s = 1., d = 0., i = d;

  vec2 m = (iMouse.xy-.5*iResolution.xy)/iResolution.y*4.;

  vec3 o = vec3(0, 0, -7), // cam
       u = normalize(vec3((gl_FragCoord.xy-.5*iResolution.xy)/iResolution.y, 1)),
       c = vec3(0), k = c, p;

  if (iMouse.z < 1.)
    m = -vec2(t/20.-pi_2, 0); // move when not clicking
  mat2 v = A(y), h = A(x); // pitch & yaw

  for (; i++<50.;) // raymarch
  {
    p = o+u*d;
    p.yz *= v;
    p.xz *= h;
    p.x -= 3.; // slide objects to the right a bit
    if (p.y < -1.5) p.y = 2./p.y; // reflect into neg y
    k.x = min( max(p.x+lt, W(sin)), P(sin) ); // sine wave
    k.y = min( max(p.x+lt, W(cos)), P(cos) ); // cosine wave
    s = min(s, min(k.x, k.y)); // blend
    if (s < .001 || d > 100.)
      break; // limits
    d += s*.5;
  }

  // add and color scene
  c = max(cos(d*pi2) - s*sqrt(d) - k, 0.);
  c.gb += .1;
  gl_FragColor = vec4(c*.4 + c.brg*.6 + c*c, 1);
}
