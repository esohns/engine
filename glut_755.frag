uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define FAR 50.

#define SHOW_DETAILS
#define SHOW_EDGES

float objID;
float bObjID;

mat2
rot (float th)
{
  vec2 a = sin(vec2(1.5707963, 0) - th);
  return mat2(a, -a.y, a.x);
}

vec3
camPath (float t)
{
  float a = sin(t * 3.14159265 / 24. + 1.5707963);
  float b = cos(t * 3.14159265 / 32.);

  return vec3(a * 4.35, b * a, t);
}

float
sminP (float a, float b, float smoothing)
{
  float h = clamp((b - a) / smoothing * .5 + .5, 0., 1.);
  return mix(b, a, h) - smoothing * h * (1. - h);
}

float
tiles (vec2 p)
{
  p = fract(p);

  float s = pow(16. * p.x * p.y * (1.0 - p.x) * (1.0 - p.y), 0.125);

  return smoothstep(0., 1., s);
}

float
lattice (vec3 p)
{
  p = abs(mod(p, 2.) - 1.);
  float x1 = min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - .32;

  p = abs(mod(p, .5) - .25);
  float x2 = min(p.x, min(p.y, p.z));

  bObjID = step(x2, x1);

  return max(x1, x2) - .08;
}

float
columns (vec3 p)
{
  p = abs(mod(p, 4.) - 2.);
	
  float x1 = max(p.x, p.z) - .32;
    
  float bl = max(max(p.x - .5, p.z - .5), p.y + .1);

  x1 = min(x1, bl);

  p = abs(mod(p, .5) - .25);
  float x2 = min(p.x, min(p.y, p.z));

  objID = step(x1, x2 - .05);

  return max(x1, x2) - .05;
}

float
map (vec3 p)
{
  float d = columns(p);

  float fl = p.y + 2.5;

  p = abs(p);

  d = sminP(d, -(p.y - 2.5 - d * .75), 1.5);

  d = min(d, -(p.x - 5.85));

  d = sminP(d, fl, .25);

  return d * .75;
}


float
trace (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for (int i = 0; i < 80; i++)
  {
    d = map(ro + rd * t);
    if (abs(d) < .001 * (t * .125 + 1.) || t > FAR)
      break;
    t += d;
  }

  return min(t, FAR);
}

vec3
tex3D (sampler2D channel, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(channel, p.yz).xyz;
  vec3 ty = texture(channel, p.xz).xyz;
  vec3 tz = texture(channel, p.xy).xyz;

  return tx * tx * n.x + ty * ty * n.y + tz * tz * n.z;
}

float
bumpFunction (vec3 p)
{
  float c = 0.;

  if (p.y > 2.15)
    c = min(abs(lattice(p * 3.)) * 1.6, 1.);
  else if (p.y > -2.15)
    c = min(abs(lattice(p * 4.)) * 1.6, 1.);
  else
    c = max(tiles(p.xz + .5) - min(abs(lattice(p * 2.)) * 1.6, 1.), 0.) * .5;

  return c;
}

vec3
doBumpMap (vec3 p, vec3 n, float bumpfactor, inout float edge)
{
  vec2 e = vec2(3. / iResolution.y, 0);

  float f = bumpFunction(p);

  float fx = bumpFunction(p - e.xyy);
  float fy = bumpFunction(p - e.yxy);
  float fz = bumpFunction(p - e.yyx);

  float fx2 = bumpFunction(p + e.xyy);
  float fy2 = bumpFunction(p + e.yxy);
  float fz2 = bumpFunction(p + e.yyx);

  vec3 grad = vec3(fx - fx2, fy - fy2, fz - fz2) / (e.x * 2.);

  edge = abs(fx + fy + fz + fx2 + fy2 + fz2 - 6. * f);
  edge = smoothstep(0., 1., edge / e.x);

  grad -= n * dot(n, grad);

  return normalize(n + grad * bumpfactor);
}

vec3
nr (vec3 p, inout float edge, float t)
{
  vec2 e = vec2(3. / iResolution.y, 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p) * 2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);

  edge = smoothstep(0., 1., sqrt(edge / e.x * 2.));

  e = vec2(.005 * min(1. + t, 5.), 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
cao (vec3 p, vec3 n)
{
  float sca = 1., occ = 0.;
  for (float i = 0.; i < 5.; i++)
  {
    float hr = .01 + i * .5 / 4.;
    float dd = map(n * hr + p);
    occ += (hr - dd) * sca;
    sca *= 0.7;
  }

  return clamp(1.0 - occ, 0., 1.);
}

float
softShadow (vec3 ro, vec3 lp, float k)
{
  const int maxIterationsShad = 20;

  vec3 rd = (lp - ro);

  float shade = 1.0;
  float dist = 0.05;
  float end = max(length(rd), 0.001);
    
  rd /= end;

  for (int i = 0; i < maxIterationsShad; i++)
  {

    float h = map(ro + rd * dist);
    shade = min(shade, smoothstep(0.0, 1.0, k * h / dist));
    dist += clamp(h, 0.01, 0.2);

    if (h < 0.001 || dist > end)
      break;
  }

  return min(max(shade, 0.) + 0.2, 1.0);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy * 0.5) / iResolution.y;

  float speed = 3.;
  vec3 ro = camPath(iTime * speed);
  vec3 lk = camPath(iTime * speed + .1);
  vec3 lp = vec3(0, 0, iTime * speed) + vec3(0, .5, 3.5);

  float FOV = 3.14159 / 3.;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x));
  vec3 up = cross(fwd, rgt);

  vec3 rd = fwd + FOV * (u.x * rgt + u.y * up);
  rd = normalize(vec3(rd.xy, (rd.z - length(rd.xy) * .125) * .75));

  rd.xy = rot(-camPath(lk.z).x / 32.) * rd.xy;

  float t = trace(ro, rd);
  float svObjID = objID;

  vec3 sp = ro + rd * t;

  float edge;
  vec3 sn = nr(sp, edge, t);

  float sh = softShadow(sp, lp, 16.);
  float ao = cao(sp, sn);

  lp -= sp;
  float lDist = max(length(lp), .0001);
  lp /= lDist;

  float atten = 1. / (1.0 + lDist * lDist * .15);

  vec3 spl = sp;
  spl.x = mod(spl.x, 2.) - 1.;
  float spot = max(4. - length(spl.xy - vec2(0, 2.)), 0.) * (sin((spl.z + 1.) * 3.14159 / 2.) * .5 + .5);
  spot = smoothstep(0.25, 1., spot);

  float edge2 = 0.;
  float svBObjID = 0.;
#ifdef SHOW_DETAILS
  sn = doBumpMap(sp, sn, .15 / (1. + t / FAR), edge2);
  svBObjID = bObjID;
#endif    

  float dif = max(dot(lp, sn), 0.);
  float spe = pow(max(dot(reflect(rd, sn), lp), 0.), 6.);
  float fre = pow(clamp(dot(rd, sn) + 1., 0., 1.), 4.);

  vec3 tx;
  if (sp.y > 2.25)
  {
    tx = tex3D(iChannel0, sp / 2., sn) * 1.;
  }
  else if ((sp.y) > 1.88)
  {
    tx = tex3D(iChannel1, sp / 1., sn);
    tx = smoothstep(0.025, .7, tx);
  }
  else if (sp.y > -1.88)
  {
    if (svObjID > .5)
    {
      tx = tex3D(iChannel1, sp / 1., sn);
      tx = smoothstep(0.025, .7, tx);
    }
    else
      tx = tex3D(iChannel0, sp / 4. + .5, sn) * 1.;
  }
  else if ((sp.y) > -2.25)
  {
    tx = tex3D(iChannel1, sp / 1., sn);
    tx = smoothstep(0.025, .7, tx);
  }
  else
  {
    tx = tex3D(iChannel0, sp / 4. + .5, sn);
    tx = smoothstep(-.15, .9, tx);
    if (svBObjID > .5)
      tx *= 2. / 1.25;
  }

#ifdef SHOW_EDGES
  tx *= (1. - edge * .7) * (1. - edge2 * .7);
#endif

  vec3 fc = tx * (dif + .25 + vec3(.5, .7, 1) * fre * 4.) + vec3(1, .7, .3) * spe * 3. + spot * tx * 3.;
  fc *= atten * sh * ao;

  vec3 bg = mix(vec3(1, .5, .3), vec3(1, .9, .5), rd.y * .5 + .5);

  fc = mix(fc, bg * 1.25, smoothstep(0., .9, t / FAR));

  fc = fc * .65 + vec3(1.2, 1.05, .9) * pow(max(fc, 0.), vec3(1, 1.2, 1.5)) * .35;

  u = gl_FragCoord.xy / iResolution.xy;
  fc = min(fc, 1.) * pow(16.0 * u.x * u.y * (1.0 - u.x) * (1.0 - u.y), .125);
    
  gl_FragColor = vec4(sqrt(clamp(fc, 0., 1.)), 1.0);
}
