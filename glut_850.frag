#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform samplerCube iChannel0;

const vec2 A = vec2(0.4, -1.5), B = vec2(0.4, 0.2), C = vec2(0.2, 0.6);
const vec2 D = vec2(0.15, 1.3), E = vec2(0.17, 1.36), F = vec2(0.15, 1.55);
const vec2 P1 = vec2(0.1399869656012, 0.2), P2 = vec2(0.4789069783753, 0.6199219270268), I = vec2(0.3032905667047,0.4023331768004);
const float r1 = distance(P1, B), r2 = distance(P2, C);
float rad = 1.65;
const float thickness = 0.02, eta = 0.7, inveta = 1.0 / eta;
const float e = 0.001;
const float r0 = (1.0 - inveta) * (1.0 - inveta) / ((1.0 + inveta) * (1.0 + inveta)), reflectiveness = 5.0, minrefl = 0.1, maxrefl = 0.8;
const vec3 bottlecol = vec3(0.312, 0.239, 0.075) / 0.312;
vec3 beercol = vec3(0.984, 0.894, 0.590);

const vec3 bp = vec3(0, 0, -4);
const float bh = 1.0;
float angY = 0.0, angX = 0.2, cangY, sangY;
vec3 beerpos;
float beerstretch, beerampl;
float sloshfreq = 7.5;
vec3 lightdir = normalize(vec3(1, 1, 1));

mat3 rotmat, invrotmat;

const float stepsize = 0.05, smallstepsize = 0.005;

vec3
camera (vec2 uv)
{
  float fov = 1.0;
  vec3 forw  = vec3(0.0, 0.0, -1.0);
  vec3 right = vec3(1.0, 0.0, 0.0);
  vec3 up = vec3(0.0, 1.0, 0.0);

  return normalize(uv.x * right + uv.y * up + fov * forw);
}

mat3
rotateX (float theta)
{
  float c = cos(theta);
  float s = sin(theta);
  return mat3(
    vec3(1, 0, 0),
    vec3(0, c, -s),
    vec3(0, s, c)
  );
}

mat3
rotateY (float theta)
{
  float c = cos(theta);
  float s = sin(theta);
  return mat3(
    vec3(c, 0, s),
    vec3(0, 1, 0),
    vec3(-s, 0, c)
  );
}

float
segm (vec2 p, vec2 a, vec2 b)
{
  vec2 pa = p-a, ba = b-a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h );
}

float
arc (vec2 p, vec2 c, vec2 p1, vec2 p2)
{
  vec2 v1 = p1 - c;
  vec2 v2 = p2 - c;
  vec2 v = p - c;
  vec2 w = vec2( dot(v, vec2( v1.y,-v1.x)), dot(v, vec2(-v2.y, v2.x)));
  bool longarc = dot(v1, vec2(-v2.y, v2.x)) < 0.0;
  float z = longarc ? max(w.x,w.y) : min(w.x,w.y); 
  return z>0.0 ? 100.0 : abs(length(v) - length(v1));
}

bool
insideBottle (vec2 p)
{
  p.x = abs(p.x);
  if (p.x > A.x || p.y < A.y || p.y > F.y + thickness)
      return false;
  if (p.y < B.y)
      return true;
  if (p.y < I.y)
      return distance(p, P1) < r1;
  if (p.y < C.y)
      return distance(p, P2) > r2;
  if (p.y < D.y)
      return p.x < mix(C.x, D.x, (p.y - C.y) / (D.y - C.y));
  if (p.y < E.y)
      return p.x < mix(D.x, E.x, (p.y - D.y) / (E.y - D.y));

  return p.x < mix(E.x, F.x, (p.y - E.y) / (F.y - E.y));
}

float
bottle2D (vec2 p)
{
  if (p.y < -1.0)
    return min(segm(p, A, B), segm(p, A, vec2(0, A.y))) - thickness;
  if (p.y < B.y)
  {
    if (p.x < B.x) 
      return min(segm(p, A, B), distance(p, P2) - r2) - thickness;
    return segm(p, A, B) - thickness;
  }

  return min(min(min(min(arc(p, P2, C, I), arc(p, P1, B, I)), segm(p, C, D)), segm(p, D, E)), segm(p, E, F)) - thickness;
}

float
bottle3D (vec3 p)
{
  p -= bp;
  p *= invrotmat;

  return bottle2D(vec2(length(p.xz), p.y));
}

bool
insideBottle3D (vec3 p)
{
  p -= bp;
  p *= invrotmat;

  return insideBottle(vec2(length(p.xz), p.y));
}

vec3
calcNormal (vec3 p)
{
  const float h = 0.0001;
  const vec2 k = vec2(1,-1);

  return normalize( k.xyy*bottle3D( p + k.xyy*h ) + 
                    k.yyx*bottle3D( p + k.yyx*h ) + 
                    k.yxy*bottle3D( p + k.yxy*h ) + 
                    k.xxx*bottle3D( p + k.xxx*h ) );
}

float
fresnel (vec3 I, vec3 N)
{
  return clamp(reflectiveness * (r0 + (1.0 - r0) * pow(1.0 - abs(dot(I, N)), 5.0)), minrefl, maxrefl);
}

float
sloshfunc1D (float x, float a, float t)
{
  float stx = x * sin(t);

  return a * (stx + (max(0.0, a - 0.25) * (exp(2.0 * stx) - 1.0)) * 0.5);
}

float
sloshfunc3D (vec3 pos, float a, float t)
{
  vec3 center = rotmat * vec3(0, bh + A.y + pos.y - beerpos.y, 0) + bp;
  vec2 xz = vec2(pos.x - center.x, pos.z - center.z);
  xz = vec2(cangY * xz.x - sangY * xz.y, (sangY * xz.x + cangY * xz.y) * beerstretch) / A.x;

  return A.x * sloshfunc1D(xz.y, a, t) + beerpos.y;
}

vec3
sloshfuncnormal3D (vec3 pos, float a, float t)
{
  float h = 0.0001;
  float s = sloshfunc3D(pos, a, t);
  float dx = (sloshfunc3D(pos + vec3(h, 0, 0), a, t) - s) / h;
  float dz = (sloshfunc3D(pos + vec3(0, 0, h), a, t) - s) / h;

  return -normalize(cross(vec3(1, dx, 0), vec3(0, dz, 1)));
}

void
bottleCol (vec3 pos, vec3 dir, out vec4 fragColor)
{
  vec3 normal = calcNormal(pos);
  vec4 reflcol = vec4(texture(iChannel0, reflect(dir, normal), 0.).rgb, fresnel(dir, normal));
  vec4 beerreflcol = vec4(0, 0, 0, 0);
  dir = refract(dir, normal, eta);
  float d = bottle3D(pos);
  for (int i = 0; i < 100; i++)
  {
    if (d > e)
      break;
    pos += max(abs(d), e) * dir;
    d = bottle3D(pos);
  }
    
  if (!insideBottle3D(pos))
  {
    fragColor = vec4(mix(textureLod(iChannel0, dir, 0.).rgb * bottlecol, reflcol.rgb, reflcol.a), 1);
    return;
  }

  bool insidebeer = true, switched = false;
  bool beertint = true;
  if (pos.y > sloshfunc3D(pos, beerampl, iTime * sloshfreq))
  {
    insidebeer = false;
    beertint = false;
    normal = -calcNormal(pos);
    vec3 refr = refract(dir, normal, inveta);
    if (refr == vec3(0, 0, 0))
    {
      vec3 odir = dir;
      dir = reflect(dir, normal);
      pos += e * dir;
      d = bottle3D(pos);
      int i;
      for (i = 0; i < 100; i++)
      {
        if (d > e)
            break;
        pos += max(abs(d), e) * dir;
        d = bottle3D(pos);
      }
      vec3 refr = refract(dir, -calcNormal(pos), inveta);
      if (refr != vec3(0, 0, 0))
        dir = refr;
      else
        dir = odir;
      fragColor = vec4(mix(textureLod(iChannel0, dir, 0.).rgb * bottlecol, reflcol.rgb, reflcol.a), 1);
      return;
    }
    dir = refr;
  }
    
  for (int i = 0; i < 100; i++)
  {
    if (pos.y > F.y + thickness)
    {
      fragColor = vec4(mix(textureLod(iChannel0, dir, 0.).rgb * bottlecol, reflcol.rgb, reflcol.a), 1);
      return;
    }
        
    if (d < e)
      break;
    if (!switched)
    {
      pos += clamp(d, e, stepsize) * dir;
      float dif = pos.y - sloshfunc3D(pos, beerampl, iTime * sloshfreq);
      if (!insidebeer && dif < 0.0)
      {
        for (int j = 0; j < 50; j++)
        {
          pos -= smallstepsize * dir;
          dif = pos.y - sloshfunc3D(pos, beerampl, iTime * sloshfreq);
          if (dif > 0.0)
            break;
        }
        vec3 beernormal = sloshfuncnormal3D(pos, beerampl, iTime * sloshfreq);
        beerreflcol = vec4(textureLod(iChannel0, reflect(dir, beernormal), 0.).rgb, 0.3);
        dir = refract(dir, beernormal, 0.95);
        insidebeer = true;
        beertint = true;
        switched = true;
      }
      else if (insidebeer && dif > 0.0)
      {
        for (int j = 0; j < 50; j++)
        {
          pos -= smallstepsize * dir;
          dif = pos.y - sloshfunc3D(pos, beerampl, iTime * sloshfreq);
          if (dif < 0.0)
            break;
        }
        vec3 beernormal = sloshfuncnormal3D(pos, beerampl, iTime * sloshfreq);
        vec3 refr = refract(dir, -beernormal, inveta);
        if (refr == vec3(0, 0, 0))
          dir = reflect(dir, beernormal);
        else
        {
          insidebeer = false;
          dir = refr;
        }
        switched = true;
      }
    }
    else
      pos += max(d, e) * dir;
    d = bottle3D(pos);
  }
    
  if (!insidebeer)
    dir = refract(dir, calcNormal(pos), eta);
  for (int i = 0; i < 100; i++)
  {
    if (d > e)
      break;
    pos += max(abs(d), e) * dir;
    d = bottle3D(pos);
  }

  dir = refract(dir, -calcNormal(pos), inveta);

  vec3 backcol = mix(textureLod(iChannel0, dir, 0.).rgb, beerreflcol.rgb, beerreflcol.a) * bottlecol;
  if (beertint)
    backcol *= beercol;
  fragColor = vec4(mix(backcol * bottlecol, reflcol.rgb, reflcol.a), 1);
}

void
castRay (vec3 dir, out vec4 fragColor)
{
  float a = dir.x / dir.z, b = dir.y / dir.z;
  float qa2 = 2.0 * (a * a + b * b + 1.0);
  float qb = -2.0 * (a * bp.x + b * bp.y + bp.z);
  float qc = bp.x * bp.x + bp.y * bp.y + bp.z * bp.z - rad * rad;
  float delta = qb * qb - 2.0 * qa2 * qc;
    
  if (delta < 0.0)
  {
    fragColor = textureLod(iChannel0, dir, 0.);
    return;
  }
  float z = (-qb + sqrt(delta)) / qa2;
  vec3 pos = vec3(a * z, b * z, z);
  for (int i = 0; i < 50; i++)
  {
    float dist = bottle3D(pos);
    if (dist < e)
    {
      bottleCol(pos, dir, fragColor);
      return;
    }
    pos += dir * dist;
    if (distance(pos, bp) > rad)
    {
      fragColor = textureLod(iChannel0, dir, 0.);
      return;
    }
  }
  fragColor = textureLod(iChannel0, dir, 0.);
}

void
main ()
{
  vec2 R = iResolution,
  uv  = vec2(gl_FragCoord.xy - R/2.0) / R.y;

  float t = mod(iTime, 25.1327408) *  2.0 + 6.0;
  angY = 0.5 * (t + 1.57);
  if (int((t + 0.6) / 3.1415) % 4 == 0)
    angX = 0.2 + sin(10.0 * t) * (0.13 + 0.13 * cos(2.0 * (t + 0.6) + 3.1415));
  else
    angX = 0.2;
  float g = 0.5 * (0.4 * mod(t, 12.56637) - 1.7);
  beerampl = 0.55 * exp(-pow(g*exp(-g) + 1.0, 2.0)) * tanh(10.0 * (g + 0.85)) * exp(-pow(g, 3.0)) + 0.1;

  cangY = cos(angY); sangY = sin(angY);
  invrotmat = rotateY(-angY) * rotateX(-angX);
  rotmat = rotateY(angY) * rotateX(angX);
  beerpos = rotmat * vec3(0, bh + A.y, 0) + bp;
  beerstretch = abs(cos(angX));

  vec3 dir = camera(uv);
  castRay(dir, gl_FragColor);
}
