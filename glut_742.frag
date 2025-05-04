uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform bvec4 iKeyboard; // a, b, f, r

#define time iTime
#define mouse iMouse
#define resolution iResolution

bool animate_pattern = false; // a
bool show_background = true; // b
bool show_reflections = false; // f
bool rotation_scene = true; // r
bool cross_eye_view = false; // s
//---------------------------------------------------------
void keyInput()
{
  animate_pattern = iKeyboard.x;
  show_background = iKeyboard.y;
  show_reflections = iKeyboard.z;
  rotation_scene = iKeyboard.w;
  cross_eye_view = false;
}

#define MAP_OCTAVE 7
#define BACK_COLOR vec3(0.4, 0.05, 0.05)
#define INNER_COLOR vec3(0.2, 0.2, 0.6)
#define GLAS_COLOR vec3(0.5, 0.5, 0.9)

vec2
csqr (vec2 a)
{
  return vec2(a.x * a.x - a.y * a.y, 2. * a.x * a.y);
}

mat2
rot (float a)
{
  return mat2(cos(a), sin(a), -sin(a), cos(a));
}

float
map (vec3 p)
{
  float res = 0.;
  vec3 c = p;
  for (int i = 0; i < MAP_OCTAVE; ++i)
  {
    p = .7 * abs(p) / dot(p, p) - .7;
    p.yz += csqr(p.yz);
    p = p.zxy;
    res += exp(-25.0 * abs(dot(p, c)));
  }
  return res;
}

vec2
iSphere (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot(rd, oc);
  float c = dot(oc, oc);
  float h = b * b - c + sph.w * sph.w;
  if (h < 0.0)
    return vec2(-1.0);
  h = sqrt(h);
  return vec2(-b - h, -b + h);
}

vec3
raymarch (vec3 ro, vec3 rd, vec2 tminmax)
{
  float t = tminmax.x;
  float dt = 0.02;
  if (animate_pattern)
    dt = 0.02 + 0.01 * cos(time * 0.5);

  vec3 col = vec3(0.);
  float c = 0.;
  for (int i = 0; i < 64; i++)
  {
    t += dt * exp(-2. * c);
    if (t > tminmax.y)
      break;
    vec3 pos = ro + t * rd;
    c = 0.45 * map(ro + t * rd);
    col = 0.98 * col + 0.08 * vec3(c * c, c, c * c * c);
    col = 0.99 * col + 0.08 * vec3(c * c * c, c * c, c);
    col = 0.99 * col + 0.08 * vec3(c, c * c * c, c * c);
  }
  return col;
}

void
main ()
{
  keyInput();

  vec2 uv = 2.0 * (gl_FragCoord.xy / resolution.xy) - 1.0;
  uv.x *= resolution.x / resolution.y;
  vec2 m = vec2(-0.5);
  if( mouse.z > 0.0 )
    m = mouse.xy / resolution.xy * 3.14;
//  m += mouse.xy * 3.14;

  float eye = 0.1;
  float radius = 1.4;

  if (cross_eye_view)
  {
    radius = 1.0;
    if (uv.x > 0.)
    {
      uv.x -= 0.8;
      eye = -0.1;
    }
    else
    {
      uv.x += 0.8;
      eye = 0.1;
    }
  }

  vec3 ro = vec3(-6.8, 0, 0);
  ro.z += eye;
  ro.yz *= rot(m.y);
  ro.xz *= rot(m.x);
  if (rotation_scene)
    ro.xz *= rot(0.07 * time);

  vec3 ta = vec3(0.0);
  vec3 ww = normalize(ta - ro);
  vec3 uu = normalize(cross(ww, vec3(0.0, 1.0, 0.0)));
  vec3 vv = normalize(cross(uu, ww));
  vec3 rd = normalize(uv.x * uu + uv.y * vv + 4.0 * ww);

  vec2 tmm = iSphere(ro, rd, vec4(0., 0., 0., radius));

  vec3 col = raymarch(ro, rd, tmm);

  const float taa = 6.3;
  float aa = (2.2 * (tmm.x - taa));
  if ((tmm.x < 0.0) || (tmm.x > taa))
  {
    vec3 bgCol = vec3(0);
    if (show_background)
      bgCol = BACK_COLOR * map(rd);
    if (tmm.x > taa)
      col = mix(col, bgCol, aa);
    else
      col = bgCol;
  }

  if (tmm.x > 0.0)
  {
    vec3 nor = reflect(rd, (ro + tmm.x * rd) * 0.5);
    float fre = pow(0.3 + clamp(dot(nor, rd), 0.0, 1.0), 3.) * 1.3;

    col += GLAS_COLOR * fre * 0.1 * (1.0 - aa);
    if (show_reflections)
      col += BACK_COLOR * map(nor) * fre * 0.3 * (1.0 - aa);
  }

  col = 0.5 * (log(1.0 + col));
  col = clamp(col, 0., 1.0);
  gl_FragColor = vec4(col, 1.0);
}
