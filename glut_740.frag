#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

vec2
perp (vec2 u)
{
  return vec2(-u.y, u.x);
}

vec2
rotate (vec2 rvec, vec2 p)
{
  return p.x * rvec + p.y * vec2(-rvec.y, rvec.x);
}

vec2
unrotate (vec2 rvec, vec2 p)
{
  return p.x * vec2(rvec.x, -rvec.y) + p.y * rvec.yx;
}

float
dseg (vec2 p, vec2 a, vec2 b)
{
  vec2 pa = p - a;
  vec2 ba = b - a;

  float u = dot(pa, ba) / dot(ba, ba);

  u = clamp(u, 0.0, 1.0);

  return length(pa - u * ba);
}

bool
in_half_plane (vec2 p, vec2 a, vec2 b)
{
  vec2 pa = p - a;
  vec2 ba = b - a;
  return dot(pa, perp(ba)) > 0.0;
}

bool
in_triangle (vec2 p, vec2 a, vec2 b, vec2 c)
{
  return in_half_plane(p, a, b) && in_half_plane(p, b, c) && in_half_plane(p, c, a);
}

vec2
grad (ivec2 z)
{
  int n = z.x + z.y * 11111;

  n = (n << 13) ^ n;
  n = (n * (n * n * 15731 + 789221) + 1376312589) >> 16;

  n &= 7;

  vec2 gr = vec2(n & 1, n >> 1) * 2.0 - 1.0;

  return (n >= 6) ? vec2(0.0, gr.x) : (n >= 4) ? vec2(gr.x, 0.0) : gr;
}

float
noise (vec2 p)
{
  ivec2 i = ivec2(floor(p));
  vec2 f = fract(p);
  
  vec2 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);

  return mix(mix(dot(grad(i + ivec2(0, 0)), f - vec2(0.0, 0.0)),
                 dot(grad(i + ivec2(1, 0)), f - vec2(1.0, 0.0)), u.x),
             mix(dot(grad(i + ivec2(0, 1)), f - vec2(0.0, 1.0)),
                 dot(grad(i + ivec2(1, 1)), f - vec2(1.0, 1.0)), u.x), u.y);
}

const vec3 CELL_COLORS[4] = vec3[4](vec3(0.9, 0.0, 0.05), vec3(0.95, 0.85, 0.0), vec3(0.1, 0.8, 0.05), vec3(0.1, 0.5, 0.8));
const vec2 ROT_VECTORS[4] = vec2[4](vec2(1, 0), vec2(0, 1), vec2(-1, 0), vec2(0, -1));
vec2 O, A, B, C, D;

vec2
heightmap (vec2 p)
{
  float dpoly = dseg(p, O, A);
  dpoly = min(dpoly, dseg(p, A, B));
  dpoly = min(dpoly, dseg(p, B, C));
  dpoly = min(dpoly, dseg(p, C, D));
  dpoly = min(dpoly, dseg(p, D, O));

  float k = 0.08;

  float z = k + 0.01 * noise(5. * p);

  if (dpoly < k)
  {
    float w = (dpoly / k - 1.0);
    z *= sqrt(1.0 - w * w);
  }
  else
    z *= (1.0 - 0.03 * smoothstep(k, 2.0 * k, dpoly));
    
  return vec2(z, dpoly);
}

void
main ()
{
  vec2 global_rot = vec2(0.5 * sqrt(2.0));
  float scl = 6.0 * global_rot.x / iResolution.y;

  vec2 uv = vec2(0.7886751345948132, 0.21132486540518713);

  vec2 lcoord = vec2(0);

  if (iMouse.z > 0.)
  {
    uv = clamp(iMouse.xy / iResolution.xy, 0.0, 1.0);
    lcoord = vec2(-0.0, 0.5);
  }
  else
  {
    uv = (gl_FragCoord.xy - 0.5 * iResolution.xy) / (max(iResolution.x, iResolution.y));

    const float repeat = 30.0;
    const float wraps_per_repeat = 5.0;

    const float x_lobes = 3.0;
    const float y_lobes = 2.0;

    const float two_pi = 6.283185307179586;

    float t = iTime * two_pi / repeat;

    float t_wrap = t * wraps_per_repeat;

    float c = cos(t_wrap);
    float s = sin(t_wrap);

    uv = rotate(vec2(s, -c), uv);

    uv = clamp(uv + 0.5, 0.0, 1.0);

    lcoord = vec2(-sin(t * x_lobes), cos(t * y_lobes));
  }

  const float cz = 3.5;

  vec3 lpos = vec3(lcoord * 0.5 * iResolution.xy * scl, cz);

  const vec3 cpos = vec3(0, 0, cz);

  vec2 porig = (gl_FragCoord.xy + vec2(0.13, 0.17) - 0.5 * iResolution.xy) * scl;

  vec2 p = rotate(porig, global_rot);

  O = floor(p + 0.5);

  ivec2 qstep = ivec2(step(p, O));
  int quadrant = (qstep.x ^ qstep.y) + 2 * qstep.y;

  vec2 rvec = ROT_VECTORS[quadrant];

  vec2 xy = 0.5 * uv;
  vec2 st = 0.5 - xy;

  A = O + rotate(rvec, xy);
  B = O + rotate(rvec, vec2(0.5));
  vec2 X = O + rotate(rvec, vec2(0.5, 0));

  float dline = dseg(p, A, X);

  int cell = quadrant;

  if (in_triangle(p, X, B, A))
  {
    cell = (quadrant + 1) & 3;
    O += rvec;
    rvec = perp(rvec);
  }
  else if (in_triangle(p, O, X, A))
  {
    cell = (quadrant + 3) & 3;
    rvec = -perp(rvec);
  }

  A = O + rotate(rvec, xy);
  B = O + rotate(rvec, vec2(0.5));

  C = B + rotate(rvec, perp(st));
  D = O + rotate(rvec, perp(xy));

  vec2 hm = heightmap(p);

  const float h = 1e-3;
  const vec2 eps = vec2(h, 0);

  vec2 hgrad = (0.5 / h) * vec2(
      heightmap(p + eps.xy).x - heightmap(p - eps.xy).x,
      heightmap(p + eps.yx).x - heightmap(p - eps.yx).x
  );

  hgrad = unrotate(global_rot, hgrad);

  float z = hm.x;

  dline = min(dline, hm.y);

  vec3 N = normalize(cross(vec3(1, 0, hgrad.x), vec3(0, 1, hgrad.y)));

  vec3 color = CELL_COLORS[cell];
  color *= color;

  color = mix(color, vec3(0.5), 0.08);

  vec3 pos = vec3(porig, z);

  vec3 L = normalize(lpos - pos);
  vec3 V = -normalize(cpos - pos);

  vec3 R = reflect(L, N);

  color *= 0.1 + 0.9 * clamp(dot(N, L), 0.0, 1.0);
  color += 0.3 * pow(clamp(dot(V, R), 0.0, 1.0), 10.0) * vec3(1);

  color *= smoothstep(0.0, 0.0125, dline);

  color = sqrt(color);

  gl_FragColor = vec4(color, 1);
}
