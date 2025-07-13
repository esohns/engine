#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

#define POINTS 60
const vec3 points[POINTS] = vec3[POINTS](vec3(-0.356755,-0.873512,0.331213),vec3(0.535964,0.693963,-0.480789),vec3(0.357604,0.921424,0.15198),vec3(0.548446,0.110022,0.828916),vec3(0.366869,-0.910807,-0.189307),vec3(-0.279594,-0.461,0.842203),vec3(0.352146,-0.44884,0.821301),vec3(-0.951564,-0.295671,0.0842844),vec3(0.476791,0.491518,-0.728753),vec3(-0.270188,0.893548,-0.358568),vec3(0.58947,0.00808059,0.80775),vec3(-0.26365,-0.621182,-0.737985),vec3(-0.532168,-0.845241,0.0486288),vec3(0.666028,0.00892905,-0.745873),vec3(-0.43499,-0.545113,-0.716684),vec3(-0.185326,0.274356,-0.943601),vec3(0.511563,0.772642,0.375936),vec3(0.662696,-0.725596,-0.185321),vec3(0.327068,-0.40428,0.854157),vec3(-0.787343,-0.290344,-0.543867),vec3(-0.325887,-0.917403,-0.228406),vec3(-0.585621,0.810298,0.0215757),vec3(-0.484601,-0.87227,-0.0656304),vec3(-0.618888,0.0938393,-0.779853),vec3(0.167859,-0.148567,-0.974552),vec3(-0.509896,-0.860234,0.00214743),vec3(-0.78295,-0.579171,0.227044),vec3(-0.339518,0.511514,-0.789355),vec3(-0.466753,-0.649705,-0.600021),vec3(-0.512577,-0.844025,-0.157757),vec3(-0.869362,-0.172863,-0.462955),vec3(-0.768295,-0.545301,-0.335218),vec3(-0.974889,-0.0797954,-0.207904),vec3(-0.580815,-0.465492,-0.667811),vec3(-0.342499,0.036841,0.938796),vec3(0.827917,0.0501437,-0.558604),vec3(-0.39676,0.787479,0.471655),vec3(-0.689548,0.325915,-0.646764),vec3(-0.286164,0.93809,0.195184),vec3(-0.680276,-0.421937,0.599328),vec3(-0.280049,-0.550522,-0.786447),vec3(-0.253725,0.277821,-0.92652),vec3(-0.519516,0.364802,-0.772672),vec3(0.744284,-0.0430754,0.666473),vec3(-0.544789,-0.733346,-0.406704),vec3(0.364386,0.644561,0.672133),vec3(-0.0339625,0.146794,-0.988584),vec3(0.159183,-0.911348,0.379613),vec3(0.46649,0.461266,0.754732),vec3(-0.989875,0.136263,-0.0397485),vec3(-0.358959,0.21818,-0.907495),vec3(0.980602,-0.0733397,0.181772),vec3(0.351426,0.521163,-0.777746),vec3(-0.339848,-0.561754,0.754278),vec3(0.922215,-0.381781,0.0613326),vec3(-0.513277,-0.732375,0.447408),vec3(0.228385,0.385953,-0.893801),vec3(0.843674,-0.169676,-0.509337),vec3(-0.0975234,-0.797487,-0.595403),vec3(0.0569963,0.596316,-0.800724));
const vec3 pointColors[POINTS] = vec3[POINTS](vec3(0.87,0,0.0725),vec3(0,0.551,0.87),vec3(0.87,0.6525,0),vec3(0,0.5945,0.87),vec3(0,0.2755,0.87),vec3(0.87,0,0.5945),vec3(0.7395,0,0.87),vec3(0.551,0,0.87),vec3(0,0.87,0.1305),vec3(0.87,0,0.696),vec3(0,0.0435,0.87),vec3(0,0.87,0.435),vec3(0.464,0,0.87),vec3(0.3625,0.87,0),vec3(0.87,0,0.5945),vec3(0.87,0,0.1595),vec3(0.87,0.7975,0),vec3(0.87,0.4785,0),vec3(0.7395,0,0.87),vec3(0.87,0,0.5655),vec3(0.87,0,0.087),vec3(0.87,0.783,0),vec3(0.87,0,0.5365),vec3(0,0.87,0.319),vec3(0.87,0.58,0),vec3(0.1885,0,0.87),vec3(0.87,0,0.4495),vec3(0.2465,0.87,0),vec3(0,0.4785,0.87),vec3(0.87,0,0.0145),vec3(0.87,0,0.174),vec3(0.87,0.754,0),vec3(0,0.87,0.493),vec3(0.6235,0,0.87),vec3(0.87,0.087,0),vec3(0.87,0.725,0),vec3(0,0.58,0.87),vec3(0.029,0,0.87),vec3(0,0.87,0.3045),vec3(0.4785,0,0.87),vec3(0.7105,0.87,0),vec3(0,0.87,0.6235),vec3(0.725,0.87,0),vec3(0,0.87,0.029),vec3(0.348,0.87,0),vec3(0.87,0.493,0),vec3(0,0.232,0.87),vec3(0.696,0.87,0),vec3(0.87,0,0.6235),vec3(0.4205,0,0.87),vec3(0.116,0.87,0),vec3(0.4785,0,0.87),vec3(0,0.348,0.87),vec3(0.116,0.87,0),vec3(0.87,0.8265,0),vec3(0.87,0,0.174),vec3(0,0.87,0.783),vec3(0.87,0,0.6815),vec3(0.87,0.4495,0),vec3(0.87,0.1885,0));

float y0 = 0.0, z0 = -5.0, r = 1.0;
float cy0 = -1.0, cz0 = -8.0;
float ang = 0.0;

vec3 lightCol = vec3(1, 0.97, 0.9);

vec3
camera (vec2 uv)
{
  float fov = 1.0;
  vec3 forw  = vec3(0.0, 0.0, -1.0);
  vec3 right = vec3(1.0, 0.0, 0.0);
  vec3 up = vec3(0.0, 1.0, 0.0);

  return (normalize((uv.x) * right + (uv.y) * up + fov * forw));
}

vec3
room (vec2 ab, out vec3 normal, out vec3 albedo)
{
  float y = ab.y * cz0;
  if (y >= cy0)
  {
    normal = vec3(0, 0, 1);
    float x = ab.x * cz0;
    albedo = texture(iChannel1, vec2(x, y) * 0.25).rgb;
    return vec3(x, y, cz0);
  }
  normal = vec3(0, 1, 0);
  float z = cy0 / ab.y;
  float x = ab.x * z;
  albedo = texture(iChannel2, vec2(x, z) * 0.25).rgb;
  return vec3(x, cy0, z);
}

vec2
longlat (vec3 pos)
{
  pos -= vec3(0, y0, z0);
  return vec2(atan(pos.z, pos.x) + ang, atan(pos.y, length(pos.xz)) - 1.57);
}

vec3
voronoi (vec3 pos)
{
  pos -= vec3(0, y0, z0);
  pos /= r;
  vec3 posrot = vec3(cos(ang) * pos.x - sin(ang) * pos.z, pos.y, sin(ang) * pos.x + cos(ang) * pos.z);

  float md = 100.0, smd = 100.0;
  int m = 0, sm = -1;
  for (int i = 0; i < POINTS; i++)
  {
    float d = distance(points[i], posrot);
    if (d < md)
    {
      sm = m;
      smd = md;
      m = i;
      md = d;
    }
    else if (d < smd)
    {
      sm = i;
      smd = d;
    }
  }

  vec3 a = points[m], b = points[sm];
  float borderDist = dot(posrot - 0.5*(a+b),normalize(a-b));
  return pointColors[m] * min(1.0, borderDist * 6.0 + 0.8);
}

void
main ()
{
  vec2 R = iResolution,
  uv  = vec2(gl_FragCoord.xy - R/2.0) / R.y;
  ang = iTime * 0.5;

  vec3 dir = camera(uv);
  float a = dir.x / dir.z, b = dir.y / dir.z;
  float qa2 = 2.0 * (a * a + b * b + 1.0);
  float qb = -2.0 * (b * y0 + z0);
  float qc = y0 * y0 + z0 * z0 - r * r;
  float delta = qb * qb - 2.0 * qa2 * qc;

  vec3 roomAlbedo, roomNormal;
  vec3 roompos = room(vec2(a, b), roomNormal, roomAlbedo);
  vec3 roomposvec = roompos - vec3(0, y0, z0);
  float roomdist = length(roomposvec);
  roomposvec /= roomdist;
  vec3 shadow = voronoi(roompos);

  vec3 roomCol = roomAlbedo * shadow * lightCol * dot(-roomposvec, roomNormal) / max((roomdist * roomdist * 0.05), 0.7);

  if (delta < 0.0)
  {
    float glow = max(0.0, delta + 1.5) * 0.08;
    gl_FragColor = vec4(roomCol + glow * lightCol, 1);
    return;
  }
  float sqd = sqrt(delta);
  float z1 = (-qb + sqd) / qa2, z2 = (-qb - sqd) / qa2;
  vec3 pos1 = vec3(a * z1, b * z1, z1), pos2 = vec3(a * z2, b * z2, z2);
  vec3 texCol = texture(iChannel0, longlat(pos1)).rgb, frontCol = voronoi(pos1) * (0.9 + 0.15 * texCol.r) * lightCol, backCol = voronoi(pos2) * lightCol;

  gl_FragColor = vec4(mix(frontCol, mix(backCol, roomCol, 0.3), 0.3), 1);
}
