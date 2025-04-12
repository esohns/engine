uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define resolution (iResolution.xy)
#define t (iTime * .3)

struct mat_t
{
  vec3 diffuse;
  vec4 specular;
};

#define N 6
vec3 b1[N];
vec3 b2[N];

vec3 l1pos = 1.*vec3(6., 3., -7.);
vec3 l2pos = 1.*vec3(-7., 1., 6.);
vec3 l3pos = 1.*vec3(7., -4., 7.);
vec3 l4pos = vec3(0.,5.,25.);
vec3 l1color = 40.*vec3(1.,.7,.7);
vec3 l2color = 20.*vec3(.7,.7,1.);
vec3 l3color = 60.*vec3(.7,1.,.7);
vec3 l4color = vec3(100.);

float
t1 (vec3 p)
{
  float v = 0.;
  for (int i = 0; i < N; ++i)
  {
    vec3 b = p - b1[i];
    v += 5. / dot(b, b);
  }
  float d = 12. - p.y; v += 3. / (d*d);
  return v;
}

float
t2 (vec3 p)
{
  float v = 0.;
  for (int i = 0; i < N; ++i)
  {
    vec3 b = p - b2[i];
    v += 5. / dot(b, b);
  }
  float d = 12. + p.y; v += 3. / (d*d);
  return v;
}

float w1(vec3 p) { return 1. - t1(p) + t2(p); }
float w2(vec3 p) { return 1. + t1(p) - t2(p); }

float
world (vec3 p)
{
  return min(w1(p), w2(p));
}

vec3
normal (vec3 p)
{
  vec2 e = vec2(.001,0.);
  return normalize(vec3(
      world(p+e.xyy) - world(p-e.xyy),
      world(p+e.yxy) - world(p-e.yxy),
      world(p+e.yyx) - world(p-e.yyx)));
}

mat_t
material (vec3 p)
{
  mat_t m;
  m.diffuse = vec3(0.);
  m.specular = vec4(0.);
  if (w1(p) < w2(p))
  {
    m.diffuse = vec3(.92, .027, .027);
    m.specular = vec4(mix(vec3(1.),m.diffuse,.7), 2000.);
  }
  else
  {
    m.diffuse = vec3(.14, .17, 1.57);
    m.specular = vec4(mix(vec3(1.),m.diffuse,.5), 100.);
  }
  return m;
}

float iter = 0.;
float
trace (vec3 O, vec3 D, float L, float Lmax)
{
  float Lp = L;
  for (int i = 0; i < 40; ++i)
  {
    iter = float(i);
    float d = world(O + D * L);
    if (d < .01*L || L > Lmax) break;
    Lp = L;

    L += d * 1.5;
  }

  if (L < Lmax)
  {
    for (int i = 0; i < 5; ++i)
    {
      float Lm = (Lp + L) * .5;
      if (world(O + D * Lm) < .01*Lm) L = Lm;
      else Lp = Lm;
    }
  }
  iter /= 32.;
  return L;
}

float
shadowtrace (vec3 p, vec3 d, float lmax)
{
  float v = .02;
  for (int i = 1; i < 9; ++i)
    v = min(v, world(p+d*lmax*float(i)/12.));
  return smoothstep(.0, .02, v);
}

vec3
enlight (vec3 p, vec3 v, vec3 n, mat_t m, vec3 lpos, vec3 lcolor)
{
  vec3 ldir = lpos - p;
  float ldist2 = dot(ldir, ldir);
  float ldist = sqrt(ldist2);
  ldir /= ldist;
  float shadow = shadowtrace(p, ldir, ldist-.2);
  return shadow * lcolor * (
    m.diffuse * max(0., dot(n,ldir))
    + m.specular.rgb * pow(max(0.,dot(normalize(ldir-v), n)), m.specular.w)
    * (m.specular.w + 2.) * (m.specular.w + 4.) / (24. * (m.specular.w + pow(2., -m.specular.w/2.)))
    ) / ldist2;
}

vec3
lightball (vec3 lpos, vec3 lcolor, vec3 O, vec3 D, float L)
{
  vec3 ldir = lpos-O;
  float ldist = length(ldir);
  if (ldist > L) return vec3(0.);
  float pw = pow(max(0.,dot(normalize(ldir),D)), 20000.);
  return (normalize(lcolor)+vec3(1.)) * pw;
}

void
main ()
{
  for (int i = 0; i < N; ++i)
  {
    float fi = float(i)*.7;
    b1[i] = vec3(3.7*sin(t+fi), 1.+10.*cos(t*1.1+fi), 2.3*sin(t*2.3+fi));
    fi = float(i)*1.2;
    b2[i] = vec3(4.4*cos(t*.4+fi),-1.-10.*cos(t*0.7+fi), -2.1*sin(t*1.3+fi));
  }

  vec2 uv = gl_FragCoord.xy / resolution.xy * 2. - 1.;
  uv.x *= resolution.x / resolution.y;

  vec3 O = vec3(0.,0.,20.);
  vec3 D = normalize(vec3(uv,-1.6));

  if (iMouse.z > 0.)
  {
    vec2 m = iMouse.xy/iResolution.xy*2.-1.;
    float a = - m.x * 2. * 3.1415926;
    float s = sin(a), c = cos(a);
    O = vec3(s*20.,-m.y*10.,c*20.);
    vec3 fw = normalize(vec3(0.) - O);
    vec3 rg = cross(fw,vec3(0.,1.,0.));
    D = normalize(mat3(rg, cross(rg, fw), -fw) * D);
  }

  float L = trace(O, D, 0., 40.);
  vec3 color = vec3(0.);
  if (L < 40.)
  {
    vec3 p = O + D * L;
    vec3 n = normal(p); 
    mat_t m = material(p);

    color = .001 * m.diffuse * n;

    color += enlight(p, D, n, m, l1pos, l1color);
    color += enlight(p, D, n, m, l2pos, l2color);
    color += enlight(p, D, n, m, l3pos, l3color);
    color += enlight(p, D, n, m, l4pos, l4color);

    color *= (1. - smoothstep(10., 20., length(p)));
  }
  else
    L = 100.;

  color += lightball(l1pos, l1color, O, D, L);
  color += lightball(l2pos, l2color, O, D, L);
  color += lightball(l3pos, l3color, O, D, L);
  color += lightball(l4pos, l4color, O, D, L);

  gl_FragColor = vec4(pow(color,vec3(.7)),1.0);
}
