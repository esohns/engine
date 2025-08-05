#version 130

// glut_978_common.glsl
mat3
rotX (float a)
{
  return mat3(1., 0., 0.,
              0., cos(a), sin(a),
              0., -sin(a), cos(a));
}

mat3
rotY (float a)
{
  return mat3(cos(a), 0., sin(a),
              0., 1., 0.,
              -sin(a), 0., cos(a));
}

mat3
rotZ (float a)
{
  return mat3(cos(a), sin(a), 0.,
              -sin(a), cos(a), 0.,
              0., 0., 1.);
}

float
area (vec3 a, vec3 b, vec3 c)
{
  b -= a;
  c -= a;

  return length(cross(b,c))/2.;
}
// glut_978_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

const float pi = 3.14159265358979323;
const float th = pi * 2. / 3.;
const float edgeLength = 2. / tan(th);
const float th2 = -pi * .25;
const float e = edgeLength * 1.5;
const mat2 rm = mat2(cos(th2), sin(th2), -sin(th2), cos(th2));
const mat2 m = rm * mat2(1. / 2., 0., 0., e / 2.) * sqrt(2.);
const mat2 mt = (1. / sqrt(2.)) * mat2(2., 0., 0., 2. / e) * transpose(rm);
const vec2 ns[3] = vec2[3](vec2(1, 0), vec2(cos(th), sin(th)), vec2(cos(th * 2.), sin(th * 2.)));
const float maxHeight = 6.;

float
sampleHeightfield (vec2 p)
{
  float h = textureLod(iChannel0, p / 40. + iTime / 400., 2.).b *
            textureLod(iChannel1, p / 8., 2.).r * 1.6;

  return clamp(h, 0., 1. - 1e-4) * maxHeight;
}

float
trace (vec3 ro, vec3 rd, out vec3 triNorm, out vec3 bary)
{
  vec3 oro = ro;

  float mint = (maxHeight * step(rd.y, 0.) - ro.y) / rd.y;

  ro += rd * max(0., mint);

  vec2 u = m * ro.xz;

  vec2 cu = floor(u), fu = u - cu;

  vec2 tri0, tri1, tri2;

  tri0 = mt * cu;

  if(fu.x > fu.y)
  {
    tri1 = mt * (cu + vec2(1, 1));
    tri2 = mt * (cu + vec2(1, 0));
  }
  else
  {
    tri1 = mt * (cu + vec2(1, 1));
    tri2 = mt * (cu + vec2(0, 1));
  }

  vec3 rod = vec3(ro.x, dot(ro.xz, ns[1]), dot(ro.xz, ns[2]));
  vec3 rdd = vec3(rd.x, dot(rd.xz, ns[1]), dot(rd.xz, ns[2]));

  vec3 inv = vec3(1) / rdd;

  vec3 is = (floor(rod) + step(0., rdd) - rod) * inv;

  inv = abs(inv);

  vec3 triangle[3];

  for(int j = 0; j < 3; ++j)
  {
    float d0 = abs(dot(tri1 - tri0, ns[j]));
    float d1 = abs(dot(tri2 - tri1, ns[j]));
    float d2 = abs(dot(tri0 - tri2, ns[j]));

    triangle[j].xz = tri1;

    if(d0 < d1)
    {
      if(d0 < d2)
        triangle[j].xz = tri2;
    }
    else if(d1 < d2)
      triangle[j].xz = tri0;

    triangle[j].y = sampleHeightfield(triangle[j].xz);
  }

  vec2 triSteps[3] = vec2[3](ns[0] * 2. * sign(rdd.x),
                             ns[1] * 2. * sign(rdd.y),
                             ns[2] * 2. * sign(rdd.z));

  float t0 = 0., t1, t = -1.;

  float maxt = (maxHeight * step(0., rd.y) - ro.y) / rd.y;

  triNorm = vec3(0);

  for(int i = min(iFrame, 0); i < 200; ++i)
  {
    int idx = 2;
    t1 = is.z;

    if(is.x < is.y)
    {
      if(is.x < is.z)
      {
        idx = 0;
        t1 = is.x;
      }
    }
    else if(is.y < is.z)
    {
      idx = 1;
      t1 = is.y;
    }

    triNorm = cross(triangle[2] - triangle[0], triangle[1] - triangle[0]);
    t = dot(triangle[0] - ro, triNorm) / dot(rd, triNorm);

    if(t > t0 && t < t1)
      break;

    if(t1 > maxt)
      return 1e5;

    int idx1 = (idx + 1) % 3, idx2 = (idx + 2) % 3;

    is[idx] += inv[idx];

    triangle[idx].xz += triSteps[idx];

    triangle[idx].y = sampleHeightfield(triangle[idx].xz);

    vec3 temp = triangle[idx1];
    triangle[idx1] = triangle[idx2];
    triangle[idx2] = temp;

    t0 = t1;
  }

  triNorm = normalize(triNorm);

  vec3 rp = ro + rd * t;

  float alpha = area(triangle[0], triangle[1], rp);
  float beta = area(triangle[1], triangle[2], rp);
  float gamma = area(triangle[2], triangle[0], rp);

  float area = alpha + beta + gamma;

  bary = vec3(alpha, beta, gamma) / area;

  return distance(oro, rp);
}

vec3
rfunc (vec2 uv)
{
  vec3 r = normalize(vec3(uv.xy, -1.5));
  mat3 m = rotX(-.75) * rotZ(sin(iTime / 4.) * .1);
  return m * r;
}

void
main ()
{
  vec3 col = vec3(0);

  vec2 uv = gl_FragCoord.xy / iResolution * 2. - 1.;
  uv.x *= iResolution.x / iResolution.y;

  vec3 o = vec3(cos(iTime / 4.) * 4., 10., -iTime),
       r = rfunc(uv);

  vec3 triNorm, bary;
  float t = trace(o, r, triNorm, bary);

  vec3 n = triNorm;

  vec3 rp = o + r * t;
  vec3 ld = normalize(vec3(10, 6, 3));

  col = vec3(max(0., dot(triNorm, ld))) * .8;

  float st = trace(rp + ld * 1e-2, ld, triNorm, triNorm);
  if(st > 1e-2 && st < 1e3)
    col *= .1;

  col += max(0., n.y) * vec3(.3);

  col *= cos((rp.y + 6.5) * vec3(1.5, 2, .5) / 3.) * .5 + .5;
  float w = t / 800. + pow(max(0., 1. - dot(-r, n)), 4.) * .2;
  col *= mix(1.4, 1., smoothstep(.02 - w, .02 + w, min(bary.x, min(bary.y, bary.z))));

  col = mix(vec3(.5, .5, 1.), col, exp2(-t / 400.));

  gl_FragColor = vec4(pow(clamp(col, 0., 1.), vec3(1. / 2.2)), 1.0);
}
