#version 140

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform samplerCube iChannel0;

const float pi = 3.14159265358979232;
const float order = 6.;
const float th = pi * 2. / order;
const float r2 = .08;
const float opp = tan(th / 2.);
const float hyp = sqrt(opp * opp + 1.);

const vec2 q = hyp * vec2(cos(th / 2.), sin(th / 2.));
const vec2 thv = vec2(cos(th), sin(th));
const mat3 iter_xfrm = mat3(mat2(1. / opp)) * mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(-q.x, -q.y, 1.));

mat2
rot (float th)
{
  return mat2(cos(th),sin(th),-sin(th),cos(th));
}

mat3
rotX (float a)
{
  return mat3(1., 0., 0., 0., cos(a), sin(a), 0., -sin(a), cos(a));
}

mat3
rotY (float a)
{
  return mat3(cos(a), 0., sin(a), 0., 1., 0., -sin(a), 0., cos(a));
}

mat3
rotZ (float a)
{
  return mat3(cos(a), sin(a), 0., -sin(a), cos(a), 0., 0., 0., 1.);
}

vec4
ring (vec2 uv, vec2 rp, float r1, float r2, float sc, vec3 rd, vec3 col, float lod)
{
  mat3 envm = rotZ(cos(iTime) * .5) * rotY(sin(iTime) * .3);
  vec4 res;
  float u = (length(uv - rp) * sc - r1 + r2 * 2.) / r2;
  res.a = abs(length(uv - rp) - (r1 - r2 * 2.) / sc) - r2 / sc;
  u = clamp(u, -1., +1.);
  vec3 n = vec3(normalize(uv - rp) * u, sqrt(1. - u * u));
  vec3 r = reflect(rd, n);
  res.rgb = textureLod(iChannel0, envm * r, lod).rrr * col;

  return res;
}

void
main ()
{
  float zoom = 4.;
  vec2 uv = (gl_FragCoord.xy - iResolution * .5) / iResolution.y * zoom;

  if(iMouse.z > 0.)
  {
    zoom = 1.2;
    vec2 offset = (iMouse.xy - iResolution * .5) / iResolution;
    uv = (gl_FragCoord.xy - iResolution * .5) / iResolution.y * zoom + offset * 4.;
  }

  vec3 rd = normalize(vec3(uv.xy / 4., 1.));
  vec3 col = vec3(0.);
  vec2 p = uv;

  float w = zoom / iResolution.x;
  float sc = 1.;
  float a;
  int j = 0;
  mat3 m = mat3(1.);

  for(int i = 0; i < 15; ++i)
  {
    float a2 = atan(p.y, p.x);
    a = mod(a2, th);        
    mat2 rm = rot(a - a2);

    m = mat3(rm) * m;
    p = rm * p;

    if(p.x < 1. && dot(p - thv, thv) < 0.)
      break;

    p = (p - q) / opp;
    m = iter_xfrm * m;

    if(order == 6.)
    {
      p = rot(th / 2.) * p;
      m = mat3(rot(th / 2.)) * m;
    }

    sc /= opp;
    ++j;
  }

  float d = min(min(min(p.y, -p.x + 1.), -dot(p - thv, thv)), dot(p, vec2(thv.y, -thv.x)));

  vec3 r0p = inverse(m) * vec3(0., 0., 1.);
  vec3 r1p = inverse(m) * vec3(q, 1.);

  col = vec3(.05);

  vec3 ringCol0 = vec3(1.), ringCol1 = vec3(1, .9, .5);

  if((j & 1) == 1)
  {
    vec3 temp = ringCol0;
    ringCol0 = ringCol1;
    ringCol1 = temp;
  }

  vec4 ring0, ring1;
  float lod = .5 + log2(3. * sc * zoom);
  
  ring0 = ring(uv, r0p.xy, 1., r2, sc, rd, ringCol0, lod);
  ring1 = ring(uv, r1p.xy, opp, r2 * opp, sc, rd, ringCol1, lod);

  if(a < th / 2.)
  {
    vec4 temp = ring0;
    ring0 = ring1;
    ring1 = temp;
  }

  col = mix(col, ring0.rgb * smoothstep(.0, .07 / sc, ring1.a + .005 / sc), 1. - smoothstep(-w, +w, ring0.a));
  col = mix(col, ring1.rgb, 1. - smoothstep(-w, +w, ring1.a));

  if(ring0.a > 0. && ring1.a > 0.)
    col *= mix(.0, 1., smoothstep(-.1, .11, min(ring0.a, ring1.a)));

  //col += vec3(1. - smoothstep(.002, .003,d / (sc * zoom) * 2.)) * .3;

  gl_FragColor = vec4(sqrt(col * 1.3), 1.0);
}
