uniform vec2 iResolution;
uniform float iTime;

// Color palette. The default red and gold trim (0), a four-colored pastel palette (1), greyscale with
// color (2), or just greyscale (3).
#define PALETTE 0 
//#define GREY_LINES
//#define FIXED
//#define BG_MESH
#define INCIRCLES
//#define SHOW_GRID_CELLS

mat2
rot2 (float a)
{
  vec2 v = sin(vec2(1.570796, 0) + a);
  return mat2(v, -v.y, v.x);
}

vec3
grey (vec3 col)
{
  return vec3(1) * dot(col, vec3(.299, .587, .114));
}

vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(113, 1)));
#ifdef FIXED
  return (fract(vec2(262144, 32768)*n) - .5)*2.*.35;//*.8 + .2; 
#else
  p = fract(vec2(262144, 32768) * n);
  return sin(p * 6.2831853 + iTime / 2.) * .35;
#endif
}

float
hash21 (vec2 p)
{
  return fract(sin(dot(p, vec2(113, 1))) * 43758.5453);
}

vec2
inCent (vec2 p0, vec2 p1, vec2 p2)
{
  float bc = length(p1 - p2), ac = length(p0 - p2), ab = length(p0 - p1);
  return (bc * p0 + ac * p1 + ab * p2) / (bc + ac + ab);
}

float
inCentRad (vec2 p0, vec2 p1, vec2 p2)
{
  float bc = length(p1 - p2), ac = length(p2 - p0), ab = length(p0 - p1);

  float p = (bc + ac + ab) / 2.;
  float area = sqrt(p * (p - bc) * (p - ac) * (p - ab));

  return area / p;
}

float
sDistLine (vec2 a, vec2 b)
{
  b -= a;
  return dot(a, vec2(-b.y, b.x) / length(b)); //return dot(a, normalize(vec2(-b.y, b.x)));
}

float
distLine (vec2 a, vec2 b)
{
  vec2 pa = a;
  vec2 ba = a - b;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0., 1.);
  return length(a - ba * h);
}

float
flipDistance (vec2 h1, vec2 h2, vec2 h3)
{
  vec3 g1 = vec3(h1, dot(h1, h1));
  vec3 g2 = vec3(h2, dot(h2, h2));
  vec3 g3 = vec3(h3, dot(h3, h3));
  return dot(g1, cross(g3 - g1, g2 - g1));
}

float
cross2d (vec2 a, vec2 b)
{
  return a.x * b.y - a.y * b.x;
}

bool
insideQuad (vec2 a, vec2 b, vec2 c, vec2 d)
{
  vec2 res = vec2(-1.0);

  vec2 e = b - a;
  vec2 f = d - a;
  vec2 g = a - b + c - d;
  vec2 h = -a;

  float k2 = cross2d(g, f);
  float k1 = cross2d(e, f) + cross2d(h, g);
  float k0 = cross2d(h, e);

  float w = k1 * k1 - 4.0 * k0 * k2;
  if (w < 0.0)
    return false;
  w = sqrt(w);

  float ik2 = 0.5 / k2;
  float v = (-k1 - w) * ik2;
  if (v < 0.0 || v > 1.0)
    v = (-k1 + w) * ik2;
  float u = (h.x - f.x * v) / (e.x + g.x * v);
  if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0)
    return false;

  return true;
}

struct triObj
{
  vec2 p0, p1, p2;
  vec2 id, cID;
};

triObj
triangulate (vec2 p)
{
  vec2 o, o1, o2, o3;

  vec2 g = floor(p);
  p -= g + .5;

  triObj tri;
  tri.p0 = tri.p1 = tri.p2 = vec2(0);
  tri.id = vec2(-1);
  tri.cID = vec2(-1);

  vec2 aO[16];
  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      aO[j * 4 + i] = vec2(i - 1, j - 1) + hash22(g + vec2(i - 1, j - 1)) - p;

  for (int j = 0; j < 3; j++)
    for (int i = 0; i < 3; i++)
    {
      o = aO[(j + 1) * 4 + i];
      o1 = aO[(j + 1) * 4 + i + 1];
      o2 = aO[j * 4 + i + 1];
      o3 = aO[j * 4 + i];

      if (insideQuad(o, o1, o2, o3))
      {
        float f = flipDistance(o - o2, o1 - o2, o3 - o2) < 0. ? 1. : -1.;
        if (f > 0.)
        {
          if (sDistLine(o1, o3) >= 0.)
          {
            o2 = o1;
            o1 = o;
            o = o3;
            tri.cID = vec2(0);
          }
          else
          {
            o = o1;
            o1 = o2;
            o2 = o3;
            tri.cID = vec2(1);
          }
        }
        else
        {
          if (sDistLine(o, o2) >= 0.)
          {
            o1 = o3;
            o3 = o;
            o = o2;
            o2 = o3;
            tri.cID = vec2(2);
          }
          else
            tri.cID = vec2(3);
        }

        tri.p0 = o;
        tri.p1 = o1;
        tri.p2 = o2;
        tri.id = tri.cID + g + vec2(i - 1, j - 1);

        break;
      }
    }

  return tri;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy * .5) / clamp(iResolution.y, 350., 800.);

#ifdef FIXED
  vec2 p = uv*5. - vec2(2, 1)*iTime/8.;
#else 
  vec2 p = uv * 5. - vec2(0, -1) * iTime / 8.;
#endif

  triObj tri = triangulate(p);

  vec3 inC;
  inC.xy = inCent(tri.p0, tri.p1, tri.p2);
  inC.z = inCentRad(tri.p0, tri.p1, tri.p2);

  float d0, d1, d2;
  d0 = distLine(tri.p0, tri.p1);
  d1 = distLine(tri.p1, tri.p2);
  d2 = distLine(tri.p2, tri.p0);
  float triDist = min(min(d0, d1), d2);

  vec3 cellCol;
  if (tri.cID.x == 3.)
    cellCol = vec3(1, 1.3, .6);
  else if (tri.cID.x == 2.)
    cellCol = vec3(1.4, 1, .6);
  else if (tri.cID.x == 1.)
    cellCol = vec3(.6, 1, 1.4);
  else
    cellCol = vec3(1.4, .7, .8);

#if PALETTE==0
  if (tri.cID.x != 3.)
    cellCol = grey(cellCol);
  else
    cellCol = vec3(1.3, .2, .1);
#elif PALETTE==2
  if (tri.cID.x == 1. || tri.cID.x == 3.)
    cellCol = grey(cellCol);
#elif PALETTE==3
  cellCol = grey(cellCol);
#endif

  vec3 bg = mix(cellCol, vec3(1, .9, .7), .25);
  bg *= (triDist * .7 + .3) * 1.55;

  vec3 col = bg;

  float hatch = clamp(sin((p.y - p.x) * 6.283 * 14.) * 1.5 + 1.35, 0., 1.);

  col *= hatch * .5 + .5;

#ifdef BG_MESH
  if (tri.cID.x != 3. && hash21(tri.id) > .5)
  {
    const float scF = 2.5;
    triObj tri2 = triangulate(p * scF);

    float point2 = min(min(length(tri2.p0 / 2.5), length(tri2.p1 / 2.5)), length(tri2.p2 / 2.5)) - .04;
    vec3 inC2;
    inC2.xy = inCent(tri2.p0, tri2.p1, tri2.p2);

    d0 = (distLine(tri2.p0, tri2.p1));
    d1 = (distLine(tri2.p1, tri2.p2));
    d2 = (distLine(tri2.p2, tri2.p0));
    float triDist2 = min(min(d0, d1), d2) / scF - .015;

    col = mix(col, vec3(0), (1. - smoothstep(0., .1, triDist2)) * .5);
    col = mix(col, vec3(0), 1. - smoothstep(0., .015, triDist2 - .02));
    vec3 lCol2 = vec3(1, .85, .4) * .8;
#ifdef GREY_LINES
    lCol2 = grey(lCol2);
#endif
    col = mix(col, lCol2, 1. - smoothstep(0., .015, triDist2));

    col = mix(col, vec3(0), 1. - smoothstep(0., .01, point2 - .02));
    col = mix(col, vec3(1, .9, .7), 1. - smoothstep(0., .01, point2));
  }
#endif

#ifdef INCIRCLES
  vec2 a = inC.xy;
  float dir = (tri.cID.x == 0. || tri.cID.x == 2.) ? -1. : 1.;
  float ang = mod(atan(a.y, a.x) + dir * iTime / 2., 3.14159 * 2.);
  float hLines = clamp(sin(ang * (floor((inC.z - .055) * 69.) + 3.)) * 1.5 + 1.35, 0., 1.) * .7 + .3;

  float inPoint = length(inC.xy) - inC.z + .055;
  col = mix(col, vec3(0), (1. - smoothstep(0., .1, inPoint - .0)) * .5);
  col = mix(col, vec3(0), 1. - smoothstep(0., .01, inPoint - .02));
  vec3 iC = cellCol * max(1. - length(inC.xy) / inC.z * .7, 0.);

  col = mix(col, mix(vec3(1, .9, .7), iC, .4) * hLines, 1. - smoothstep(0., .01, inPoint));
  col = mix(col, vec3(0), (1. - smoothstep(0., .01, inPoint - .02 + .08)) * .9);
  col = mix(col, iC, 1. - smoothstep(0., .01, inPoint + .08));

  inPoint = length(inC.xy) - .025;
  col = mix(col, vec3(0), 1. - smoothstep(0., .01, inPoint - .02));
  col = mix(col, mix(vec3(1, .9, .7), cellCol, .25), 1. - smoothstep(0., .01, inPoint));
#endif

  vec3 lCol = vec3(1, .8, .3);
#ifdef GREY_LINES 
  lCol = grey(lCol);
#endif
  triDist -= .0175;
  col = mix(col, vec3(0), (1. - smoothstep(0., .1, triDist)) * .5);
  col = mix(col, vec3(0), 1. - smoothstep(0., .015, triDist - .02));
  col = mix(col, lCol, 1. - smoothstep(0., .015, triDist));

  float verts = min(min(length(tri.p0), length(tri.p1)), length(tri.p2)) - .06;
  col = mix(col, vec3(0), (1. - smoothstep(0., .1, verts)) * .35);
  col = mix(col, vec3(0), 1. - smoothstep(0., .01, verts - .02));
  col = mix(col, vec3(1, .9, .7), 1. - smoothstep(0., .01, verts));
  verts += .05;
  col = mix(col, vec3(0), 1. - smoothstep(0., .01, verts - .02));
  col = mix(col, vec3(1, .9, .7), 1. - smoothstep(0., .01, verts));
 
#ifdef SHOW_GRID_CELLS
  vec2 q = abs(fract(p) - .5);
  float bord = max(q.x, q.y) - .5;
  bord = max(bord, -(bord + .01));
    
  col = mix(col, vec3(0), (1. - smoothstep(0., .1, bord))*.35);
  col = mix(col, vec3(0), (1. - smoothstep(0., .01, bord - .02)));
  col = mix(col, vec3(1), (1. - smoothstep(0., .01, bord))*.75);
#endif

  uv = gl_FragCoord.xy / iResolution.xy;
  col = mix(col, vec3(0), (1. - pow(16. * uv.x * uv.y * (1. - uv.x) * (1. - uv.y), 0.125 * .5)));

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
